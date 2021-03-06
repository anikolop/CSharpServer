#pragma once

#include "Service.h"

namespace CSharpServer {

    //! TCP Endpoint
    public ref class TcpEndpoint
    {
    internal:
        //! Initialize empty TCP endpoint
        TcpEndpoint() {}

    public:
        //! Initialize TCP endpoint with a given protocol and port number
        /*!
            \param protocol - Protocol type
            \param port - Port number
        */
        TcpEndpoint(CSharpServer::InternetProtocol protocol, int port);
        //! Initialize TCP endpoint with a given IP address and port number
        /*!
            \param address - IP address
            \param port - Port number
        */
        TcpEndpoint(String^ address, int port);
        //! Initialize TCP endpoint with another endpoint instance
        /*!
            \param endpoint - Endpoint
        */
        TcpEndpoint(TcpEndpoint^ endpoint);
        ~TcpEndpoint() { this->!TcpEndpoint(); }

    protected:
        !TcpEndpoint() { _endpoint.Release(); };

    internal:
        Embedded<asio::ip::tcp::endpoint> _endpoint;
    };

    //! UDP Endpoint
    public ref class UdpEndpoint
    {
    internal:
        //! Initialize empty UDP endpoint
        UdpEndpoint() {}

    public:
        //! Initialize UDP endpoint with a given protocol and port number
        /*!
            \param protocol - Protocol type
            \param port - Port number
        */
        UdpEndpoint(CSharpServer::InternetProtocol protocol, int port);
        //! Initialize UDP endpoint with a given IP address and port number
        /*!
            \param address - IP address
            \param port - Port number
        */
        UdpEndpoint(String^ address, int port);
        //! Initialize UDP endpoint with another endpoint instance
        /*!
            \param endpoint - Endpoint
        */
        UdpEndpoint(UdpEndpoint^ endpoint);
        ~UdpEndpoint() { this->!UdpEndpoint(); }

    protected:
        !UdpEndpoint() { _endpoint.Release(); };

    internal:
        Embedded<asio::ip::udp::endpoint> _endpoint;
    };

}
