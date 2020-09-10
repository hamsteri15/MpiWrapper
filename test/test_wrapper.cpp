#include "catch.hpp"

#include "mpi_communicator.hpp"
#include "mpi_cart_communicator.hpp"
#include "mpi_native_datatypes.hpp"



TEST_CASE("Communicator tests"){

    using namespace MpiWrapper;

    REQUIRE_NOTHROW(Communicator());

}

TEST_CASE ("CartCommunicator constructors"){

    using namespace MpiWrapper;

    size_t world_size = static_cast<size_t>(Mpi::world_size());


    // 1D tests
    std::array<size_t, 1> dims{world_size};
    std::array<size_t, 1> periods{0};

    CartCommunicator comm1(dims, periods, 0);

    
    CartCommunicator comm2 = CartCommunicator( dims, periods, 0);
    
    REQUIRE_NOTHROW( comm2.get_rank());

    dims[0] = world_size + 1;
    //REQUIRE_THROWS(CartCommunicator(dims, periods, 0));


    CartCommunicator comm3(comm2);
    CartCommunicator comm4 = comm2;

    REQUIRE_NOTHROW(comm3.get_rank());
    REQUIRE_NOTHROW(comm4.get_rank());


    //3D tests
    std::array<size_t, 3> dims_3d{1, world_size, 1};
    std::array<size_t, 3> periods_3d{1, 1,0};

    CartCommunicator comm1_3d( dims_3d, periods_3d, 0);
    CartCommunicator comm2_3d = CartCommunicator(dims_3d, periods_3d, 0);
    
    REQUIRE_NOTHROW( comm2_3d.get_rank());

    dims_3d = std::array<size_t, 3>{1, world_size + 1, 1};
    REQUIRE_THROWS(CartCommunicator(dims_3d, periods_3d, 0));


}


TEST_CASE("CartCommunicator::shift()"){

    using namespace MpiWrapper;

    size_t world_size = static_cast<size_t>(Mpi::world_size());


    std::array<size_t, 3> dims{world_size, 1, 1};
    std::array<size_t, 3> periods{0,1,0};
    
    auto comm = CartCommunicator( dims, periods);
    std::array<int,3> dir;

    //periodic direction
    dir = {0, 1, 0};
    auto s1 = comm.shift(dir);
    CHECK(s1.first == comm.get_rank());
    CHECK(s1.second == comm.get_rank());
    
    //non-periodic
    dir = {0, 1, 1};
    auto s2 = comm.shift(dir);
    CHECK(s2.first == MPI_PROC_NULL);
    CHECK(s2.second == MPI_PROC_NULL);



    //regular shift
    dir = {1, 0, 0};
    auto s3 = comm.shift(dir);
    int source = s3.first;
    int dest = s3.second;

    if (world_size == 1) {
        CHECK(source == MPI_PROC_NULL);
        CHECK(dest == MPI_PROC_NULL);
    }

    else {

        if (comm.get_coords()[0] == 0){
            CHECK(source != MPI_PROC_NULL);
            CHECK(dest == MPI_PROC_NULL);
        }

        else if (comm.get_coords()[0] == (world_size - 1) ){
            CHECK(source == MPI_PROC_NULL);
            CHECK(dest != MPI_PROC_NULL);
        }

        else {
            CHECK(source != MPI_PROC_NULL);
            CHECK(dest != MPI_PROC_NULL);
        }


    }


}



TEST_CASE("Datatype tests"){

    using namespace MpiWrapper;

    REQUIRE_NOTHROW(MpiDatatype<void>());
    REQUIRE_NOTHROW(MpiDatatype<double>());

}