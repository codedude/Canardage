
#include "game/party.hpp"
#include "game/player.hpp"
#include "game/board.hpp"
#include "game/pack.hpp"
#include "game/rules.hpp"
#include "network/msg.hpp"
#include "json.hpp"
#include <vector>

using namespace std;
using json = nlohmann::json;

namespace Game {
    Party::Party(int nb_players) {
        players.resize(nb_players, nullptr);
        board = nullptr;
        pack_action = nullptr;
        turns = 1;
        player_turn = -1;
    }

    Party::~Party() {
        for(auto player=players.begin(); player!=players.end(); player++) {
            if((*player) == nullptr) continue;
            delete (*player);
        }
        delete pack_action;
        delete board;
    }

    int Party::get_winner() {
        for(auto player=players.begin(); player!=players.end(); player++) {
            if((*player) == nullptr) continue;
            if((*player)->dead_ducks() < 5)
                return (*player)->get_id();
        }
        return -1;
    }
    bool Party::is_end() {
        int nb = 0;
        for(auto player=players.begin(); player!=players.end(); player++) {
            if((*player) == nullptr) continue;
            if((*player)->dead_ducks() < 5)
                nb++;
        }
        return !(nb > 1);
    }
    int Party::nextPlayer() {
        int i = player_turn+1;
        while(1) {
            if(i == (int)players.size())
                i = 0;
            if(players[i] == nullptr) {
                i++;
                continue;
            }
            break;
        }
        player_turn = i;
        return i;
    }
    int Party::findPlace() {
        for(unsigned int i=0; i<players.size(); i++) {
            if(players[i] == nullptr)
                return i;
        }
        return -1;
    }
    bool Party::enoughPlayers() {
        int r = 0;
        for(unsigned int i=0; i<players.size(); i++) {
            if(players[i] == nullptr) continue;
            if(players[i]->get_color() != None)
                r += 1;
        }
        return (r >= Rules::player_min());
    }
    bool Party::readyPlayers() {
        for(unsigned int i=0; i<players.size(); i++) {
            if(players[i] == nullptr) continue;
                if(!players[i]->is_ready()) return false;
        }
        return true;
    }
    int Party::isColorFree(Color c) {
        Color cc = (Color)(c+10);
        for(auto player=players.begin(); player!=players.end(); player++) {
            if(*player == nullptr) continue;
            if((*player)->get_color() == cc)
                return false;
        }
        return true;
    }
    void Party::init_choice() {
        choice.id = -1;
        choice.pos1 = -1;
        choice.pos2 = -1;
    }

    int Party::treatMsg(std::vector<nlohmann::json*> *datas, json *data) {
        //Infos communes à tout message
        Msg::Type type = (*data).at("type");    //Type du message
        int id = (*data).at("id");              //Id client concerné
        //On ignore les messages (hors déco), des autres joueurs
        //if(id != player_turn) return SUCCESS;
        int pos_need = 0;
        bool make_action = false;

        switch(type) {
            //Un joueur a choisit une couleur
            case Msg::Msg_choice:
                if(state == State_Choice) {
                    Color new_c = (*data).at("color");
                    if(isColorFree(new_c)) {
                        log_infos("Le joueur %d prend la couleur %d",
                            id, new_c);
                        players[id]->set_color(new_c);
                            datas->push_back(Msg::valid(id,
                        std::string("Vous avez choisi votre couleur")));
                    }else {
                        log_infos("Joueur %d : couleur %d deja prise",
                            id, new_c);
                        datas->push_back(Msg::invalid(id,
                            std::string("Cette couleur est deja prise !")));
                    }
                }
            break;

            case Msg::Msg_ready:
                if(state == State_Preparation) {
                    players[id]->set_ready(true);
                    log_infos("Le joueur %d est pret", id);
                    if(readyPlayers()) {
                        log_info("La partie commence !");
                        state = State_Party;
                        nextPlayer();
                        datas->push_back(Msg::startParty(player_turn));
                        datas->push_back(Msg::turn(player_turn));
                    }
                }
            break;

            /*  Le joueur a choisit une carte
                On récupère le nombre d'emplacements à choisir pour jouer cette
                carte
            */
            case Msg::Msg_turn:
                if(id != player_turn) break;
                choice.id = (*data).at("value");
                pos_need = Rules::need_pos(
                    players[player_turn]->get_card(choice.id));
                if(pos_need > 0)
                    datas->push_back(Msg::slot(id, pos_need));
                else
                    make_action = true;
            break;

            case Msg::Msg_change:
                if(id != player_turn) break;
                choice.id = (*data).at("value");
                choice.pos1 = -2; //Special mark
                make_action = true;
            break;

            case Msg::Msg_resetTurn:
                init_choice();
                break;

            case Msg::Msg_slot:
                choice.pos1 = (*data).at("pos1");
                choice.pos2 = (*data).at("pos2");
                make_action = true;
            break;


            default: break; //Should NEVER happen
        }

        if(make_action) {
            int r;
            if((r = Rules::make_action(player_turn, choice,
                players, *board, *pack_action)) != -1) {
                turns++;
                if(turns == 55) {//Mélange le paquet d'actions quand on la vidé
                    pack_action->shuffle();
                    turns = 1;
                }
                datas->push_back(Msg::endTurn(
                    players[player_turn]->get_json()));

                int new_p = nextPlayer();
                Rules::check_cover(new_p, *board);
                datas->push_back(Msg::nextTurn(player_turn, r, get_json()));

                //On vérifie si c'est là fin de la partie
                if(is_end()) {
                    log_infos("Fin de la partie, le joueur %d a gagné !",
                        get_winner());
                    datas->push_back(Msg::endParty(get_winner()));
                    state = State_Stop;
                    return SUCCESS;
                }
                datas->push_back(Msg::turn(new_p));
                //Envoi nextTurn à tout le monde et endTurn au joueur
                //Puis envoi turn au nouveau joueur
            }else {
                datas->push_back(Msg::invalid(id, "Action impossible"));
            }
            init_choice();
        }

        return SUCCESS;
    }

    /*
        Init le plateau, les cartes, les mains etc...
        Renvoi toutes les données dans un gros json
    */
    void Party::start(std::vector<nlohmann::json*> *datas) {
        //Changement d'état, la partie commence
        state = State_Preparation;

        //Initialise les structures de la partie
        Pack_Duck *pack_duck = new Pack_Duck();
        pack_action = new Pack_Action();

    //On remplit les paquets, mélange, distribue, init les canards etc...

        //On récupère la liste des couleurs utilisées
        vector<Color> colors;
        for(auto player=players.begin(); player!=players.end(); player++) {
            if((*player) == nullptr) continue;
            colors.push_back((*player)->get_color());
        }
        //On init les 2 paquets de cartes
        Rules::init_cards(pack_duck, pack_action, colors);

        //On distribue 4 cartes actions aux joueurs
        for(int n=0; n<Rules::player_hand(); n++) {
            for(auto player=players.begin(); player!=players.end(); player++) {
                if((*player) == nullptr) continue;
                (*player)->draw(*pack_action);
            }
        }

        //On créé et remplit le plateau
        board = new Board(Rules::nb_board_slots(), pack_duck);
        board->fill();

    //On est bon
        //Envoi les infos à TOUT le monde
        json *tmp = get_json();
        datas->push_back(tmp);
        //Envoi à chaque joueur leur main
        for(auto player=players.begin(); player!=players.end(); player++) {
            if((*player) == nullptr) continue;
            datas->push_back((*player)->get_json());
        }
    }

//Infos générales et communes de la partie
    json * Party::get_json() {
        json *data = new json();

        (*data)["id"] = -1;
        (*data)["msg"] = std::string("Donnees de la partie");
        (*data)["type"] = Msg::Msg_dataParty;

        //Nombre de cartes actions restantes dans la pile
        (*data)["actions"] = pack_action->size();
        (*data)["turns"] = turns;

        //Infos sur les joueurs
        (*data)["players"] = {};
        for(auto player=players.begin(); player!=players.end(); player++) {
            if((*player) == nullptr) continue;
            //Id, couleurs, canards restants
            (*data)["players"][(*player)->get_id()] = {};

            (*data)["players"][(*player)->get_id()]["color"] =
                (*player)->get_color();
            (*data)["players"][(*player)->get_id()]["grave"] =
                (*player)->dead_ducks();
            (*data)["players"][(*player)->get_id()]["id"] =
                (*player)->get_id();
        }

        //Infos sur le plateau
        board->get_json(data);

        return data;
    }

}

