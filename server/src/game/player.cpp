
#include "game/player.hpp"
#include "game/pack.hpp"
#include "network/msg.hpp"
#include "json.hpp"


using namespace std;
using json = nlohmann::json;

namespace Game {
    int Player::draw(Pack_Action& pack) {
        Action_ptr c = nullptr;
        if(hand.size() < 4) {
            c = pack.pop();
            if(c != nullptr) {
                hand.push(c);
            }
        }

        return (c == nullptr)?-1:c->get_id();
    }

    json *Player::get_json() {
        json *data = new json();

        (*data)["id"] = id;
        (*data)["color"] = color;
        (*data)["grave"] = (int)grave.size();
        (*data)["msg"] = std::string("Votre main");
        (*data)["type"] = Msg::Msg_dataHand;

        (*data)["hand"] = {};
        std::array<int, 4> infos;
        for(int i=0; i<hand.size(); i++) {
            infos[i] = hand.at(i)->get_id();
        }
        (*data)["hand"] = infos;

        return data;
    }
}