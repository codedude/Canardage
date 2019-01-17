
#ifndef GAME__H
    #define GAME__H

    #include "network/client.hpp"
    #include "json.hpp"
    #include "gui/gui.hpp"
    #include "game/party.hpp"
    #include "game/rules.hpp"

namespace Game {

    class Game {
    private:
        State current_state;   //Current state of the game
        Party *party;

    public:
        Gui::Screen *scr;           //Ncurses screen management
        Network::Client *client;    //TCP client/server management

        Game(Gui::Screen *scr, Network::Client *client);
        virtual ~Game() {
            delete party;
            delete client;
        }

        //Pseudo state pattern
        void start();

        void menu();
        void choice();
        void prep_party();
    };

}
#endif

