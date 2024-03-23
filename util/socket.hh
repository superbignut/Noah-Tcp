#pragma once

#include "address.hh"
#include "file_descriptor.hh"
#include <sys/socket.h>
#include <functional>

// base class for network sockets (TCP. UDP, etc)
class Socket : public FileDescriptor
{

private:
    /// @brief get the local or peer address the socket is connected to
    /// @param name_of_function
    /// @param function
    /// @return
    Address get_address(const std::string &name_of_function,
                        const std::function<int(int, sockaddr *, socklen_t *)> &function) const;

protected:
    Socket(int domain, int type, int protocol = 0);

    Socket(FileDescriptor &&fd, int domain, int type, int protocol = 0);

    template <typename option_type>
    socklen_t get_socket_opt(int level, int option, option_type &option_value) const;

    template <typename option_type>
    void set_socket_opt(int level, int option, option_type &option_value);

    void set_socket_opt(int level, int option, std::string_view option_value);

public:
    void bind(const Address& address);

    void bind_to_device(std::string_view device_name);

    void connect(const Address& address);

    void shutdown(int how);

    Address local_address() const;

    Address peer_address() const;

    void set_reuse_addr();

    void throw_if_error() const;
};

class DatagrameSocket : public Socket
{
};

class UDPSocket : public DatagrameSocket
{
};

class TCPSocket : public Socket
{
};

class PacketSocket : public DatagrameSocket
{
};

class LocalStreamSocket : public Socket
{
};

class LocalDatagramSocket : public DatagrameSocket
{
};
