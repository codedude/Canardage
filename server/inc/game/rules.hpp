

#ifndef RULES__H
    #define RULES__H

    #include "game/pack.hpp"
    #include "game/player.hpp"
    #include "game/board.hpp"
    #include <string>

    #define ID_DUCK 0
    #define ID_ACTION 7
    #define NB_CARDS 25

    #define _CARD(N) static bool card_##N (int id, Choice& choice,\
            std::vector<Player*>& players, Board& board,\
            Pack_Action& pack_action);
    #define CARD(N) bool Rules::card_##N (int id, Choice& choice,\
            std::vector<Player*>& players, Board& board,\
            Pack_Action& pack_action)

namespace Game {

    class Rules {
    private:
        Rules();

        //ID card => nb of cards, name
        static std::pair<int, std::string> const cards[25];
        static int const cards_pos[18];
        //Nb of water cards
        static int const cards_water[];
        //Nb player min, max, nb cards in hand
        static int const info_player[];
        //Nb of slots on boardgame
        static int const board_slots;

        _CARD(1)_CARD(2)_CARD(3)_CARD(4)_CARD(5)_CARD(6)
        _CARD(7)_CARD(8)_CARD(9)_CARD(10)_CARD(11)_CARD(12)
        _CARD(13)_CARD(14)_CARD(15)_CARD(16)_CARD(17)_CARD(18)

        static bool(* card_funs[18])(int id, Choice& choice,
            std::vector<Player*>& players, Board& board,
            Pack_Action& pack_action);

    public:
        static void init_cards(Pack_Duck *ducks, Pack_Action *actions,
            std::vector<Color> colors);

        static int player_min() { return info_player[0]; }
        static int player_max() { return info_player[1]; }
        static int player_hand() { return info_player[2]; }
        static int nb_board_slots() { return board_slots; }
        static std::string card_name(int id) {
            if(id == -2) return "Echange";
            else return cards[id].second;
        }
        static int need_pos(int id);
        static int id_color(int c, std::vector<Player*>& players);
        static int make_action(int id, Choice& choice,
            std::vector<Player*>& players, Board& board,
            Pack_Action& pack_action);
        static void check_cover(int id_p, Board& board);
    };
}

#endif

