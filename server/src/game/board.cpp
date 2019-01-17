
#include "game/pack.hpp"
#include "game/board.hpp"
#include "json.hpp"
#include "utils.hpp"

#include <string>
#include <memory>
#include <deque>
#include <vector>
#include <map>

using json = nlohmann::json;

using namespace std;

namespace Game {
    void Slot::set_duck(Duck_ptr pDuck) {
        over = pDuck;
    }

    Duck_ptr Slot::get_hidden(bool e=false) {
        Duck_ptr p = under;
        if(e) {
            under = nullptr;
        }
        return p;
    }
    Duck_ptr Slot::get_duck(bool e) {
        Duck_ptr p = over;
        if(e) {
            over = nullptr;
            unhide();
        }
        return p;
    }

    void Slot::unhide() {
        over = under;
        under = nullptr;
    }
    void Slot::hide(Duck_ptr pDuck) {
        under = pDuck;
    }

    Board::Board(int n, Pack_Duck *pack){
        stack = pack;
        stack->set_name(string("Duck stack"));

        slots.resize(n);
        for(int i=0; i<n; i++) {
            slots[i] = make_shared<Slot>();
        }
    }

    void Board::move() {
        Slot_ptr p = slots[0];
        if(!p) return ;

        if(p->is_duck())
            push_stack(p->get_duck(true));
        if(p->is_duck()) //hidden ?
            push_stack(p->get_duck(true));

        compact();
    }
    void Board::fill() {
        for(unsigned i=0; i<slots.size(); i++) {
            if(!stack->size()) break;
            if(slots[i]->is_duck()) continue;
            slots[i]->set_duck(stack->pop_front());
        }
    }

    void Board::compact() {
        for(unsigned i=0; i<slots.size(); i++) {
            if(slots[i]->is_duck()) continue;
            int j = i;
            while(j < (int)slots.size()) {
                if(slots[j]->is_duck()) break;
                j++;
            }
            if(j == (int)slots.size()) break;
            slots[i]->set_duck(slots[j]->get_duck(true));
            slots[i]->hide(slots[j]->get_duck(true));
        }
        fill();
    }


    void Board::switch_cards(int src, int dest) {
        Duck_ptr over_src = slots[src]->get_duck(true);
        Duck_ptr under_src = slots[src]->get_duck(true);

        Duck_ptr over_dest = slots[dest]->get_duck(true);
        Duck_ptr under_dest = slots[dest]->get_duck(true);

        slots[src]->set_duck(over_dest);
        slots[src]->hide(under_dest);

        slots[dest]->set_duck(over_src);
        slots[dest]->hide(under_src);

    }
    void Board::move_card(int src, int dest) {
        Duck_ptr p = slots[src]->get_duck(true);
        Duck_ptr ph = slots[src]->get_duck(true);

        int inc = 1;
        if(src > dest) inc = -1;

        for(int i=src; i!=dest; i+=inc) {
            slots[i]->set_duck(slots[i+inc]->get_duck(true));
            slots[i]->hide(slots[i+inc]->get_duck(true));
        }

        slots[dest]->set_duck(p);
        slots[dest]->hide(ph);
    }

    void Board::get_json(json *data) {
        //nombre de cartes restantes dans la pile canard
        (*data)["ducks"] = stack->size();

        //Emplacements du plateau : id, id caché, couvert, cible
        Duck_ptr over, under;
        (*data)["board"] = {};
        for(unsigned int i=0; i<slots.size(); i++) {
            (*data)["board"][i] = {};
            over = slots[i]->get_duck(false);
            under = slots[i]->get_hidden();

            (*data)["board"][i]["over"] = (over==nullptr)?-1:over->get_id();
            (*data)["board"][i]["under"] =
                (under==nullptr)?-1:under->get_id();
            (*data)["board"][i]["cover"] = (int)slots[i]->get_cover();
            (*data)["board"][i]["target"] = (int)slots[i]->is_target();
            (*data)["board"][i]["id"] = (int)i;
        }
    }
}