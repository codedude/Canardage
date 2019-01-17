
#include "network/msg.hpp"
#include "game/rules.hpp"
#include "utils.hpp"
#include "json.hpp"
#include <sstream>

using json = nlohmann::json;

namespace Msg {

    json *disconnect(int id) {
        std::ostringstream s;
        s << "Le joueur " << id << " s'est deconnecte !";

        json *j = new json();
        *j =
        {
            {"msg", s.str()},
            {"type", Msg_disconnect},
            {"id", id}
        };

        return j;
    }

    json *stopServer(int id) {
        std::ostringstream s;
        s << "Arret du serveur : le client " << id << " a quitte la partie !";

        json *j = new json();
        *j =
        {
            {"msg", s.str()},
            {"type", Msg_stopServer},
            {"id", -1}
        };

        return j;
    }

    json *prepParty(int id) {
        std::ostringstream s;
        s << "Le joueur " << id << " a lance la partie !";

        json *j = new json();
        *j =
        {
            {"msg", s.str()},
            {"type", Msg_play},
            {"id", -1}
        };

        return j;
    }
    json *startParty(int id) {
        std::ostringstream s;
        s << "La partie commence ! Le joueur " << id << " commence.";

        json *j = new json();
        *j =
        {
            {"msg", s.str()},
            {"type", Msg_startParty},
            {"id", -1}
        };

        return j;
    }
    json *endParty(int id) {
        std::ostringstream s;
        s << "Partie terminee ! Le joueur " << id << " a gagne :=)";

        json *j = new json();
        *j =
        {
            {"msg", s.str()},
            {"type", Msg_endParty},
            {"id", -1}
        };

        return j;
    }

    json *turn(int id) {
        json *j = new json();
        *j =
        {
            {"msg", "C'est votre tour !"},
            {"type", Msg_turn},
            {"id", id}
        };

        return j;
    }
    json *slot(int id, int n) {
        std::ostringstream s;
        s << "Choisissez " << n << " emplacements";
        json *j = new json();
        *j =
        {
            {"msg", s.str()},
            {"type", Msg_slot},
            {"id", id},
            {"n", n}
        };

        return j;
    }

    json *endTurn(nlohmann::json *d) {
        (*d)["type"] = Msg_endTurn;
        std::ostringstream s;
        s << "Fin de votre tour, vous recevez une carte.";
        (*d)["msg"] = s.str();
        return d;
    }
    json *nextTurn(int id, int c, nlohmann::json *d) {
        (*d)["type"] = Msg_nextTurn;
        std::ostringstream s;
        s << "Joueur " << id << " joue " << Game::Rules::card_name(c) << ".";
        (*d)["msg"] = s.str();

        return d;
    }

    json *invalid(int id, std::string s) {
        json *j = new json();
        *j =
        {
            {"msg", s.c_str()},
            {"type", Msg_invalid},
            {"id", id}
        };

        return j;
    }
    json *valid(int id, std::string s) {
        json *j = new json();
        *j =
        {
            {"msg", s.c_str()},
            {"type", Msg_valid},
            {"id", id}
        };

        return j;
    }
}