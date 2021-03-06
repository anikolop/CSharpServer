#include "stdafx.h"

#include "TcpClient.h"

namespace CSharpServer {

    void TcpClientEx::onConnected()
    {
        root->InternalOnConnected();
    }

    void TcpClientEx::onDisconnected()
    {
        root->InternalOnDisconnected();
    }

    void TcpClientEx::onReceived(const void* buffer, size_t size)
    {
        array<Byte>^ bytes = gcnew array<Byte>((int)size);
        pin_ptr<Byte> ptr = &bytes[bytes->GetLowerBound(0)];
        memcpy(ptr, buffer, size);
        root->InternalOnReceived(bytes, size);
    }

    void TcpClientEx::onSent(size_t sent, size_t pending)
    {
        root->InternalOnSent(sent, pending);
    }

    void TcpClientEx::onEmpty()
    {
        root->InternalOnEmpty();
    }

    void TcpClientEx::onError(int error, const std::string& category, const std::string& message)
    {
        String^ cat = marshal_as<String^>(category);
        String^ msg = marshal_as<String^>(message);
        root->InternalOnError(errno, cat, msg);
    }

    TcpClient::TcpClient(CSharpServer::Service^ service, String^ address, int port) :
        _service(service),
        _client(new std::shared_ptr<TcpClientEx>(std::make_shared<TcpClientEx>(service->_service.Value, marshal_as<std::string>(address), port)))
    {
        _client->get()->root = this;
    }

    TcpClient::TcpClient(CSharpServer::Service^ service, TcpEndpoint^ endpoint) :
        _service(service),
        _client(new std::shared_ptr<TcpClientEx>(std::make_shared<TcpClientEx>(service->_service.Value, endpoint->_endpoint.Value)))
    {
        _client->get()->root = this;
    }

}
