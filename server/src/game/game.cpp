
#include "utils.hpp"
#include "json.hpp"
#include "network/server.hpp"
#include "network/msg.hpp"
#include "game/game.hpp"
#include "game/party.hpp"
#include "game/rules.hpp"

#include <unistd.h>
#include <iostream>

using json = nlohmann::json;

namespace Game {
    Game::Game(Network::Server *server) : server(server) {
        party = new Party(Rules::player_max());
        party->set_state(State_Choice);
    }

    Game::~Game() {
        delete party;
    }

    /*

    */
    void Game::start() {
        int new_id;
        bool run = true;

        std::vector<json *> datas_r, datas_s;
        datas_r.clear();
        datas_s.clear();

        while(run && party->get_state() != State_Stop) {
            server->resetFD();

            /*
                Vérifie les nouveaux clients, les ajoute si il y a de la place,
                et uniquement lors de la sélection des couleurs
            */
            if(party->get_state() == State_Choice) {
                if(server->isNewCo()) {
                    if((new_id = party->findPlace()) >= 0) {
                        int r = server->acceptClient(new_id);
                        party->add_player(r);
                        log_infos("Nouveau joueur %d !", new_id);
                    }
                    //Plus de place pour le client
                }
            }

            /*
                Recoit tous les messages, les stock dans un vector
                Si il y a une déco ou erreur interne, on crée un message
                que l'on ajoute dans le vector
            */
            if(server->recvAll(&datas_r) == ERROR) {
                log_err("Erreur lors de la réception des messages");
                run = false;
                break;
            }

            /*
                Traite les messages, et les supprime au fur et à mesure
                Crée une liste de message à envoyer
            */
            for(auto msg=datas_r.begin(); msg!=datas_r.end(); msg++) {
                if(dispatch_msg(&datas_s, *msg) == ERROR) {
                    log_err("Erreur lors du traitement des messages");
                    run = false;
                    break;
                }
                delete *msg;
            }
            datas_r.clear();

            /*
                Envoi les messages
            */
            if(server->sendAll(&datas_s) == ERROR) {
                log_err("Erreur lors de l'envoi des messages");
                run = false;
                break;
            }
            datas_s.clear();
        }
    }

    int Game::dispatch_msg(std::vector<nlohmann::json*> *datas, json *data) {
        //Infos communes à tout message
        Msg::Type type = (*data).at("type");    //Type du message
        int id = (*data).at("id");              //Id client concerné
        int r = SUCCESS;

        switch(type) {
            //Un client s'est déconnecté
            case Msg::Msg_disconnect:
                //On le déconnecte
                server->closeClient(id);
                party->remove_player(id);
                log_infos("Déconnexion du client %d", id);

                //Si on est en pleine partie en arrete le jeu
                if(party->get_state() == State_Preparation ||
                    party->get_state() == State_Party) {
                    party->set_state(State_Stop);
                    datas->push_back(Msg::stopServer(id));
                    log_info("Arret de la partie...");
                }
            break;

            //Un joueur à demandé à lancer une partie
            case Msg::Msg_play:
                if(party->get_state() == State_Choice) {
                    if(party->enoughPlayers()) { //On a assez de joueurs 2+
                        log_infos("Le joueur %d lance la partie !", id);
                        datas->push_back(Msg::prepParty(id));
                        party->start(datas);
                    }else {
                        datas->push_back(Msg::invalid(id,
                            std::string("Il faut au moins 2 joueurs pour lancer"
                                " une partie !")));
                    }
                }
            break;

            default:
                r = party->treatMsg(datas, data);
            break;
        }

        return r;
    }

}

