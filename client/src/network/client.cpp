
#include "utils.hpp"
#include "network/tcp.hpp"
#include "network/client.hpp"
#include "json.hpp"
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <string>
#include <ncurses.h>

#include <sys/types.h> //htonl, ntohs etc...
#include <sys/socket.h>
#include <netdb.h> //gethostbyname, hostent

using json = nlohmann::json;

namespace Network {
    Client::Client(char *ip, char *port) {
        Addrinfo hints;
        TCP::setAddrinfo(&hints);
        if(TCP::getaddrinfo(ip, port, &hints, &infos) == ERROR)
            throw std::invalid_argument("Failed instantiate client");

        sock = TCP::openSocket();
        if(sock == ERROR)
            throw std::invalid_argument("Failed instantiate socket client");

        TCP::setSoReuseAddr(sock, true);
        if(TCP::setBlockMode(sock, true) == ERROR) {
            TCP::closeSocket(sock);
            throw std::invalid_argument("Impossible de lancer le client en mode"
                " bloquant. Extinction... !\n");
        }
    }

    Client::~Client() {
        freeaddrinfo(infos);
        TCP::closeSocket(sock);
    }

    int Client::resetFD() {
        TCP::initDescriptor(sock, &rdfs, true);
        return TCP::select(sock, &rdfs);
    }

    bool Client::isNewMsg() {
        return TCP::isSmthIn(sock, &rdfs);
    }

    int Client::connect() {
        return TCP::connect(sock, infos);
    }

    int Client::send(json *data) {
        std::string to_send = data->dump();
        int size_data = to_send.length() + 4;
        std::allocator<char> alloc;
        char *raw_data = alloc.allocate(size_data);

        std::memcpy(raw_data, &size_data, 4);
        std::memcpy(raw_data+4, to_send.data(), to_send.length());

        int r = SUCCESS;
        r = TCP::send(sock, raw_data, size_data);

        alloc.deallocate(raw_data, size_data);

        return r;
    }

    int Client::recv(json **data) {
        int n = 0, r = 0;
        char buff[4] = {0};
        while((n = TCP::recv(sock, buff+r, 4-r))){
            r += n;
            if(r == 4 || n == 0)
                break;
        }

        //client disco ?
        if(r == 0) return ERROR;
        int msg_size = 0;
        std::memcpy(&msg_size, buff, 4);
        //On enlève les 4o
        msg_size -= 4;
        std::allocator<char> alloc;

        char *msg = alloc.allocate(msg_size+1);
        Utils::memset_s(msg, 0, msg_size+1);
        r = 0;
        while((n = TCP::recv(sock, msg+r, msg_size-r))){
            r += n;
            if(r == msg_size || n == 0)
                break;
        }
        if(r == 0) {
            alloc.deallocate(msg, msg_size+1);
            return ERROR;
        }

        //mvwprintw(stdscr, 38, 0, "Recu %d : %s", msg_size, msg);

        try {
            *data = new json();
            **data = json::parse(msg);
        }catch(const std::invalid_argument& e) {
            log_errs("json recv : %s", e.what());
            alloc.deallocate(msg, msg_size+1);
            return ERROR;
        }

        alloc.deallocate(msg, msg_size+1);

        return r;
    }

}