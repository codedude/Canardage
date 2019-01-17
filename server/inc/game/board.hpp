

#ifndef BOARD__H
    #define BOARD__H

    #include "game/pack.hpp"
    #include "json.hpp"

    #include <iostream>
    #include <memory>
    #include <vector>


namespace Game {
/*
    Inner class of GameBoard, contains target + layers
*/
    class Slot {
    private:
        Duck_ptr    over;
        Duck_ptr    under;
        bool        target;
        int         cover;  //is this slot cover ? (-1=no, else=player id)

    public:
        Slot() :
            over(nullptr), under(nullptr), target(false), cover(-1) { }

        void        set_target(bool pTarget)    { target = pTarget; }
        bool        is_target() const           { return target; }

        void        set_cover(int pCover)       { cover = pCover;}
        int         get_cover()                 { return cover;}
        bool        is_cover() const            { return (cover>=0); }

        bool        is_hidden() const           { return !(under == nullptr); }
        bool        is_duck() const             { return !(over == nullptr); }

        void        set_duck(Duck_ptr pDuck);
        Duck_ptr    get_hidden(bool e);
        Duck_ptr    get_duck(bool e);
        void        unhide();
        void        hide(Duck_ptr pDuck);

    };

    typedef std::shared_ptr<Slot> Slot_ptr;

/*
    Game board, contains slots
*/
    class Board {
    private:
        std::vector<Slot_ptr>   slots;
        Pack_Duck               *stack;

    public:
        Board(int n, Pack_Duck *pack);
        ~Board() {
            delete stack;
        }

        void        move();
        void        fill();
        void        switch_cards(int src, int dest);
        void        move_card(int src, int dest);
        void        shuffle_stack() { stack->shuffle(); }
        void        push_stack(Duck_ptr card)       { stack->push(card); }
        Slot_ptr    get_slot(int i) const           { return slots.at(i); }
        void        get_json(nlohmann::json *data);
        void        compact();

    };

    typedef std::shared_ptr<Board> Board_ptr;

}


#endif

