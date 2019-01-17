
#ifndef TCP__H
    #define TCP__H

    #include <netdb.h>
    #include <sys/socket.h>

    #define IP_LOCAL "127.0.0.1"

namespace Network {
    typedef int Sock;
    typedef struct sockaddr_in Sockaddr;
    typedef struct addrinfo Addrinfo;

    class TCP {
    public:
        static int setTcpNoDelay(Sock socket, bool val);
        static int setSoReuseAddr(Sock socket, bool val);
        static int setBlockMode(Sock socket, bool val);
        static Sock openSocket(void);
        static int closeSocket(Sock socket);
        static void setSockaddr(Sockaddr *sockaddr, int family, int port,
            char *addr, bool mode);
        static int recv(Sock sock, void *buffer, size_t len);
        static int send(Sock sock, void *buffer, size_t len);
        static int getaddrinfo(char *ip, char *port, Addrinfo *hints,
            Addrinfo **result);
        static void setAddrinfo(Addrinfo *hints);
        static int bind(Sock sock, Sockaddr *sockaddr);
        static int listen(Sock sock, int backlog);
        static void initDescriptor(Sock sock, fd_set *rdfs, bool mode);
        static int select(Sock sock, fd_set *rdfs);
        static bool isSmthIn(Sock sock, fd_set *rdfs);
        static Sock accept(Sock sock);
        static int connect(Sock sock, Addrinfo *infos);

    };
}

#endif

