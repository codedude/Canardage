
#ifndef DATA__H
    #define DATA__H

    #include "json.hpp"
    #include "game/rules.hpp"
    #include <vector>
    #include <string>

namespace Game {

    class Data {
    public:
        //Données du joueur : main = id de carte DANS L'ORDRE
        int         id;
        Color       color;
        std::vector<int> hand;
        int   grave;

        //Données des autres joueurs : id = id du joueur
        std::vector<int> p_color;
        std::vector<int> p_grave;

        //Données générales : cartes restantes et numéro du tour
        int card_action, card_duck, turns;

        //Plateau
        std::vector<std::map<std::string, int>> board;


        Data() {};
        ~Data() {};

        void init_party(nlohmann::json *data);
        void set_party(nlohmann::json *data);

        void init_hand(nlohmann::json *data);
        void set_hand(nlohmann::json *data);
    };

}
#endif