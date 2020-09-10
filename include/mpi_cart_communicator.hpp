#pragma once

#include <array>
#include <utility> //std pair

#include "mpi_communicator.hpp"
#include "mpi_functions.hpp"
#include "array_casts.hpp"
//#include "utils/array_op_overloads.hpp"

namespace MpiWrapper {

template <size_t N> // N dimensions
class CartCommunicator : public Communicator {
public:
    CartCommunicator() = default;

    ///
    ///@brief Construct a new Cart Communicator object
    ///
    
    ///@param dims topology dimensions
    ///@param periods periodicity information, array of 0/1
    ///@param reorder whether to reorder the ranks or not 0/1, default = 1
    ///@param old old communicator, default = Commuincator() (i.e. MPI_COMM_WORLD)
    ///
    CartCommunicator(const std::array<size_t, N>& topo_dims,
                     const std::array<size_t, N>& periods,
                     size_t                       reorder = 1,
                     const Communicator&          old     = Communicator())
        : Communicator(Mpi::cart_create(old.get_handle(), topo_dims, periods, reorder)) {}



    ///
    ///@brief Get the coordrinates of this process in the cartesian mpi topology
    ///
    ///@return std::array<size_t, N> array of coordinates
    ///
    std::array<size_t, N> get_coords() const {

        std::array<int, N> coords;
        Mpi::cart_coords(this->get_handle(), this->get_rank(), N, coords.begin());
        return Utils::cast_to_uintarray(coords);
    }

    ///
    ///@brief Get the periodicity information
    ///
    ///@return std::array<size_t, N> array of periodicity information corresponding to each
    /// direction N
    ///
    std::array<size_t, N> get_periods() const {

        std::array<int, N> dims, periods, coords;
        Mpi::cart_get(this->get_handle(), N, dims.data(), periods.data(), coords.data());
        return Utils::cast_to_uintarray(periods);
    }

    ///
    ///@brief Get the topology dimensions
    ///
    ///@return std::array<size_t, N> array of dimensions in each direction
    ///
    std::array<size_t, N> get_topology_dims() const {

        std::array<int, N> dims, periods, coords;
        Mpi::cart_get(this->get_handle(), N, dims.data(), periods.data(), coords.data());
        return Utils::cast_to_uintarray(dims);
    }

    ///
    ///@brief Given an array of integers returns the corresponding source and destination ranks.
    /// Similar to
    ///       the standard MPI_Cart_shift() but works for multiple dimensions at once. As an
    ///       example, for a N=3 size topology a shift with [0,1,1] the source would be "northeast"
    ///       and the destination would be southwest.
    ///
    ///
    ///@param dir direction to with respect to perform the shift
    ///@return std::pair<int, int> pair of source and dest rank ids
    ///
    std::pair<int, int> shift(const std::array<int, N>& dir) {

        //using namespace Utils::ArrayOpOverloads;

        auto my_coords     = Utils::cast_to_intarray(get_coords());
        std::array<int, N> source_coords;
        std::array<int, N> dest_coords;
        for (size_t i = 0; i < N; ++i){
            source_coords[i] = my_coords[i] + dir[i];
            dest_coords[i] = my_coords[i] - dir[i];
        }

        //auto source_coords = my_coords + dir;
        //auto dest_coords   = my_coords - dir;

        source_coords = check_periodicity(source_coords);
        dest_coords   = check_periodicity(dest_coords);

        int source = coordinates_to_rank(source_coords);
        int dest   = coordinates_to_rank(dest_coords);

        return std::make_pair(source, dest);
    }



    ///
    ///@brief Given the topology coordinates, returns the corresponding mpi-rank.
    ///
    ///@param coords coordinates to convert
    ///@return int   the rank number or MPI_PROC_NULL if rank out of bounds
    ///
    int coordinates_to_rank(const std::array<int, N>& coords) const {

        auto topo_dims = get_topology_dims();

        for (size_t i = 0; i < N; ++i) {
            if ((coords[i] < 0) || (coords[i] >= int(topo_dims[i]))) { return MPI_PROC_NULL; }
        }

        return Mpi::cart_rank(this->get_handle(), coords.cbegin());
    }


private:
    ///
    ///@brief Checks that the given input coordinates are in bounds in the mpi-topology. Fixes the
    /// coordinates
    ///       accordingly if periodicity is assumed and coordinates are out of bounds by a maximum
    ///       of 1 index.
    ///
    ///@param coords Coordinates to check
    ///@return std::array<int, N> New coordinates where periodicity is taken into account for out of
    /// bounds
    ///                           coordinates
    ///
    std::array<int, N> check_periodicity(const std::array<int, N>& coords) const {

        auto               topo_dims = get_topology_dims();
        auto               periods   = get_periods();
        std::array<int, N> new_coords{coords};

        for (size_t i = 0; i < N; ++i) {

            if (periods[i] == 1) {

                if (coords[i] == -1) {
                    new_coords[i] = int(topo_dims[i] - 1);
                } else if (coords[i] == int(topo_dims[i])) {
                    new_coords[i] = 0;
                }
            }
        }

        return new_coords;
    }

    
};

} // namespace MpiWrapper