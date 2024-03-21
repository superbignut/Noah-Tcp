#pragma once

#include "address.hh"
#include "file_descriptor.hh"

#include<sys/socket.h>


class Socket: public FileDescriptor{



};


class DatagrameSocket: public Socket{



};

class UDPSocket : public DatagrameSocket {


};

class TCPSocket : public Socket{


};

class PacketSocket : public DatagrameSocket{


};

class LocalStreamSocket : public Socket{



};

class LocalDatagramSocket :public DatagrameSocket {


};

