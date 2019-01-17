

#ifndef RULES__H
    #define RULES__H

    #include <string>

    #define ID_DUCK 0
    #define ID_ACTION 7
    #define NB_CARDS 25

namespace Game {
    typedef enum {
        Red=10, Green=11, Blue=12, Orange=13, Yellow=14, Pink=15,
        Water=16, None=17
    }Color;

    typedef enum {
        State_Default, State_Menu,
        State_Choice, State_Preparation, State_Party,
        State_Stop,
    }State;

    class Rules {
    private:
        Rules();

        //ID card => nb of cards, name
        static std::pair<int, std::string> const cards[25];
        static std::string const cards_desc[25];
        //Nb of water cards
        static int const cards_water[];
        //Nb player min, max, nb cards in hand
        static int const info_player[];
        //Nb of slots on boardgame
        static int const board_slots;

    public:
        static int player_min() { return info_player[0]; }
        static int player_max() { return info_player[1]; }
        static int player_hand() { return info_player[2]; }
        static int nb_board_slots() { return board_slots; }
        static std::string card_name(int id) { return cards[id].second; }
        static std::string card_desc(int id) { return cards_desc[id]; }

    };
}

#endif

