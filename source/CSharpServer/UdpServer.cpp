#include "stdafx.h"

#include "UdpServer.h"

namespace CSharpServer {

    void UdpServerEx::onStarted()
    {
        root->InternalOnStarted();
    }

    void UdpServerEx::onStopped()
    {
        root->InternalOnStopped();
    }

    void UdpServerEx::onReceived(const asio::ip::udp::endpoint& endpoint, const void* buffer, size_t size)
    {
        array<Byte>^ bytes = gcnew array<Byte>((int)size);
        pin_ptr<Byte> ptr = &bytes[bytes->GetLowerBound(0)];
        memcpy(ptr, buffer, size);
        root->_receive_endpoint->_endpoint.Assign((asio::ip::udp::endpoint*)&endpoint);
        root->InternalOnReceived(root->_receive_endpoint, bytes, size);
        root->_receive_endpoint->_endpoint.Assign(nullptr);
    }

    void UdpServerEx::onSent(const asio::ip::udp::endpoint& endpoint, size_t sent)
    {
        root->_send_endpoint->_endpoint.Assign((asio::ip::udp::endpoint*)&endpoint);
        root->InternalOnSent(root->_send_endpoint, sent);
        root->_send_endpoint->_endpoint.Assign(nullptr);
    }

    void UdpServerEx::onError(int error, const std::string& category, const std::string& message)
    {
        String^ cat = marshal_as<String^>(category);
        String^ msg = marshal_as<String^>(message);
        root->InternalOnError(errno, cat, msg);
    }

    UdpServer::UdpServer(CSharpServer::Service^ service, CSharpServer::InternetProtocol protocol, int port) : UdpServer(service, gcnew UdpEndpoint(protocol, port))
    {
    }

    UdpServer::UdpServer(CSharpServer::Service^ service, String^ address, int port) :
        _service(service),
        _server(new std::shared_ptr<UdpServerEx>(std::make_shared<UdpServerEx>(service->_service.Value, marshal_as<std::string>(address), port))),
        _receive_endpoint(gcnew UdpEndpoint()),
        _send_endpoint(gcnew UdpEndpoint())
    {
        _server->get()->root = this;
    }

    UdpServer::UdpServer(CSharpServer::Service^ service, UdpEndpoint^ endpoint) :
        _service(service),
        _server(new std::shared_ptr<UdpServerEx>(std::make_shared<UdpServerEx>(service->_service.Value, endpoint->_endpoint.Value))),
        _receive_endpoint(gcnew UdpEndpoint()),
        _send_endpoint(gcnew UdpEndpoint())
    {
        _server->get()->root = this;
    }

}
