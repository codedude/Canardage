
#include "game/data.hpp"
#include "json.hpp"
#include "utils.hpp"
#include "ncurses.h"
#include <sstream>

using json = nlohmann::json;

namespace Game {
    void Data::init_party(json *data) {
        card_action = (*data)["actions"];
        card_duck = (*data)["ducks"];
        turns = (*data)["turns"];

        int num;
        p_color.resize(6, -1);
        p_grave.resize(6, -1);
        board.resize(6);

        for(unsigned int i=0; i<(*data)["players"].size(); i++) {
            num = (*data)["players"][i]["id"];
            p_color[num] = (*data)["players"][i].at("color");
            p_grave[num] = (*data)["players"][i].at("grave");
        }

        for(unsigned int i=0; i<(*data)["board"].size(); i++) {
            num = (*data)["board"][i]["id"];
            board[num] = std::map<std::string, int>();

            for(json::iterator it=(*data)["board"][i].begin();
                it!=(*data)["board"][i].end(); it++) {
                board[num][it.key()] = it.value();
            }
        }
    }

    void Data::set_party(json *data) {
        card_action = (*data)["actions"];
        card_duck = (*data)["ducks"];
        turns = (*data)["turns"];

        int num;
        for(unsigned int i=0; i<(*data)["players"].size(); i++) {
            num = (*data)["players"][i]["id"];
            p_color[num] = (*data)["players"][i].at("color");
            p_grave[num] = (*data)["players"][i].at("grave");
        }

        for(unsigned int i=0; i<(*data)["board"].size(); i++) {
            num = (*data)["board"][i]["id"];
            for(json::iterator it=(*data)["board"][i].begin();
                it!=(*data)["board"][i].end(); it++) {
                board[num][it.key()] = it.value();
            }
        }
    }

    void Data::init_hand(json *data) {
        id = (*data)["id"];
        color = (*data)["color"];
        grave = (*data)["grave"];
        hand.resize(4);
        for(unsigned int i=0; i<(*data)["hand"].size(); i++) {
            hand[i] = (*data)["hand"][i];
        }
    }

    void Data::set_hand(json *data) {
        grave = (*data)["grave"];
        for(unsigned int i=0; i<(*data)["hand"].size(); i++) {
            hand[i] = (*data)["hand"][i];
        }
    }
}

