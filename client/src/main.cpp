
#include "utils.hpp"

#include "gui/gui.hpp"
#include "network/client.hpp"
#include "game/game.hpp"

#include <csignal>

using namespace std;

Gui::Screen *scr = nullptr;
void handle(int signal)
{
    if(signal != SIGWINCH)
        return ;
    endwin();
    refresh();
    scr->draw(true);
}

int main(int argc, char **argv)
{
    if(argc < 3) {
        log_warn("Usage : ./canardage IP PORT");
        exit(1);
    }

    try {
        scr = new Gui::Screen();
    }
    catch(const std::invalid_argument& e) {
        log_errs("Error Window init : %s", e.what());
    }
    signal(SIGWINCH, handle);

    Network::Client *client = nullptr;
    try {
        client = new Network::Client(argv[1], argv[2]);
    }
    catch(const std::invalid_argument& e) {
        log_errs("Error Client init : %s", e.what());
    }

    Game::Game game(scr, client);

    game.start();

    delete scr;

    return 0;
}

