#pragma once
#include <mpi.h>

namespace MpiWrapper {

// CRTP injection of ~ operator
template <class T> class MpiDatatypeBase {

public:
    constexpr MPI_Datatype operator~() { return static_cast<T*>(this)->get_handle(); }

    constexpr MPI_Datatype operator~() const {
        return static_cast<const T*>(this)->get_handle();
    }
};

} // namespace MpiWrapper