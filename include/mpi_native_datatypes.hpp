#pragma once

#include <mpi.h>

#include "mpi_datatype.hpp"
#include "mpi_datatype_base.hpp"

namespace MpiWrapper {

/// Conversions of native dataypes to MpiDatatypes

template <> struct MpiDatatype<void> : MpiDatatypeBase<MpiDatatype<void>> {

     static MPI_Datatype get_handle() { return MPI_DATATYPE_NULL; }
};

template <> struct MpiDatatype<signed char> : MpiDatatypeBase<MpiDatatype<signed char>> {

     static MPI_Datatype get_handle() { return MPI_SIGNED_CHAR; }
};

template <> struct MpiDatatype<unsigned char> : MpiDatatypeBase<MpiDatatype<unsigned char>> {

     static MPI_Datatype get_handle() { return MPI_UNSIGNED_CHAR; }
};

template <> struct MpiDatatype<short> : MpiDatatypeBase<MpiDatatype<short>> {

     static MPI_Datatype get_handle() { return MPI_SHORT; }
};

template <> struct MpiDatatype<unsigned short> : MpiDatatypeBase<MpiDatatype<unsigned short>> {

     static MPI_Datatype get_handle() { return MPI_UNSIGNED_SHORT; }
};

template <> struct MpiDatatype<int> : MpiDatatypeBase<MpiDatatype<int>> {

     static MPI_Datatype get_handle() { return MPI_INT; }
};

template <> struct MpiDatatype<unsigned> : MpiDatatypeBase<MpiDatatype<unsigned>> {

     static MPI_Datatype get_handle() { return MPI_UNSIGNED; }
};

template <> struct MpiDatatype<long> : MpiDatatypeBase<MpiDatatype<long>> {

     static MPI_Datatype get_handle() { return MPI_LONG; }
};

template <> struct MpiDatatype<unsigned long> : MpiDatatypeBase<MpiDatatype<unsigned long>> {

     static MPI_Datatype get_handle() { return MPI_UNSIGNED_LONG; }
};

template <> struct MpiDatatype<long long> : MpiDatatypeBase<MpiDatatype<long long>> {

     static MPI_Datatype get_handle() { return MPI_LONG_LONG; }
};

template <>
struct MpiDatatype<unsigned long long> : MpiDatatypeBase<MpiDatatype<unsigned long long>> {

     static MPI_Datatype get_handle() { return MPI_UNSIGNED_LONG_LONG; }
};

template <> struct MpiDatatype<float> : MpiDatatypeBase<MpiDatatype<float>> {

     static MPI_Datatype get_handle() { return MPI_FLOAT; }
};

template <> struct MpiDatatype<double> : MpiDatatypeBase<MpiDatatype<double>> {

     static MPI_Datatype get_handle() { return MPI_DOUBLE; }
};

template <> struct MpiDatatype<bool> : MpiDatatypeBase<MpiDatatype<bool>> {

     static MPI_Datatype get_handle() { return MPI_C_BOOL; }
};

} // namespace MpiWrapper