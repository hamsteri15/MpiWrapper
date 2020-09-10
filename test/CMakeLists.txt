

# Build catch mpi main
add_library(catch_mpi_main STATIC catch_mpi_main.cpp)

target_link_libraries(catch_mpi_main PUBLIC mpi_wrapper)


SET(TestSources
    test_wrapper.cpp;
)

add_executable(TestWrapper.bin ${TestSources})
target_link_libraries(TestWrapper.bin PUBLIC project_options catch_mpi_main mpi_wrapper)
target_compile_options(TestWrapper.bin PRIVATE -DDEBUG)

#serial execution of mpi code
add_test( NAME WrapperMpiTest0 
          COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestWrapper.bin)


add_test( NAME WrapperMpiTest1 
          COMMAND mpirun -np 1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestWrapper.bin)

add_test( NAME WrapperMpiTest2 
          COMMAND mpirun -np 2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestWrapper.bin
          )

add_test( NAME WrapperMpiTest4 
          COMMAND mpirun -np 4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestWrapper.bin
)   

