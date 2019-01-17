#ifndef SERVER__H
	#define SERVER__H

	#include "network/tcp.hpp"
	#include <netinet/in.h>
	#include <vector>
	#include "json.hpp"

	namespace Network {

		class Server {
			private:
				Sock sock;
				fd_set rdfs;
				std::vector<Sock> slots;

			public:
				Server(char *addr, char *port, int max_slots, char *mode);
				~Server();

				int resetFD();
				bool isNewCo();
				bool isNewMsg(int id);
				int acceptClient(int id);
				void closeClient(int id);

				int send(int id, nlohmann::json *data);
				int recv(int id, nlohmann::json **data);
				int recvAll(std::vector<nlohmann::json*> *datas);
				int sendAll(std::vector<nlohmann::json*> *datas);
		};
	}
#endif