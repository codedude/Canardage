#ifndef MSG__H
	#define MSG__H

    #include "json.hpp"
    #include <string>
    #include <vector>

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

    nlohmann::json *color(int id);
    nlohmann::json *startParty();
    nlohmann::json *ready();
    nlohmann::json *reset();

    nlohmann::json *turn(int id);
    nlohmann::json *change(int id);
    nlohmann::json *slot(std::vector<int> pos);
}

#endif