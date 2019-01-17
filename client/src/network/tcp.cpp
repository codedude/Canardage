
#include "utils.hpp"
#include "network/tcp.hpp"

#include <errno.h>
#include <time.h>
#include <cstring>

#include <sys/socket.h>
#include <netdb.h> //gethostbyname, hostent
#include <sys/types.h> //htonl, ntohs etc...
#include <arpa/inet.h>  //inet_addr
#include <fcntl.h>
#include <netinet/tcp.h> //TCP_NODELAY et SOL_TCP.
#include <unistd.h>


namespace Network {

int TCP::setTcpNoDelay(Sock socket, bool val) {
    int errsv;
    int onOff = (val == true)?1:0;
    if (setsockopt(socket, SOL_TCP, TCP_NODELAY, &onOff, sizeof(onOff))
        == -1)
    {
        errsv = errno;
        log_errs("setsockopt: %d - %s\n", errsv, strerror(errsv));
        return ERROR;
    }
    return SUCCESS;
}
int TCP::setSoReuseAddr(Sock socket, bool val)
{
    int errsv;
    int onOff = (val == true)?1:0;
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &onOff,
        sizeof(onOff)) == -1)
    {
        errsv = errno;
        log_errs("setsockopt: %d - %s\n", errsv, strerror(errsv));
        return ERROR;
    }
    return SUCCESS;
}
int TCP::setBlockMode(Sock socket, bool val)
{
    int flags, r, errsv;
    if ((flags = fcntl(socket, F_GETFL)) == -1)
    {
        errsv = errno;
        log_errs("fnctl1: %d - %s\n", errsv, strerror(errsv));
        return ERROR;
    }

    if (val == true)
        r = fcntl(socket, F_SETFL, flags & ~O_NONBLOCK);
    else
        r = fcntl(socket, F_SETFL, flags | O_NONBLOCK);

    if (r == -1)
    {
        errsv = errno;
        printf("fnctl2: %d - %s\n", errsv, strerror(errsv));
    }
    return (r);
}

Sock TCP::openSocket(void) {
    Sock sock;
    if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
        log_errs("openSocket(): %d - %s\n", errno, strerror(errno));

    return sock;
}
int TCP::closeSocket(Sock socket) {
    if (::close(socket) == -1) {
        log_errs("SocketClose: %d - %s\n", errno, strerror(errno));
        return ERROR;
    }
    return SUCCESS;
}

void TCP::setSockaddr(Sockaddr *sockaddr, int family, int port, char *addr,
    bool mode)
{
    Utils::memset_s(sockaddr, '0', sizeof(Sockaddr));
    sockaddr->sin_family = family;
    sockaddr->sin_port = htons(port);
    if(mode)
        sockaddr->sin_addr.s_addr = htonl(INADDR_ANY);
    else
        sockaddr->sin_addr.s_addr = inet_addr(addr);
}

int TCP::recv(Sock sock, void *buffer, size_t len)
{
    int nbRecv, errsv;
    Utils::memset_s(buffer, 0, len);

    if((nbRecv = ::recv(sock, buffer, len, 0)) == -1)
    {
        errsv = errno;
        //if(errsv != EAGAIN && errsv != EWOULDBLOCK)
        //{
            log_errs("recv: %d - %s\n", errsv, strerror(errsv));
            return ERROR;
        //}
    }

    return nbRecv;
}
int TCP::send(Sock sock, void *buffer, size_t len)
{
    int nbSend, errsv;
    if((nbSend = ::send(sock, buffer, len, 0)) == -1)
    {
        errsv = errno;
        log_errs("send: %d - %s\n", errsv, strerror(errsv));
    }
    return nbSend;
}

int TCP::getaddrinfo(char *ip, char *port, Addrinfo *hints, Addrinfo **result) {
    int s = ::getaddrinfo(ip, port, hints, result);
    if(s != 0) {
        log_errs("getaddrinfo() %d : %s\n", s, gai_strerror(s));
        return ERROR;
    }
    return SUCCESS;
}

void TCP::setAddrinfo(Addrinfo *hints) {
    Utils::memset_s(hints, 0, sizeof(Addrinfo));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;
}

int TCP::bind(Sock sock, Sockaddr *sockaddr) {
    if(::bind(sock, (struct sockaddr *)sockaddr, sizeof(struct sockaddr))
        == -1)
    {
        log_errs("bind() : %d - %s\n", errno, strerror(errno));
        return ERROR;
    }
    return SUCCESS;
}
int TCP::listen(Sock sock, int backlog) {
    if(::listen(sock, backlog) == -1)
    {
        log_errs("listen() : %d - %s\n", errno, strerror(errno));
        return ERROR;
    }
    return SUCCESS;
}

void TCP::initDescriptor(Sock sock, fd_set *rdfs, bool mode) {
    if(mode)
        FD_ZERO(rdfs);
    FD_SET(sock, rdfs);
}

int TCP::select(Sock sock, fd_set *rdfs) {
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1;
    if(::select(sock+1, rdfs, NULL, NULL, &timeout) == -1)
    {
        log_errs("select(): %d - %s\n", errno, strerror(errno));
        return ERROR;
    }
    return SUCCESS;
}

bool TCP::isSmthIn(Sock sock, fd_set *rdfs) {
    return (FD_ISSET(sock, rdfs) > 0);
}

Sock TCP::accept(Sock sock) {
    int errsv;

    Sock sockTmp;
    Sockaddr addrTmp;
    socklen_t nAddrTmp = sizeof(addrTmp);
    Utils::memset_s(&addrTmp, '0', sizeof(addrTmp));

    if((sockTmp = ::accept(sock, (struct sockaddr*)&addrTmp, &nAddrTmp))
        == -1)
    {
        errsv = errno;
        if (errsv != EWOULDBLOCK && errsv != EAGAIN)
        {
            log_errs("accept: %d - %s\n", errsv, strerror(errsv));
            return ERROR;
        }
    }

    setBlockMode(sockTmp, true);
    return sockTmp;
}

int TCP::connect(Sock sock, Addrinfo *infos) {
    if(::connect(sock, infos->ai_addr, infos->ai_addrlen) == -1)
    {
        switch(errno) {
            case ECONNREFUSED:
                log_errs("connect() %d : aucune ecoute sur ce port\n", errno);
                break;
            case EISCONN:
                log_errs("connect() %d : socket deja connecte\n", errno);
                break;
            case ENETUNREACH:
                log_errs("connect() %d : client non connecté à internet\n",
                    errno);
                break;
            case ENOTSOCK:
                log_errs("connect() %d : socket invalide\n", errno);
                break;
            default:
                log_errs("connect: %d : %s\n", errno, strerror(errno));
                break;
        }

        return ERROR;
    }

    return SUCCESS;
}

}

