

#ifndef TEMPLATE__H
    #define TEMPLATE__H

    #include "utils.hpp"
    #include "gui/gui.hpp"
    #include <game/data.hpp>
    #include <game/rules.hpp>
    #include <ncurses.h>
    #include <string>
    #include <list>

namespace Gui {
    class Win_Background : public Window {
    public:
        Win_Background() : Window(MAX_WIDTH, MAX_HEIGHT, 1, 1, true) {};

        ~Win_Background() {
            clear(); delwin(win);
        }

        void draw_border() {
            wbkgd(win, COLOR_PAIR(BG));
            wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
        };

        void draw_content() { };
    };

    class Win_Title : public Window {
    public:
        Win_Title() : Window(69, 3, (MAX_WIDTH-69)/2, 4, true) {}

        ~Win_Title() {
            clear(); delwin(win);
        }

        void draw_border() {
            wbkgd(win, COLOR_PAIR(BW));
            wborder(win, '#', '#', '#', '#', '#', '#', '#', '#');
        }

        void draw_content() {
            wattron(win, A_BOLD);
            print_mid(1, "Canardage");
            wattroff(win, A_BOLD);
        }
    };

    class Win_MenuTitle : public Menu {
    public:
        Win_MenuTitle();

        void select_entry(int key);

        void draw_border() {
            wbkgd(win, COLOR_PAIR(BW));
            wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
        }

        void draw_content();

    };

    class Win_TitleColor : public Window {
    public:
        Win_TitleColor() : Window(MAX_WIDTH-3, 5, 2, 2, true){};

        ~Win_TitleColor() {
            clear(); delwin(win);
        }

        void draw_border() {
            wbkgd(win, COLOR_PAIR(BW));
            wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
        }

        void draw_content() {
            print_mid(0, "Choisir la couleur du canard");
            mvwprintw(win, 3, 1, "Choisissez une couleur. Mais depechez vous, "
                "sinon vous n'aurez plus le choix !");
        }
    };

    class Win_MenuColor : public Menu {
    public:
        Win_MenuColor();

        void select_entry(int key);
        void draw_border() {
            wbkgd(win, COLOR_PAIR(BW));
            wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
        }
        void draw_content();

    };

    class Win_Console : public Window {
    private:
        int n;
        std::list<std::string> msgs;

    public:
        Win_Console() :
        Window(46, 10, 2, 26, true), n(8) { };

        void add_entry(std::string msg);
        void draw_border() {
            wbkgd(win, COLOR_PAIR(BW));
            wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
        }
        void draw_content();

    };

    class Win_PlayButt : public Window {
    public:
        Win_PlayButt() : Window(9, 5, 110, 23, false) {}

        ~Win_PlayButt() {
            clear(); delwin(win);
        }

        void draw_border() {
            wbkgd(win, COLOR_PAIR(BW));
            wborder(win, '#', '#', '#', '#', '#', '#', '#', '#');
        }

        void draw_content() {
            wattron(win, A_BOLD);
            print_mid(2, "Jouer");
            wattroff(win, A_BOLD);
        }
    };

    class Win_Info_Players : public Window {
    private:
        Game::Data *data;
    public:
        Win_Info_Players(Game::Data *data) : Window(MAX_WIDTH-3, 2, 2, 2, true),
        data(data) {}

        ~Win_Info_Players() {
            clear(); delwin(win);
        }

        void draw_border() {
            wbkgd(win, COLOR_PAIR(BG));
            wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
        }

        void draw_content();
    };

    class Win_Info_Party : public Window {
    private:
        Game::Data *data;
    public:
        Win_Info_Party(Game::Data *data) :
        Window(8, 3, MAX_WIDTH-18, MAX_HEIGHT/2-4, true), data(data) {}

        ~Win_Info_Party() {
            clear(); delwin(win);
        }

        void draw_border() {
            wbkgd(win, COLOR_PAIR(BG));
            wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
        }

        void draw_content();
    };

    class Win_Hand : public Menu {
    private:
        Game::Data *data;
        bool selectable;
    public:
        Win_Hand(Game::Data *data);
        void update();
        void enable_select() { selectable = true;}
        void disable_select() { selectable = false; }
        bool is_select() const { return selectable; }

        void select_entry(int key);
        void draw_border() {
            wbkgd(win, COLOR_PAIR(BG));
            wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
        }
        void draw_content();

    };

    /*
        Un peu comme Menu mais avec gestion particulière des cartes
    */

    class Card {
    public:
        Game::Color* color;
        Game::Color* hidden;
        Card() {};
        Card(Game::Color* color, Game::Color* hidden) :
            color(color), hidden(hidden) {};
        ~Card() {};

        static std::string const target_s;
        static std::string const border_s;
        static std::string const none_s;
        static std::string const water_s;
        static std::string const duck_s;
        static std::string const duck_double_s;

        void print(WINDOW *win, int x, int y);
    };

    typedef struct Slot Slot;
    struct Slot {
        int x, y;
        bool* target;
        int* cover;
        Card card;
        Slot() {};
        Slot(int x, int y, bool* target, int* cover, Card card) :
            x(x), y(y), target(target), cover(cover), card(card) {}
        void print(WINDOW *win);
        void hover(WINDOW *win);
    };

    class Win_Board : public Window {
    private:
        Game::Data *data;
        int selected_entry;
        bool selectable;
        std::map<int, Slot> entries;
    public:
        Win_Board(Game::Data *data);

        void enable_select() { selectable = true; selected_entry = 0;}
        void disable_select() { selectable = false; }
        bool is_select() const { return selectable; }

        int get_entry() const { return selected_entry; };
        void add_entry(int id, int x, int y, bool* target, int* cover, Card obj);
        void select_entry(int key); //Entoure la carte choisie
        void hover(WINDOW *win, int id) { entries[id].hover(win); }
        void draw_border();
        void draw_content();
    };

    class Win_Popup_Card : public Menu {
    private:
        Game::Data *data;
        bool selectable;
        int c_id;
    public:
        Win_Popup_Card(Game::Data *data);

        void enable_select() { selectable = true; selected_entry = 0;}
        void disable_select() { selectable = false; }
        bool is_select() const { return selectable; }

        void select_entry(int key); //Entoure la carte choisie
        void draw_border();
        void draw_content();
        void show(int pc_id);
        void unshow();
    };

    class Win_Player : public Window {
    private:
        Game::Data *data;
    public:
        Win_Player(Game::Data *data);

        void draw_border();
        void draw_content();
    };

}
#endif

