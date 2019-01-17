
#ifndef PARTY__H
    #define PARTY__H

    #include "game/rules.hpp"
    #include "game/player.hpp"
    #include "game/pack.hpp"
    #include "game/board.hpp"
    #include "json.hpp"
    #include <vector>

namespace Game {

    typedef enum {
        State_Default, State_Menu,
        State_Choice, State_Preparation, State_Party,
        State_Stop,
    }State;

    class Party {
    private:
        State state;            //Current state of the game
        std::vector<Player*> players;
        Board *board;
        Pack_Action *pack_action;
        int turns;
        int player_turn;
        Choice choice;

    public:
        Party(int nb_players);
        virtual ~Party();

        State   get_state() const       { return state; }
        void    set_state(State pState) { state = pState; }

        int treatMsg(std::vector<nlohmann::json*> *datas,
            nlohmann::json *data);

        bool is_end();
        int get_winner();
        int nextPlayer();
        int findPlace();
        bool enoughPlayers();
        int isColorFree(Color c);
        bool readyPlayers();
        void init_choice();

        void add_player(int id) {
            players[id] = new Human(id);
        }
        void remove_player(int id) {
            delete players[id];
            players[id] = nullptr;
        }

        void start(std::vector<nlohmann::json*> *datas);
        nlohmann::json *get_json();

    };
}
#endif

