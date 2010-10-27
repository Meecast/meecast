#ifndef CONNECTION_H
#define CONNECTION_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
////////////////////////////////////////////////////////////////////////////////
namespace Core {
    class Connection{
        std::string *_proxyHost;
        int _proxyPort;
        bool _hasProxy;
        bool _connected;
        long _timeout;
        public:
            Connection(long timeout = 10);
            virtual ~Connection();
            void wakeup();
            bool connected() const;
            void close();
            bool hasProxy() const;
            std::string& proxyHost() const;
            int proxyPort() const;
            long timeout() const;
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // CONNECTION_H
