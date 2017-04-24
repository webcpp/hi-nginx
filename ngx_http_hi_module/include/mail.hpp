#ifndef MAIL_HPP
#define MAIL_HPP

#include <string>
#include <vector>
#include <iostream>
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/SecureSMTPClientSession.h>
#include <Poco/Net/SMTPClientSession.h>
#include <Poco/Net/NetException.h>
#include <Poco/SharedPtr.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/StringPartSource.h>
#include <Poco/Net/FilePartSource.h>
#include <Poco/Net/SecureStreamSocket.h>
#include <Poco/File.h>

namespace hi {

    class mail {
    public:

        mail()
        : host()
        , port(0)
        , from()
        , to()
        , bcc()
        , cc()
        , message()
        , contentType("text/plain;charset=UTF-8")
        , subject()
        , content()
        , user()
        , password()
        , attachment() {
            Poco::Net::initializeSSL();
        }
        mail(const mail& orig) = delete;

        virtual ~mail() {
            Poco::Net::uninitializeSSL();
        }
    public:

        void setHost(const std::string& host) {
            this->host = host;
        }

        void setPort(int port) {
            this->port = port;
        }

        void setFrom(const std::string& from) {
            this->from = from;
        }

        void setTo(const std::string& to) {
            this->to = to;
        }

        void addBCC(const std::string& bcc) {
            this->bcc.push_back(bcc);
        }

        void addCC(const std::string& cc) {
            this->cc.push_back(cc);
        }

        void setContentType(const std::string& type) {
            this->contentType = type;
        }

        void setSubject(const std::string& subject) {
            this->subject = subject;
        }

        void setContent(const std::string& content) {
            this->content = content;
        }

        void setUser(const std::string& user) {
            this->user = user;
        }

        void setPassword(const std::string& password) {
            this->password = password;
        }

        void addAttachment(const std::string& filePath, const std::string& mimeType) {
            this->attachment.push_back(std::pair<std::string, std::string>(filePath, mimeType));
        }
    private:

        void createMessage(Poco::Net::MailMessage& message) {
            message.setSender(this->from);

            message.addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT, this->to, this->to));

            for (auto &item : this->bcc) {
                message.addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::BCC_RECIPIENT, item, item));
            }
            for (auto& item : this->cc) {
                message.addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::CC_RECIPIENT, item, item));
            }
            for (auto& item : this->attachment) {
                if (Poco::File(item.first).exists()) {
                    try {
                        message.addAttachment(item.first, new Poco::Net::FilePartSource(item.first, item.second));
                    } catch (Poco::Exception& e) {
                        this->message.push_back(e.message());
                    }
                } else {
                    this->message.push_back(item.first + " is not exists.");
                }
            }
            message.setSubject(this->subject);
            message.setContentType(this->contentType);
            message.addContent(new Poco::Net::StringPartSource(this->content));
        }
    public:

        void run() {
            if (this->port == 25) {
                this->by25();
            } else if (this->port == 465) {
                this->by465();
            } else if (this->port == 587) {
                this->by587();
            } else {
                this->message.push_back("The port not suppor.Suppor:25|465|587");
            }
        }

        std::vector<std::string> getMessage() {
            return this->message;
        }

    private:

        void by25() {
            try {
                Poco::Net::MailMessage message;
                this->createMessage(message);

                Poco::Net::SMTPClientSession session(this->host, this->port);
                session.open();
                session.login(Poco::Net::SMTPClientSession::AUTH_LOGIN, this->user, this->password);
                session.sendMessage(message);
                session.close();

            } catch (Poco::Net::NetException& exc) {
                this->message.push_back(exc.message());
            }
        }

        void by465() {
            try {
                Poco::Net::MailMessage message;
                this->createMessage(message);

                Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> pCert = new Poco::Net::AcceptCertificateHandler(false);
                Poco::Net::Context::Ptr pContent = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_RELAXED, 9, true);
                Poco::Net::SSLManager::instance().initializeClient(0, pCert, pContent);

                Poco::Net::SocketAddress as(this->host, this->port);
                Poco::Net::SecureStreamSocket socket(as);

                Poco::Net::SecureSMTPClientSession session(socket);
                session.open();
                session.login(Poco::Net::SMTPClientSession::AUTH_LOGIN, this->user, this->password);
                session.sendMessage(message);
                session.close();
            } catch (Poco::Net::NetException& exc) {
                this->message.push_back(exc.message());
            }
        }

        void by587() {
            try {
                Poco::Net::MailMessage message;
                this->createMessage(message);

                Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> pCert = new Poco::Net::AcceptCertificateHandler(false);
                Poco::Net::Context::Ptr pContent = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_RELAXED, 9, true);
                Poco::Net::SSLManager::instance().initializeClient(0, pCert, pContent);
                Poco::Net::SecureSMTPClientSession session(this->host, this->port);
                session.open();
                if (session.startTLS(pContent)) {
                    session.login(Poco::Net::SMTPClientSession::AUTH_LOGIN, this->user, this->password);
                    session.sendMessage(message);
                }
                session.close();

            } catch (Poco::Net::NetException& exc) {
                this->message.push_back(exc.message());
            }
        }
    private:
        std::string host;
        int port;
        std::string from, to;
        std::vector<std::string> bcc, cc, message;
        std::string contentType;
        std::string subject, content;
        std::string user, password;
        std::vector<std::pair<std::string, std::string> > attachment;
    };
}

#endif /* MAIL_HPP */

