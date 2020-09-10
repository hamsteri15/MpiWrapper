#pragma once

#include "mpi_datatype_base.hpp"
#include "mpi_functions.hpp"

namespace MpiWrapper {

class Communicator {
public:
    // default to world
    Communicator()
        : m_handle(MPI_COMM_WORLD)
        , m_should_free(false) {}

    explicit Communicator(MPI_Comm handle)
        : m_handle(handle)
        , m_should_free(true) {}

    // copy
    Communicator(const Communicator& other)
        : m_handle(other.m_handle)
        , m_should_free(false) {}

    // move
    Communicator(Communicator&& other)
        : m_handle(other.m_handle)
        , m_should_free(other.m_should_free) {
        other.m_should_free = false;
    }

    ~Communicator() { free(); }

    ///
    ///@brief Wrapper around MPI_Sendrecv
    ///
    ///@tparam T send and receive buffer type
    ///@tparam ST sendtype base class
    ///@tparam RT recvtype base class
    ///@param send_buffer buffer to take the send data from
    ///@param sendcount   number of sendtypes to send
    ///@param sendtype    the mpi-datatype of the send element
    ///@param dest_rank   rank of the destination
    ///@param recv_buffer buffer to place the received data
    ///@param recvcount   number of recvtype to receive
    ///@param recvtype    mpi-datatype of the reveived element
    ///@param source_rank rank of the source
    ///
    template <class T, class ST, class RT>
    void send_recv(const T*                   send_buffer,
                   int                        sendcount,
                   const MpiDatatypeBase<ST>& sendtype,
                   int                        dest_rank,
                   T*                         recv_buffer,
                   int                        recvcount,
                   const MpiDatatypeBase<RT>& recvtype,
                   int                        source_rank) {

        // MPI_Status status;
        int sendtag = 1;
        int recvtag = 1;
        int err     = MPI_Sendrecv(send_buffer,
                               sendcount,
                               ~sendtype,
                               dest_rank,
                               sendtag,
                               recv_buffer,
                               recvcount,
                               ~recvtype,
                               source_rank,
                               recvtag,
                               m_handle,
                               MPI_STATUS_IGNORE);

        if (err != MPI_SUCCESS) { throw "MPI_Sendrcv() fails."; }
    }

    ///
    ///@brief Get the mpi-handle
    ///
    ///@return MPI_Comm handle
    ///
    MPI_Comm get_handle() const{ return m_handle; }



    ///
    ///@brief Get the rank of this process in the handle
    ///
    ///@return int rank
    ///
    int get_rank() const { return Mpi::get_rank(m_handle); }

    ///
    ///@brief Get the world size
    ///
    ///@return int world size
    ///
    int size() const { return Mpi::comm_size(m_handle); }

private:
    MPI_Comm m_handle;
    bool     m_should_free;

    ///
    ///@brief Check if the handle is a builtin type which sould not be freed
    ///
    ///@param c handle to check
    ///@return true is a builtin type
    ///@return false not a builtin type
    ///
    bool is_builtin(MPI_Comm c) const {
        return c == MPI_COMM_WORLD || c == MPI_COMM_SELF || c == MPI_COMM_NULL;
    }

    ///
    ///@brief Safely free m_handle
    ///
    ///
    void free() {
        if (!is_builtin(m_handle) && m_should_free) { Mpi::comm_free(m_handle); }
    }
};

} // namespace MpiWrapper