#define CATCH_CONFIG_RUNNER

#include "catch.hpp"
#include "mpi_functions.hpp"


int main(int argc, char* argv[])
{

    MpiWrapper::Mpi::init();
    
    Catch::Session session;
  
    const int result = session.run(argc, argv);

    MpiWrapper::Mpi::finalize();
    return result;
    
    
}
