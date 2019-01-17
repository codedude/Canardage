#ifndef MSG__H
	#define MSG__H

    #include "json.hpp"
    #include <string>

namespace Msg {
    typedef enum {
        Msg_valid, Msg_invalid,         //Generic with custom message
        Msg_connect, Msg_disconnect,    //Client co/disco from server
        Msg_choice, Msg_play, Msg_ready, //Signal from clients
        Msg_dataParty, Msg_dataHand,    //Data from server
        Msg_turn, Msg_slot, Msg_change, Msg_endTurn, Msg_nextTurn,
        Msg_resetTurn, Msg_endParty,
        Msg_startParty, Msg_stopServer, //Signal from server
    }Type;

    nlohmann::json *disconnect(int id);
    nlohmann::json *stopServer(int id);
    nlohmann::json *prepParty(int id);
    nlohmann::json *startParty(int id);
    nlohmann::json *endParty(int id);

    nlohmann::json *turn(int id);
    nlohmann::json *slot(int id, int n);

    nlohmann::json *endTurn(nlohmann::json *d);
    nlohmann::json *nextTurn(int id, int c, nlohmann::json *d);


    nlohmann::json *invalid(int id, std::string s);
    nlohmann::json *valid(int id, std::string s);

}

#endif