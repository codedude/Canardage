

#ifndef PLAYER__H
    #define PLAYER__H

    #include "utils.hpp"
    #include "json.hpp"
    #include "game/pack.hpp"
    #include <string>
    #include <memory>

namespace Game {
    typedef struct Choice Choice;
    struct Choice {
        int id; //Id de la carte
        int pos1, pos2; //Emplacements, 0-1-2 nécessaire selon la carte
    };

    class Player {
    private:
        int         id;
        bool        ready;
        Color       color;
        Pack_Action hand;
        Pack_Duck   grave;

    public:
        Player(int id) : id(id), color(None) { }
        virtual ~Player() {};

        virtual int     draw(Pack_Action& pack);

        void            set_color(Color pColor)
            {   color = (Color)(pColor+10); }
        int             get_id() const              { return id; }
        Color           get_color() const           { return color; }
        bool            is_ready() const            { return ready; }
        void            set_ready(bool pReady)      { ready = pReady; }

        Pack_Action*     get_hand()                 { return &hand; }
        Pack_Duck*       get_grave()                { return &grave; }

        int             get_card(int pid) { return hand.at(pid)->get_id();}

        void add_grave(Duck_ptr duck) {
            grave.push(duck);
        }
        Duck_ptr rem_grave() {
            return grave.pop();
        }

        int dead_ducks() {
            return (int)grave.size();
        }

        nlohmann::json *get_json();
    };

    class Human: public Player {
    public:
        Human(int id) : Player(id) { }
        ~Human() {};
    };


/**
    TO-DO : simple IA
**/
    class Computer: public Player {
    private:
        int level;

    public:
        Computer(int id, int level) : Player(id), level(level) { }
        ~Computer();

        int pick_card() { return 0; }
    };

}

#endif

