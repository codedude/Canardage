

#ifndef PACK__H
    #define PACK__H

    #include <string>
    #include <deque>
    #include <memory>
    #include <random>
    #include "utils.hpp"

namespace Game {
    typedef enum {
        Red=10, Green=11, Blue=12, Orange=13, Yellow=14, Pink=15,
        Water=16, None=17
    }Color;
/*
    Abstract class card
    Duck and Action inherite from it
*/
    class Card {
    private:
        int id;

    protected:
        Card(int id) :
            id(id) {}

        virtual std::string get_name() const = 0;

    public:
        int get_id() const { return id; }
    };

    class Duck: public Card {
    public:
        Duck(int id) :
            Card(id) {}

        std::string get_name() const {
            return std::string("canard");
        }
    };

    class Action: public Card {
    public:
        Action(int id) :
            Card(id) {}

        std::string get_name() const {
            return std::string("action");
        }
    };

    typedef std::shared_ptr<Duck>   Duck_ptr;
    typedef std::shared_ptr<Action> Action_ptr;
    typedef std::shared_ptr<Card>   Card_ptr;


/*
    Cards pack -> deque list
*/
    template <typename T>
    class Pack {
    private:
        std::string     name;
        std::deque<T>   cards;

    public:
        Pack() :
            name("default") { }
        Pack(std::string pName) :
            name(pName) { }
        ~Pack()
            {}

        std::string get_name() const            { return name; }
        void        set_name(std::string pName) { name = pName; }
        int         size()                 { return cards.size(); }

        void push(T card)          { cards.push_back(card); }
        void push_front(T card)         { cards.push_front(card); }
        void push_at(T card, int pos)   { cards.insert(cards.begin()+pos, card
                                        ); }

        /*
            Check if pos and deque is valid -> return null (0) else
        */
        T at(int pos) {
            T c = nullptr;
            if(pos < size() && pos >= 0) {
                c = cards.at(pos);
            }
            return c;
        }
        T pop() {
            T c = nullptr;
            if(size()) {
                c = cards.back();
                cards.pop_back();
            }
            return c;
        }
        T pop_front() {
            T c = nullptr;
            if(size()) {
                c = cards.front();
                cards.pop_front();
            }
            return c;
        }
        T pop_at(int pos) {
            T c = nullptr;
            if(pos < size() && pos >= 0) {
                c = cards.at(pos);
                cards.erase(cards.begin()+pos);
            }
            return c;
        }

        void shuffle() {
            auto dice = [this](){ return rand()%cards.size(); };
            for(int i=0; i<5*(int)cards.size(); i++)
                push_at(pop_at(dice()), dice());
        }

    };

    typedef Pack<Duck_ptr>      Pack_Duck;
    typedef Pack<Action_ptr>    Pack_Action;
    typedef Pack<Card_ptr>      Pack_Card;
}

#endif

