
#include "utils.hpp"
#include "gui/gui.hpp"
#include "gui/template.hpp"
#include "network/client.hpp"
#include "network/msg.hpp"
#include "game/party.hpp"
#include "game/data.hpp"
#include "game/rules.hpp"
#include "json.hpp"

#include <unistd.h>
#include <vector>

using json = nlohmann::json;


namespace Game {
    Party::Party(Gui::Screen *scr, Network::Client *client) :
    current_state(State_Party), data(new Data()), my_turn(false),
    scr(scr), client(client)
    {
    }
    Party::~Party() {
        delete data;
    }

    void Party::run() {
        scr->add_win(1, new Gui::Win_Console());
        scr->add_win(2, new Gui::Win_Info_Players(data));
        scr->add_win(3, new Gui::Win_Info_Party(data));
        scr->add_win(4, new Gui::Win_Hand(data));
        scr->add_win(5, new Gui::Win_Board(data));
        scr->add_win(6, new Gui::Win_Popup_Card(data));
        scr->add_win(7, new Gui::Win_Player(data));
        scr->draw();

        Gui::Win_Hand *hand = (Gui::Win_Hand *)scr->get_win(4);
        Gui::Win_Board *board = (Gui::Win_Board *)scr->get_win(5);
        Gui::Win_Popup_Card *popup = (Gui::Win_Popup_Card *)scr->get_win(6);

        Gui::Win_Console *console = (Gui::Win_Console *)scr->get_win(1);


        json *data_r = nullptr, *data_s = nullptr;
        bool run = true;
        int selected_card = -1, c;
        int nb_pos = 0;
        std::vector<int> pos;

        while(run && current_state == State_Party) {
            client->resetFD();

        //Quelque chose à lire
            if(client->isNewMsg()) {
                if(client->recv(&data_r) == ERROR) {
                    run = false;
                    break;
                }

                if(treatMsg(data_r, &nb_pos, pos, hand, board, console) == ERROR) {
                    run = false;
                    break;
                }
                delete data_r;
            }

            c = getch();
            switch(c) {
                case 27: //Echap
                    current_state = State_Stop;
                break;

                case 'q':
                    if(popup->is_select()) { //On ferme le popup
                        popup->unshow();
                        hand->enable_select();
                        scr->draw();
                    }else if(board->is_select()) { //Choix de carte
                        board->disable_select();
                        hand->enable_select();
                        scr->draw();
                        data_s = Msg::reset();
                        if(client->send(data_s) == ERROR) {
                            run = false; break;
                        }
                        delete data_s;
                    }
                break;

                case 10:
                    if(hand->is_select()) {
                        selected_card = hand->get_entry();
                        hand->disable_select();
                        popup->show(selected_card);
                        scr->draw();
                    }
                    else if(popup->is_select()) {
                        popup->unshow();
                        hand->enable_select();
                        scr->draw();
                        switch(popup->get_entry()) {
                            case 0: //Jouer
                            data_s = Msg::turn(selected_card);
                            if(client->send(data_s) == ERROR) {
                                run = false; break;
                            }
                            delete data_s;
                            break;

                            case 1: //Echange
                            data_s = Msg::change(selected_card);
                            if(client->send(data_s) == ERROR) {
                                run = false; break;
                            }
                            delete data_s;
                            break;

                            case 2: //Retour
                            break;
                            default: break;
                        }
                    }
                    else if(board->is_select()) {
                        pos.push_back(board->get_entry());
                        nb_pos--;
                        if(nb_pos == 0) {
                            board->disable_select();
                            hand->enable_select();
                            scr->draw();

                            data_s = Msg::slot(pos);
                            if(client->send(data_s) == ERROR) {
                                run = false; break;
                            }
                            delete data_s;
                        }
                    }
                break;

                case KEY_UP:case KEY_DOWN:
                case KEY_LEFT:case KEY_RIGHT:
                    if(hand->is_select())
                        hand->select_entry(c);
                    else if(popup->is_select())
                        popup->select_entry(c);
                    else if(board->is_select())
                        board->select_entry(c);

                    scr->draw();
                break;

                default: break;
            }

            usleep(1000);
        }

        if(run == false)
            current_state = State_Stop;
    }

    int Party::treatMsg(json *j, int *nb_pos, std::vector<int>& pos,
        Gui::Win_Hand *hand, Gui::Win_Board *board, Gui::Win_Console *console) {
        //Infos communes à tout message
        Msg::Type type = (*j).at("type");    //Type du message
        console->add_entry((*j).at("msg"));
        int r = SUCCESS;
        switch(type) {
            //On active notre tour = sélection et envoi d'une carte
            case Msg::Msg_turn:
                my_turn = true;
            break;

            //On doit choisir x positions
            case Msg::Msg_slot:
                *nb_pos = (*j).at("n");
                hand->disable_select();
                board->enable_select();
            break;

            case Msg::Msg_change:

            break;

            //Fin de notre tour + info nouvelle main
            case Msg::Msg_endTurn:
                my_turn = false;
                pos.clear();
                data->set_hand(j);
                scr->draw();
            break;

            //Fin d'un tour + info party
            case Msg::Msg_nextTurn:
                data->set_party(j);
                console->add_entry("---");
            break;

            case Msg::Msg_endParty:
                scr->draw();
                usleep(5000000);
                current_state = State_Stop;
            break;

            //Une action de choix de carte, d'échange ou de choix d'emplacement
            //n'est pas validé par le serveur -> on reste dans le meme état
            case Msg::Msg_invalid:
                pos.clear();
            break;

            case Msg::Msg_stopServer:
                r = ERROR;
            break;

            default: break;
        }
        scr->draw();

        return r;
    }

}