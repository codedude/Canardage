
#include "game/rules.hpp"
#include "game/pack.hpp"
#include "game/player.hpp"
#include "game/board.hpp"
#include "utils.hpp"

#include <memory>
#include <utility>
#include <stdlib.h>

using namespace std;

namespace Game {
    //ID card => nb of cards, name
    std::pair<int, std::string> const Rules::cards[] =
    {
        std::make_pair(6,   "rouge"),
        std::make_pair(6,   "vert"),
        std::make_pair(6,   "bleu"),
        std::make_pair(6,   "orange"),
        std::make_pair(6,   "jaune"),
        std::make_pair(6,   "rose"),
        std::make_pair(5,   "eau"),
        std::make_pair(13,  "Pan"),
        std::make_pair(11,  "En joue"),
        std::make_pair(6,   "En avant marche"),
        std::make_pair(3,   "Canard presse"),
        std::make_pair(3,   "Canard blase"),
        std::make_pair(2,   "A couvert"),
        std::make_pair(2,   "Rectifier le tir <-"),
        std::make_pair(2,   "Rectifier le tir ->"),
        std::make_pair(2,   "Planque"),
        std::make_pair(1,   "Walking duck"),
        std::make_pair(1,   "2 pour le prix d'1"),
        std::make_pair(1,   "Ducky Luck"),
        std::make_pair(1,   "Danse des canards"),
        std::make_pair(1,   "Oups"),
        std::make_pair(1,   "Canards enchaines"),
        std::make_pair(1,   "Fulguro coin"),
        std::make_pair(1,   "Peace and love"),
        std::make_pair(0,   "Canarchie")
    };

    int const Rules::cards_pos[] = {1,1,0,1,1,1,1,1,2,0,2,1,0,2,2,1,0,0};

    //Nb of water cards
    int const Rules::cards_water[] = {0, 2, 3, 3, 4, 5};
    //Nb player min, max, nb cards in hand
    int const Rules::info_player[] = {2, 6, 4};
    //Nb of slots on boardgame
    int const Rules::board_slots = 6;

    bool(* Rules::card_funs[])(int id, Choice& choice,
            std::vector<Player*>& players, Board& board,
            Pack_Action& pack_action) = {
        &card_1,&card_2,&card_3,&card_4,&card_5,&card_6,
        &card_7,&card_8,&card_9,&card_10,&card_11,&card_12,
        &card_13,&card_14,&card_15,&card_16,&card_17,&card_18
    };

    void Rules::init_cards(Pack_Duck *ducks, Pack_Action *actions,
            std::vector<Color> colors) {

    //Init le paquet de canards
        for(auto it=colors.begin(); it!=colors.end(); it++)
            for(int i=0; i<5; i++)
                ducks->push(std::make_shared<Duck>(*it));
        for(int i=0; i<cards_water[colors.size()]; i++)
            ducks->push(std::make_shared<Duck>(Water));

        ducks->shuffle();

    //Init le paquet d'actions
        for(auto i=ID_ACTION; i<NB_CARDS; i++) {
            for(auto j=0; j<cards[i].first; j++)
                actions->push(std::make_shared<Action>(i));
        }
        actions->shuffle();
        //log_infos("cartes actions : %u", actions->size());
    }

    int Rules::need_pos(int id) {
        return cards_pos[id-ID_ACTION];
    }

    int Rules::id_color(int c, std::vector<Player*>& players) {
        for(auto it=players.begin(); it!=players.end(); ++it) {
            if((*it) == nullptr) continue;
            if((*it)->get_color() == (Color)c) {
                return (*it)->get_id();
            }
        }
        return -1;
    }

    void Rules::check_cover(int id_p, Board& board) {
        for(int i=0; i<6; i++) {
            if(board.get_slot(i)->get_cover() == id_p)
                board.get_slot(i)->set_cover(-1);
        }
    }

    int Rules::make_action(int id, Choice& choice,
        std::vector<Player*>& players, Board& board, Pack_Action& pack_action) {

        int card_id = -1;
        if(choice.pos1 != -2) { //On ne demande pas un changement de carte
            card_id = players[id]->get_card(choice.id) - ID_ACTION;
            if(!card_funs[card_id](id, choice, players, board, pack_action))
                card_id = -1;
        }
        else {
            card_id = -2;
        }
        if(card_id != -1) {
            Action_ptr p = players[id]->get_hand()->pop_at(choice.id);
            pack_action.push_front(p);
            players[id]->draw(pack_action);
        }

        if(card_id >= 0)
            card_id += ID_ACTION;

        return card_id;
    }


/*
    int id,
    Choice& choice
    std::vector<Player*> players
    Board *board
    Pack_Action *pack_action
*/
    CARD(1) {
        Slot_ptr slot = board.get_slot(choice.pos1);
        if(!slot->is_target() || !slot->is_duck())
            return false;
        slot->set_target(false);

        if(!slot->is_cover()) {
            int idd = slot->get_duck(false)->get_id();
            if(idd != Water) {//pas d'eau
                int idp = id_color(idd, players);
                players[idp]->add_grave(slot->get_duck(true));
                board.compact();
            }
        }

        return true;
    }
    CARD(2) {
        Slot_ptr slot = board.get_slot(choice.pos1);
        if(slot->is_target() || !slot->is_duck())
            return false;
        slot->set_target(true);

        return true;
    }
    CARD(3) {
        board.move();

        return true;
    }
    CARD(4) {
        Slot_ptr slot = board.get_slot(choice.pos1);
        if(!slot->is_duck())
            return false;
        if(slot->get_duck(false)->get_id() != players[id]->get_color())
            return false;
        if(choice.pos1 == 0)
            return false;

        board.switch_cards(choice.pos1, choice.pos1-1);

        return true;
    }
    CARD(5) {
        Slot_ptr slot = board.get_slot(choice.pos1);
        if(!slot->is_duck())
            return false;
        if(slot->get_duck(false)->get_id() != players[id]->get_color())
            return false;
        if(choice.pos1 == 5)
            return false;

        board.switch_cards(choice.pos1, choice.pos1+1);

        return true;
    }
    CARD(6) {
        Slot_ptr slot = board.get_slot(choice.pos1);
        if(!slot->is_duck())
            return false;
        if(slot->is_cover())
            return false;

        slot->set_cover(id);

        return true;
    }
    CARD(7) {
        Slot_ptr slot = board.get_slot(choice.pos1);
        if(!slot->is_target())
            return false;
        if(choice.pos1 == 0)
            return false;
        Slot_ptr slot2 = board.get_slot(choice.pos1-1);
        if(slot2->is_target())
            return false;

        slot->set_target(false);
        slot2->set_target(true);

        return true;
    }
    CARD(8) {
        Slot_ptr slot = board.get_slot(choice.pos1);
        if(!slot->is_target())
            return false;
        if(choice.pos1 == 5)
            return false;
        Slot_ptr slot2 = board.get_slot(choice.pos1+1);
        if(slot2->is_target() || !slot2->is_duck())
            return false;

        slot->set_target(false);
        slot2->set_target(true);

        return true;
    }
    CARD(9) {
        Slot_ptr slot1 = board.get_slot(choice.pos1);
        Slot_ptr slot2 = board.get_slot(choice.pos2);
        if(!slot1->is_duck() || !slot2->is_duck())
            return false;
        if(std::abs(choice.pos1 - choice.pos2) != 1)
            return false;
        if(slot1->get_duck(false)->get_id() != players[id]->get_color())
            return false;

        if(slot2->get_hidden(false))
            return false;
        if(slot2->get_duck(false)->get_id() == Water)
            return false;

        slot2->hide(slot1->get_duck(true));
        board.compact();
        return true;
    }
    CARD(10) {
        if(players[id]->dead_ducks() == 0)
            return false;

        board.push_stack(players[id]->rem_grave());
        board.fill();

        return true;
    }
    CARD(11) {
        Slot_ptr slot1 = board.get_slot(choice.pos1);
        Slot_ptr slot2 = board.get_slot(choice.pos2);
        if(slot1->is_target() || slot2->is_target())
            return false;
        if(std::abs(choice.pos1 - choice.pos2) != 1)
            return false;
        if(!slot1->is_duck() || !slot2->is_duck())
            return false;

        slot1->set_target(true);
        slot2->set_target(true);

        return true;
    }
    CARD(12) {
        Slot_ptr slot = board.get_slot(choice.pos1);
        if(!slot->is_duck())
            return false;
        slot->set_target(false);

        if(!slot->is_cover()) {
            int idd = slot->get_duck(false)->get_id();
            if(idd != Water) {//pas d'eau
                int idp = id_color(idd, players);
                players[idp]->add_grave(slot->get_duck(true));
                board.compact();
            }
        }

        return true;
    }
    CARD(13) {
        Slot_ptr slot;
        for(int i=0; i<6; i++) {
            slot = board.get_slot(i);
            if(slot->is_duck())
                board.push_stack(slot->get_duck(true));
            if(slot->is_duck())
                board.push_stack(slot->get_duck(true));
        }
        board.shuffle_stack();
        board.fill();

        return true;
    }
    CARD(14) {
        Slot_ptr slot1 = board.get_slot(choice.pos1);
        Slot_ptr slot2 = board.get_slot(choice.pos2);
        if(!slot1->is_target() || !slot2->is_duck())
            return false;
        if(std::abs(choice.pos1 - choice.pos2) != 1)
            return false;
        slot1->set_target(false);

        if(!slot2->is_cover()) {
            int idd = slot2->get_duck(false)->get_id();
            if(idd != Water) {//pas d'eau
                int idp = id_color(idd, players);
                players[idp]->add_grave(slot2->get_duck(true));
                board.compact();
            }
        }

        return true;
    }
    CARD(15) {
        Slot_ptr slot1 = board.get_slot(choice.pos1);
        Slot_ptr slot2 = board.get_slot(choice.pos2);
        if(!slot1->is_duck() || !slot2->is_duck())
            return false;
        if(!slot1->is_target() || !slot2->is_target())
            return false;
        if(std::abs(choice.pos1 - choice.pos2) != 1)
            return false;

        slot1->set_target(false);
        slot2->set_target(false);

        int idp = -1;
        if(!slot1->is_cover()) {
            int idd = slot1->get_duck(false)->get_id();
            if(idd != Water) {//pas d'eau
                idp = id_color(idd, players);
                players[idp]->add_grave(slot1->get_duck(true));
            }
        }
        if(!slot2->is_cover()) {
            int idd = slot2->get_duck(false)->get_id();
            if(idd != Water) {//pas d'eau
                idp = id_color(idd, players);
                players[idp]->add_grave(slot2->get_duck(true));
            }
        }
        board.compact();

        return true;
    }
    CARD(16) {
        Slot_ptr slot = board.get_slot(choice.pos1);
        if(!slot->is_duck())
            return false;
        if(slot->get_duck(false)->get_id() != players[id]->get_color())
            return false;
        board.move_card(choice.pos1, 0);

        return true;
    }
    CARD(17) {
        for(int i=0; i<6; i++)
            board.get_slot(i)->set_target(false);

        return true;
    }
    CARD(18) {
        //Canarchie non implémentée !

        return true;
    }

}