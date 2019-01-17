
#ifndef CLIENT__H
    #define CLIENT__H

    #include "network/tcp.hpp"
    #include "json.hpp"

    #include <netinet/in.h>


namespace Network {

    class Client {
    private:
        Sock sock;
        fd_set rdfs;
        Addrinfo *infos;

    public:
        Client(char *ip, char *port);
        ~Client();

        int connect();
        int resetFD();
        bool isNewMsg();
        int send(nlohmann::json *data);
        int recv(nlohmann::json **data);
    };
}
#endif