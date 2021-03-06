#include "stdafx.h"

#include "SslContext.h"

namespace CSharpServer {

    void SslContextEx::set_password(const std::string& password)
    {
        _password = password;
        set_password_callback([this](size_t max_length, asio::ssl::context::password_purpose purpose) -> std::string { return _password; });
    }

    SslContext::SslContext(CSharpServer::SslMethod method) :
        _context(new std::shared_ptr<SslContextEx>(std::make_shared<SslContextEx>((asio::ssl::context_base::method)method)))
    {
    }

    void SslContext::SetPassword(String^ password)
    {
        _context->get()->set_password(marshal_as<std::string>(password));
    }

    void SslContext::SetVerifyMode(SslVerifyMode mode)
    {
        switch (mode)
        {
            case SslVerifyMode::VerifyNone:
                _context->get()->set_verify_mode(asio::ssl::verify_none);
                return;
            case SslVerifyMode::VerifyPeer:
                _context->get()->set_verify_mode(asio::ssl::verify_peer);
                return;
            case SslVerifyMode::VerifyFailIfNoPeerCert:
                _context->get()->set_verify_mode(asio::ssl::verify_fail_if_no_peer_cert);
                return;
            case SslVerifyMode::VerifyClientOnce:
                _context->get()->set_verify_mode(asio::ssl::verify_client_once);
                return;
        }
    }

    void SslContext::SetVerifyDepth(int depth)
    {
        _context->get()->set_verify_depth(depth);
    }

    void SslContext::LoadVerifyFile(String^ filename)
    {
        _context->get()->load_verify_file(marshal_as<std::string>(filename));
    }

    void SslContext::UseCertificateChainFile(String^ filename)
    {
        _context->get()->use_certificate_chain_file(marshal_as<std::string>(filename));
    }

    void SslContext::UsePrivateKeyFile(String^ filename, SslFileFormat format)
    {
        switch (format)
        {
            case SslFileFormat::ASN1:
                _context->get()->use_private_key_file(marshal_as<std::string>(filename), asio::ssl::context::asn1);
                return;
            case SslFileFormat::PEM:
                _context->get()->use_private_key_file(marshal_as<std::string>(filename), asio::ssl::context::pem);
                return;
        }
    }

    void SslContext::UseTmpDHFile(String^ filename)
    {
        _context->get()->use_tmp_dh_file(marshal_as<std::string>(filename));
    }

}
