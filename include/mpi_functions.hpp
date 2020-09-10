#pragma once

#include <mpi.h>
#include <numeric> //std::accumulate
#include "array_casts.hpp"
#include "runtime_assert.hpp"


namespace MpiWrapper {

///
///@brief Wrappers around mpi-functions for
///
///
struct Mpi {

    ///
    ///@brief Call MPI_Init.
    ///
    ///
    static void init() { MPI_Init(NULL, NULL); }



    ///
    ///@brief Call mpi abort on the WORLD COMM
    ///
    ///
    static void abort() {
        MPI_Abort(MPI_COMM_WORLD, 1); 
    }


    ///
    ///@brief Checks if MPI_Initialized has been called.
    ///
    ///@return int called = 1, not called = 0
    ///
    static int initialized() {

        int MPI_is_initialized;
        MPI_Initialized(&MPI_is_initialized);
        return MPI_is_initialized;
    }

    ///
    ///@brief Checks if mpi-finalized has been called.
    ///
    ///@return int called = 1, not called = 0
    ///
    static int finalized() {

        int MPI_is_finalized;
        MPI_Finalized(&MPI_is_finalized);
        return MPI_is_finalized;
    }

    ///
    ///@brief Finalize mpi, throws on failure in debug mode.
    ///
    ///
    static void finalize() {

        int err = MPI_Finalize();
        Utils::runtime_assert(err == MPI_SUCCESS, "Mpi::finalize fails");
    }

    ///
    ///@brief Frees the given communicator handle, throws on failure in debug mode.
    ///
    ///@param comm the handle to free
    ///
    static void comm_free(MPI_Comm comm) {

        int err = MPI_Comm_free(&comm);
        Utils::runtime_assert(err == MPI_SUCCESS, "Mpi::free fails");
    }

    ///
    ///@brief Get the rank of this process in the given handle, throws on failure in debug mode.
    ///
    ///@param comm the handle to check the rank from
    ///@return int rank
    ///
    static int get_rank(MPI_Comm comm) {

        int rank;
        int err = MPI_Comm_rank(comm, &rank);
        Utils::runtime_assert(err == MPI_SUCCESS, "Mpi::Comm_rank fails");

        return rank;
    }


    ///
    ///@brief Get the rank of this process in MPI_COMM_WORLD
    ///
    ///@return int rank of the process
    ///
    static int get_world_rank() {

        return get_rank(MPI_COMM_WORLD);
    }



    ///
    ///@brief Checks the size of the given handle, throws on failure in debug mode.
    ///
    ///@param comm the handle to check the size from
    ///@return int the size
    ///
    static int comm_size(MPI_Comm comm) {

        int size;
        int err = MPI_Comm_size(comm, &size);
        Utils::runtime_assert(err == MPI_SUCCESS, "MPI_Comm size fails.");

        return size;
    }


    ///
    ///@brief Determines the number of processes
    ///
    ///@return number of processes specified when the program was launched
    ///
    static int world_size() {

        return comm_size(MPI_COMM_WORLD);

    }



    ///
    ///@brief Free the given mpi-datatype, throws on failure in debug mode.
    ///
    ///@param t type to free
    ///
    static void type_free(MPI_Datatype t) {

        int err = MPI_Type_free(&t);
        Utils::runtime_assert(err == MPI_SUCCESS, "MPI_Type_free fails");
    }

    ///
    ///@brief Commit the given mpi datatype, throws on failure in debug mode.
    ///
    ///@param t datatype to commit
    ///
    static void type_commit(MPI_Datatype t) {

        int err = MPI_Type_commit(&t);
        Utils::runtime_assert(err == MPI_SUCCESS, "MPI_Type_commit fails.");
    }

    ///
    ///@brief Call mpi abort on the given communicator
    ///
    ///@param comm communicator handle to abort
    ///
    static void abort(MPI_Comm comm) { MPI_Abort(comm, 1); }

    ///
    ///@brief Checks if the handle has topology information attached, can throw in debug mode
    ///
    ///@param comm the mpi communicator handle to test
    ///@return true if topology information available
    ///@return false if no topology information is available
    ///
    static bool topo_test(MPI_Comm comm) {
        int status;
        int err = MPI_Topo_test(comm, &status);
        Utils::runtime_assert(err == MPI_SUCCESS, "MPI_Topo_test fails.");

        return status != MPI_UNDEFINED;
    }

    ///
    ///@brief Creates a new MPI_Comm handle using the standard MPI_Cart_create, can throw in debug
    ///mode
    ///
    ///@param old old handle (typically MPI_COMM_WORLD)
    ///@param dims dimensions of the cartesian topology
    ///@param periods periodicity information in each direction
    ///@param reorder whether to reoder or not
    ///@return MPI_Comm the new handle with Cartesian topology attached
    ///
    template <size_t N>
    static MPI_Comm cart_create(MPI_Comm                     old,
                                const std::array<size_t, N>& dims,
                                const std::array<size_t, N>& periods,
                                size_t                       reorder)  {

        auto dims_int    = Utils::cast_to_intarray(dims);
        auto periods_int = Utils::cast_to_intarray(periods);

        MPI_Comm new_handle;

        Utils::runtime_assert(
            Mpi::comm_size(old) ==
                std::accumulate(dims_int.begin(), dims_int.end(), 1, std::multiplies<int>()),
            "dimension size different than world size");

        int err = MPI_Cart_create(
            old, N, dims_int.begin(), periods_int.begin(), int(reorder), &new_handle);

        Utils::runtime_assert(err == MPI_SUCCESS, "MPI_Cart_create fails.");

        return new_handle;
    }

    ///
    ///@brief Determines the topology coordinates of the process, can throw in debug mode.
    ///
    ///@param comm Cartesian topology handle
    ///@param rank rank of the process to get coords for
    ///@param n_dimensions number of spatial dimensions
    ///@return coords coordinates of the process at rank
    ///
    static void
    cart_coords(MPI_Comm comm, int rank, int n_dimensions, int* coords) {

        Utils::runtime_assert(topo_test(comm), "No cartesian topology attached.");
        int err = MPI_Cart_coords(comm, rank, n_dimensions, coords);
        Utils::runtime_assert(err == MPI_SUCCESS, "MPI_Cart_coords fails.");
    }

    ///
    ///@brief Get the topology dimensions, periods and process coordinates, can throw in debug mode.
    ///
    ///@param comm Cartesian topology handle
    ///@param n_dimensions number of spatial dimensions
    ///@return dims topology dimensions
    ///@return periods topology periods
    ///@return coords process coordinates
    ///
    static void cart_get(MPI_Comm comm,
                         int      n_dimensions,
                         int*     dims,
                         int*     periods,
                         int*     coords) {

        Utils::runtime_assert(topo_test(comm), "No cartesian topology attached.");
        int err = MPI_Cart_get(comm, n_dimensions, dims, periods, coords);
        Utils::runtime_assert(err == MPI_SUCCESS, "MPI_Cart_get fails.");
    }

    ///
    ///@brief Get the rank of the process at coords, can throw in debug mode.
    ///
    ///@param comm Cartesian topology handle
    ///@param coords Coordinates to determine the rank for
    ///@return int rank
    ///
    static int cart_rank(MPI_Comm comm, const int* coords) {
        Utils::runtime_assert(topo_test(comm), "No cartesian topology attached.");
        int rank;
        int err = MPI_Cart_rank(comm, coords, &rank);
        Utils::runtime_assert(err == MPI_SUCCESS, "MPI_Cart_rank fails.");
        return rank;
    }
};

} // namespace MpiWrapper