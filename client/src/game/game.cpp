
#include "utils.hpp"
#include "gui/gui.hpp"
#include "gui/template.hpp"
#include "network/client.hpp"
#include "network/msg.hpp"
#include "game/game.hpp"
#include "game/rules.hpp"
#include "json.hpp"

#include <unistd.h>

using json = nlohmann::json;


namespace Game {
    Game::Game(Gui::Screen *scr, Network::Client *client)
    : current_state(State_Menu), party(nullptr), scr(scr), client(client)
    {}

    void Game::start() {
        while(current_state != State_Stop) {
            //Clean screen before new template
            scr->remove_all_win();
            scr->add_win(0, new Gui::Win_Background());

            //Switch state = change screen
            switch(current_state) {
                case State_Menu:
                    menu();
                break;

                case State_Choice:
                    choice();
                break;

                case State_Preparation:
                    prep_party();
                break;
                case State_Party:
                    party->run();
                    if(party->get_state() == State_Stop)
                        current_state = State_Stop;
                break;

                case State_Stop:
                default:
                    //Fin du jeu
                break;
            }
        }
    }

    void Game::menu() {
        scr->add_win(1, new Gui::Win_Title());
        scr->add_win(2, new Gui::Win_MenuTitle());
        scr->draw();
        scr->refresh();


        Gui::Win_MenuTitle *menu = (Gui::Win_MenuTitle *)scr->get_win(2);
        int c;
        bool run_menu = true;
        while(run_menu) {
            c = getch();
            //On valide le choix
            switch(c) {

                case 10:
                    switch(menu->get_entry()) {
                        case 0:
                            current_state = State_Choice;
                            run_menu = false;
                        break;

                        case 1:
                            current_state = State_Stop;
                            run_menu = false;
                        break;

                        default: break;
                    }
                break;

                case KEY_UP:case KEY_DOWN:
                case KEY_LEFT:case KEY_RIGHT:
                    menu->select_entry(c);
                    scr->draw();
                    scr->refresh();
                break;

                default: break;
            }

            //Wait some microseconds
            usleep(1000);
        }
    }

    void Game::choice() {
        scr->add_win(1, new Gui::Win_TitleColor());
        scr->add_win(2, new Gui::Win_MenuColor());
        scr->add_win(3, new Gui::Win_Console());
        scr->add_win(4, new Gui::Win_PlayButt());
        scr->draw();
        scr->refresh();

        if(client->connect() == ERROR) {
            current_state = State_Stop;
            return ;
        }

        Gui::Win_MenuTitle *menu = (Gui::Win_MenuTitle *)scr->get_win(2);
        Gui::Win_Console *console = (Gui::Win_Console *)scr->get_win(3);
        Gui::Win_PlayButt*butt = (Gui::Win_PlayButt *)scr->get_win(4);
        int c;
        bool run = true, color_choosen = false;

        json *data = nullptr, *data_s = nullptr;

        while(run && current_state == State_Choice) {
            client->resetFD();

        //Quelque chose à lire
            if(client->isNewMsg()) {
                if(client->recv(&data) == ERROR) {
                    log_err("Erreur lors de la réception des messages");
                    run = false;
                    current_state = State_Stop;
                    break;
                }
                //Infos communes à tout message
                Msg::Type type = (*data).at("type");    //Type du message
                delete data;
                switch(type) {
                    case Msg::Msg_play:
                        current_state = State_Preparation;
                    break;
                    case Msg::Msg_stopServer:
                        current_state = State_Stop;
                    break;

                    default: break; //Should never happen
                }
                if(current_state != State_Choice) break;
            }

        //Récupère les entrées claviers
            c = getch();
            switch(c) {
                case 'q':
                    current_state = State_Stop;
                break;

                case 10:{
                    if(!color_choosen) data_s = Msg::color(menu->get_entry());
                    else data_s = Msg::startParty();
                    if(client->send(data_s) == ERROR) {
                        run = false; break;
                    }
                    delete data_s;
                    if(client->recv(&data) == ERROR) {
                        run = false; break;
                    }

                    Msg::Type type = (*data).at("type"); //Type du message
                    console->add_entry((*data).at("msg"));
                    scr->draw();
                    delete data;
                    if(type == Msg::Msg_valid) {
                        if(!color_choosen) {
                            color_choosen = true;
                            butt->activate();
                            scr->draw();
                        }
                    }
                    else if(type == Msg::Msg_play) {
                        current_state = State_Preparation;
                    }
                }break;

                case KEY_UP:case KEY_DOWN:
                case KEY_LEFT:case KEY_RIGHT:
                    if(!color_choosen) {
                        menu->select_entry(c);
                        scr->draw(); scr->refresh();
                    }
                break;

                default: break;
            }

            //Wait some microseconds
            usleep(1000);
        }
    }

    void Game::prep_party() {
        party = new Party(scr, client);

        bool run = true, dataParty_recv = false, dataHand_recv = false;
        json *data = nullptr, *data_s = nullptr;
        Msg::Type type;

        while(run && current_state == State_Preparation) {
            client->resetFD();

            if(client->isNewMsg()) {
                if(client->recv(&data) == ERROR) {
                    log_err("Erreur lors de la réception des messages");
                    current_state = State_Stop;
                    run = false;
                    break;
                }
                type = (*data).at("type");

                switch(type) {
                    case Msg::Msg_dataHand:
                        party->get_data()->init_hand(data);
                        dataHand_recv = true;
                    break;

                    case Msg::Msg_dataParty:
                        party->get_data()->init_party(data);
                        dataParty_recv = true;
                    break;

                    case Msg::Msg_startParty:
                        current_state = State_Party;
                    break;

                    default: break;
                }
                delete data;
                if(dataHand_recv && dataParty_recv &&
                    current_state == State_Preparation) {
                    data_s = Msg::ready();
                    if(client->send(data_s) == ERROR) {
                        current_state = State_Stop;
                        return ;
                    }
                    delete data_s;
                }
            }

            //Wait some microseconds
            usleep(1000);
        }
    }

}

