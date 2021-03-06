#pragma once

#include "Endpoint.h"

#include <server/asio/tcp_client.h>

namespace CSharpServer {

    ref class TcpClient;

    class TcpClientEx : public CppServer::Asio::TCPClient
    {
    public:
        using CppServer::Asio::TCPClient::TCPClient;

        gcroot<TcpClient^> root;

        void onConnected() override;
        void onDisconnected() override;
        void onReceived(const void* buffer, size_t size) override;
        void onSent(size_t sent, size_t pending) override;
        void onEmpty() override;
        void onError(int error, const std::string& category, const std::string& message) override;
    };

    //! TCP client
    /*!
        TCP client is used to read/write data from/into the connected TCP server.
    */
    public ref class TcpClient
    {
    public:
        //! Initialize TCP client with a given service, server IP address and port number
        /*!
            \param service - Service
            \param address - Server IP address
            \param port - Server port number
        */
        TcpClient(Service^ service, String^ address, int port);
        //! Initialize TCP client with a given service and TCP endpoint
        /*!
            \param service - Service
            \param endpoint - Server TCP endpoint
        */
        TcpClient(Service^ service, TcpEndpoint^ endpoint);
        ~TcpClient() { this->!TcpClient(); }

        //! Get the client Id
        property String^ Id { String^ get() { return marshal_as<String^>(_client->get()->id().string()); } }

        //! Get the service
        property Service^ Service { CSharpServer::Service^ get() { return _service; } }

        //! Get the number of bytes pending sent by the client
        property long long BytesPending { long long get() { return (long long)_client->get()->bytes_pending(); } }
        //! Get the number of bytes sent by the client
        property long long BytesSent { long long get() { return (long long)_client->get()->bytes_sent(); } }
        //! Get the number of bytes received by the client
        property long long BytesReceived { long long get() { return (long long)_client->get()->bytes_received(); } }

        //! Get the option: keep alive
        property bool OptionKeepAlive { bool get() { return _client->get()->option_keep_alive(); } }
        //! Get the option: no delay
        property bool OptionNoDelay { bool get() { return _client->get()->option_no_delay(); } }
        //! Get the option: receive buffer size
        property long OptionReceiveBufferSize { long get() { return (long)_client->get()->option_receive_buffer_size(); } }
        //! Get the option: send buffer size
        property long OptionSendBufferSize { long get() { return (long)_client->get()->option_send_buffer_size(); } }

        //! Is the client connected?
        property bool IsConnected { bool get() { return _client->get()->IsConnected(); } }

        //! Connect the client (synchronous)
        /*!
            \return 'true' if the client was successfully connected, 'false' if the client failed to connect
        */
        bool Connect() { return _client->get()->Connect(); }
        //! Disconnect the client (synchronous)
        /*!
            \return 'true' if the client was successfully disconnected, 'false' if the client is already disconnected
        */
        bool Disconnect() { return _client->get()->Disconnect(); }
        //! Reconnect the client (synchronous)
        /*!
            \return 'true' if the client was successfully reconnected, 'false' if the client is already reconnected
        */
        bool Reconnect() { return _client->get()->Reconnect(); }

        //! Connect the client (asynchronous)
        /*!
            \return 'true' if the client was successfully connected, 'false' if the client failed to connect
        */
        bool ConnectAsync() { return _client->get()->ConnectAsync(); }
        //! Disconnect the client (asynchronous)
        /*!
            \return 'true' if the client was successfully disconnected, 'false' if the client is already disconnected
        */
        bool DisconnectAsync() { return _client->get()->DisconnectAsync(); }
        //! Reconnect the client (asynchronous)
        /*!
            \return 'true' if the client was successfully reconnected, 'false' if the client is already reconnected
        */
        bool ReconnectAsync() { return _client->get()->ReconnectAsync(); }

        //! Send data to the server (asynchronous)
        /*!
            \param buffer - Buffer to send
            \return 'true' if the data was successfully sent, 'false' if the client is not connected
        */
        bool SendAsync(array<Byte>^ buffer) { return SendAsync(buffer, 0, buffer->Length); }
        //! Send data to the server (asynchronous)
        /*!
            \param buffer - Buffer to send
            \param offset - Buffer offset
            \param size - Buffer size
            \return 'true' if the data was successfully sent, 'false' if the client is not connected
        */
        bool SendAsync(array<Byte>^ buffer, long long offset, long long size)
        {
            pin_ptr<Byte> ptr = &buffer[buffer->GetLowerBound(0) + (int)offset];
            return _client->get()->SendAsync(ptr, size);
        }
        //! Send text to the server (asynchronous)
        /*!
            \param text - Text string to send
            \return 'true' if the text was successfully sent, 'false' if the client is not connected
        */
        bool SendAsync(String^ text)
        {
            std::string temp = marshal_as<std::string>(text);
            return _client->get()->SendAsync(temp.data(), temp.size());
        }

        //! Setup option: keep alive
        /*!
            This option will setup SO_KEEPALIVE if the OS support this feature.

            \param enable - Enable/disable option
        */
        void SetupKeepAlive(bool enable) { return _client->get()->SetupKeepAlive(enable); }
        //! Setup option: no delay
        /*!
            This option will enable/disable Nagle's algorithm for TCP protocol.

            https://en.wikipedia.org/wiki/Nagle%27s_algorithm

            \param enable - Enable/disable option
        */
        void SetupNoDelay(bool enable) { return _client->get()->SetupNoDelay(enable); }
        //! Setup option: receive buffer size
        /*!
            This option will setup SO_RCVBUF if the OS support this feature.

            \param size - Receive buffer size
        */
        void SetupReceiveBufferSize(long size) { return _client->get()->SetupReceiveBufferSize(size); }
        //! Setup option: send buffer size
        /*!
            This option will setup SO_SNDBUF if the OS support this feature.

            \param size - Send buffer size
        */
        void SetupSendBufferSize(long size) { return _client->get()->SetupSendBufferSize(size); }

    protected:
        //! Handle client connected notification
        virtual void OnConnected() {}
        //! Handle client disconnected notification
        virtual void OnDisconnected() {}

        //! Handle buffer received notification
        /*!
            Notification is called when another chunk of buffer was received from the server.

            \param buffer - Received buffer
            \param size - Received buffer size
        */
        virtual void OnReceived(array<Byte>^ buffer, long long size) {}
        //! Handle buffer sent notification
        /*!
            Notification is called when another chunk of buffer was sent to the server.

            This handler could be used to send another buffer to the server
            for instance when the pending size is zero.

            \param sent - Size of sent buffer
            \param pending - Size of pending buffer
        */
        virtual void OnSent(long long sent, long long pending) {}

        //! Handle empty send buffer notification
        /*!
            Notification is called when the send buffer is empty and ready
            for a new data to send.

            This handler could be used to send another buffer to the server.
        */
        virtual void OnEmpty() {}

        //! Handle error notification
        /*!
            \param error - Error code
            \param category - Error category
            \param message - Error message
        */
        virtual void OnError(int error, String^ category, String^ message) {}

    internal:
        void InternalOnConnected() { OnConnected(); }
        void InternalOnDisconnected() { OnDisconnected(); }
        void InternalOnReceived(array<Byte>^ buffer, long long size) { OnReceived(buffer, size); }
        void InternalOnSent(long long sent, long long pending) { OnSent(sent, pending); }
        void InternalOnEmpty() { OnEmpty(); }
        void InternalOnError(int error, String^ category, String^ message) { OnError(error, category, message); }

    protected:
        !TcpClient() { _client.Release(); }

    private:
        CSharpServer::Service^ _service;
        Embedded<std::shared_ptr<TcpClientEx>> _client;
    };

}
