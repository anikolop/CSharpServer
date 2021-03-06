#include "stdafx.h"

#include "Endpoint.h"

namespace CSharpServer {

    TcpEndpoint::TcpEndpoint(CSharpServer::InternetProtocol protocol, int port)
    {
        switch (protocol)
        {
            case InternetProtocol::IPv4:
                _endpoint.Assign(new asio::ip::tcp::endpoint(asio::ip::tcp::v4(), (unsigned short)port));
                break;
            case InternetProtocol::IPv6:
                _endpoint.Assign(new asio::ip::tcp::endpoint(asio::ip::tcp::v6(), (unsigned short)port));
                break;
        }
    }

    TcpEndpoint::TcpEndpoint(String^ address, int port) :
        _endpoint(new asio::ip::tcp::endpoint(asio::ip::address::from_string(marshal_as<std::string>(address)), (unsigned short)port))
    {
    }

    TcpEndpoint::TcpEndpoint(TcpEndpoint^ endpoint) :
        _endpoint(new asio::ip::tcp::endpoint(endpoint->_endpoint.Value))
    {
    }

    UdpEndpoint::UdpEndpoint(CSharpServer::InternetProtocol protocol, int port)
    {
        switch (protocol)
        {
            case InternetProtocol::IPv4:
                _endpoint.Assign(new asio::ip::udp::endpoint(asio::ip::udp::v4(), (unsigned short)port));
                break;
            case InternetProtocol::IPv6:
                _endpoint.Assign(new asio::ip::udp::endpoint(asio::ip::udp::v6(), (unsigned short)port));
                break;
        }
    }

    UdpEndpoint::UdpEndpoint(String^ address, int port) :
        _endpoint(new asio::ip::udp::endpoint(asio::ip::address::from_string(marshal_as<std::string>(address)), (unsigned short)port))
    {
    }

    UdpEndpoint::UdpEndpoint(UdpEndpoint^ endpoint) :
        _endpoint(new asio::ip::udp::endpoint(endpoint->_endpoint.Value))
    {
    }

}
