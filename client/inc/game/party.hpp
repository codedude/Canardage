
#ifndef PARTY__H
    #define PARTY__H

    #include "network/client.hpp"
    #include "gui/gui.hpp"
    #include "gui/template.hpp"
    #include "json.hpp"
    #include "game/rules.hpp"
    #include "game/data.hpp"

namespace Game {

    class Party {
    private:
        State current_state;   //Current state of the game
        Data *data;
        bool my_turn;

    public:
        Gui::Screen *scr;           //Ncurses screen management
        Network::Client *client;    //TCP client/server management

        Party(Gui::Screen *scr, Network::Client *client);
        ~Party();

        State get_state() const { return current_state; }

        Data *get_data() const { return data; }
        int treatMsg(nlohmann::json *data, int *nb_pos, std::vector<int>& pos,
            Gui::Win_Hand *hand, Gui::Win_Board *board, Gui::Win_Console *console);
        void run();

    };
}
#endif