cmake_minimum_required(VERSION 3.21)
project(CppSockets)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(CppSockets
        main.cpp
        SocketCompat.hpp
        TcpClientSocket.hpp
        TcpServerSocket.hpp
        TcpSocket.hpp
        UdpClientSocket.hpp
        UdpServerSocket.hpp
        UdpSocket.hpp
        )
