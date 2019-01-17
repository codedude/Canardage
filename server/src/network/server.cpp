
#include "utils.hpp"
#include "network/server.hpp"
#include "network/tcp.hpp"
#include "network/msg.hpp"
#include "json.hpp"

#include <cstring>
#include <vector>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <netinet/in.h>

using namespace Network;
using json = nlohmann::json;

Server::Server(char *addr, char *port, int max_slots, char *mode){
	bool amode = (mode[0] == '0')?false:true;
	int aport = atoi(port);

	Sockaddr sockaddr;
	TCP::setSockaddr(&sockaddr, AF_INET, aport, addr, amode);

	sock = TCP::openSocket();
   	if(sock == ERROR)
   		throw std::invalid_argument("Impossible d'ouvrir une connexion "
   			"réseau!\n");

	TCP::setSoReuseAddr(sock, true);
	if(TCP::setBlockMode(sock, true) == ERROR) {
		TCP::closeSocket(sock);
		throw std::invalid_argument("Impossible de lancer le serveur en mode "
		"bloquant. Extinction... !\n");
	}

	if(TCP::bind(sock, &sockaddr) == ERROR)
		throw std::invalid_argument("bind impossible !\n");

	if(TCP::listen(sock, max_slots) == ERROR)
		throw std::invalid_argument("listen impossible !\n");

	slots.resize(max_slots, -1);

	log_infos("Le serveur est en écoute sur %s:%s", addr, port);
}

Server::~Server() {
	for(auto it=slots.begin(); it != slots.end(); it++) {
		if(*it != -1) TCP::closeSocket(*it);
	}
	TCP::closeSocket(sock);
}

bool Server::isNewCo() {
	return TCP::isSmthIn(sock, &rdfs);
}

bool Server::isNewMsg(int id) {
	return TCP::isSmthIn(slots[id], &rdfs);
}

int Server::resetFD() {
	TCP::initDescriptor(sock, &rdfs, true);
    for(auto it = slots.begin(); it != slots.end(); ++it) {
        if(*it > -1)
            TCP::initDescriptor(*it, &rdfs, false);
    }
    Sock max_sock = *std::max_element(slots.begin(), slots.end());
    max_sock = std::max(max_sock, sock);

    return TCP::select(max_sock, &rdfs);
}

int Server::acceptClient(int id) {
	Sock new_client = TCP::accept(sock);
	if(new_client == ERROR) return ERROR;

	slots[id] = new_client;

	return id;
}

void Server::closeClient(int id){
	if(slots[id] != -1){
		TCP::closeSocket(slots[id]);
		slots[id] = -1;
	}
}

int Server::send(int id, json *data) {
    if(id != -1) {
        if(slots[id] == -1) return SUCCESS; //Don't send to free slot
    }

	std::string to_send = data->dump();
    int size_data = to_send.length() + 4;
    std::allocator<char> alloc;
    char *raw_data = alloc.allocate(size_data);

    std::memcpy(raw_data, &size_data, 4);
    std::memcpy(raw_data+4, to_send.data(), to_send.length());

    //log_infos("Envoi %d a %d : %s", size_data, id, to_send.c_str());
    int r = SUCCESS;
    if(id == -1) {
    	for(auto it=slots.begin(); it!=slots.end(); it++) {
            if(*it == -1) continue; //Don't send to free slot
    		r = TCP::send(*it, raw_data, size_data);
    		if(r == ERROR) break;
    	}
    }else {
    	r = TCP::send(slots[id], raw_data, size_data);
    }

    alloc.deallocate(raw_data, size_data);

    return r;
}

int Server::recv(int id, json **data) {
	int n = 0, r = 0;
	char buff[4] = {0};

	while((n = TCP::recv(slots[id], buff+r, 4-r))){
		r += n;
		if(r == 4 || n == 0)
			break;
	}

	//client disco ?
	if(r == 0) return 0;

	int msg_size = 0;
    std::memcpy(&msg_size, buff, 4);
	//On enlève les 4o
	msg_size -= 4;

	std::allocator<char> alloc;
	char *msg = alloc.allocate(msg_size+1);
    Utils::memset_s(msg, 0, msg_size+1);
	r = 0;
	while((n = TCP::recv(slots[id], msg+r, msg_size-r))){
		r += n;
		if(r == msg_size || n == 0)
			break;
	}
    if(r == 0) {
        alloc.deallocate(msg, msg_size+1);
        return 0;
    }

    //log_infos("Recu %d de %d : %s", msg_size, id, msg);

    try {
        *data = new json();
        **data = json::parse(msg);
    }catch(const std::invalid_argument& e) {
        log_errs("json recv : %s", e.what());
        alloc.deallocate(msg, msg_size+1);
        return ERROR;
    }
    //On ajoute l'id du client
    (**data)["id"] = id;

	alloc.deallocate(msg, msg_size+1);
	//0 if client disco, >0 if data recv, error else
	return r;
}

int Server::recvAll(std::vector<json *> *datas) {
	int r;
	json *data = nullptr;
	for(unsigned int i=0; i<slots.size(); i++) {
		if(slots[i] == -1) continue;
		if(isNewMsg(i)) {
			r = recv(i, &data);

			//Error : ???
			if(r < 0) {
				return ERROR;
			}
			//Client deco
			else if(r == 0) {
				data = Msg::disconnect(i);
				datas->push_back(data);
			}
			//else new msg
			else {
				datas->push_back(data);
			}
		}
	}
	return SUCCESS;
}

int Server::sendAll(std::vector<json*> *datas) {
	int r = SUCCESS, dest;
	for(auto it=datas->begin(); it!=datas->end(); it++) {
        dest = (*it)->at("id");
		r = send(dest, (*it));
        delete (*it);
		if(r == ERROR) break;
	}

	return r;

}

