
#include "network/msg.hpp"
#include "utils.hpp"
#include "json.hpp"
#include <sstream>

using json = nlohmann::json;

namespace Msg {


    json *color(int id) {
        std::ostringstream s;
        s << "choix couleur";

        json *j = new json();
        *j =
        {
            {"msg", s.str()},
            {"type", Msg_choice},
            {"color", id}
        };

        return j;
    }

    json *startParty() {
        json *j = new json();
        *j =
        {
            {"msg", std::string("")},
            {"type", Msg_play},
        };

        return j;
    }
    json *ready() {
        json *j = new json();
        *j =
        {
            {"msg", std::string("")},
            {"type", Msg_ready},
        };

        return j;
    }

    json *turn(int id) {
        std::ostringstream s;
        s << "choix carte";

        json *j = new json();
        *j =
        {
            {"msg", s.str()},
            {"type", Msg_turn},
            {"value", id}
        };

        return j;
    }
    json *change(int id) {
        std::ostringstream s;
        s << "echange carte";

        json *j = new json();
        *j =
        {
            {"msg", s.str()},
            {"type", Msg_change},
            {"value", id}
        };

        return j;
    }
    json *slot(std::vector<int> pos) {
        std::ostringstream s;
        s << "choix emplacement";

        int pos1 = -1, pos2 = -1;
        if((int)pos.size() > 0)
            pos1 = pos[0];
        if((int)pos.size() > 1)
            pos2 = pos[1];

        json *j = new json();
        *j =
        {
            {"msg", s.str()},
            {"type", Msg_slot},
            {"pos1", pos1},
            {"pos2", pos2}
        };

        return j;
    }

    json *reset() {
        std::ostringstream s;
        s << "reset du tour";

        json *j = new json();
        *j =
        {
            {"msg", s.str()},
            {"type", Msg_resetTurn}
        };

        return j;
    }

}