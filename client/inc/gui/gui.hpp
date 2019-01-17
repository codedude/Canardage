

#ifndef WINDOW__H
    #define WINDOW__H

    #include "utils.hpp"
    #include <stdexcept>
    #include <map>
    #include <ncurses.h>

    //Define area of the game
    #define MAX_WIDTH 128
    #define MAX_HEIGHT 36

    #define BG      1
    #define BW      2
    #define WB      3
    #define CARD_WHITE  4
    #define CARD_GREY   5

    #define RED     10
    #define GREEN   11
    #define BLUE    12
    #define ORANGE  13
    #define YELLOW  14
    #define PINK    15
    #define WATER   16
    #define NONE    17

namespace Gui {
    #define DEBUG_G {mvwprintw(stdscr, 0, 0, "DEBUG");\
                    refresh();}
    //Objet imprimable = ensembles de caractères
    //1 seul ou un dessin ascci, considéré comme 1 seul élément
    class PrintObj {
    public:
        std::string str;

        int width, height;
        int color;

        PrintObj() {};
        PrintObj(int w, int h, int c, std::string s)
            : str(s), width(w), height(h), color(c) {}
        virtual ~PrintObj() {};

        const char *get_str() const { return str.c_str(); }
        void print(WINDOW *win, int x, int y);
    };

    class Window {
    protected:
        WINDOW *win;
        int width, height;
        int x, y;
        bool active;

    public:
        Window(int w, int h, int x, int y, bool pactive) :
         width(w), height(h), x(x), y(y), active(pactive) {
            win = newwin(height, width, y, x);
        }
        virtual ~Window() {}

        WINDOW *get_win() { return win; }

        void activate() { active = true; }
        void deactivate() { active = false; }
        bool is_active() const { return active; }

        void refresh() {
            if(!active) return ;
            wrefresh(win);
        }

        void clear() {
            if(!active) return ;
            wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
            wclear(win);
        }

        void draw(bool reset=false);

        void print_mid(int y, std::string str) {
            mvwprintw(win, y, (width-str.size())/2, str.c_str());
        }

        virtual void draw_border() {};
        virtual void draw_content() {};
    };

    typedef struct MenuEntry MenuEntry;
    struct MenuEntry {
        int id, x, y;
        PrintObj obj;
        MenuEntry() {};
        MenuEntry(int id, int x, int y, PrintObj obj)
        : id(id), x(x), y(y), obj(obj) {}
    };

    class Menu : public Window {
    protected:
        std::map<int, MenuEntry> entries;
        int selected_entry;

    public:
        Menu(int w, int h, int x, int y, int id, bool pactive) :
         Window(w, h, x, y, pactive), selected_entry(id) {};

        virtual ~Menu() {}

        virtual void add_entry(int id, int x, int y, PrintObj obj) {
            entries.insert(std::make_pair(id, MenuEntry(id, x, y, obj)));
        }

        virtual int get_entry() const { return selected_entry; }

        virtual void hover(int id);

        virtual void select_entry(int key) = 0;
        virtual void draw_border() {};
        virtual void draw_content() {};
    };


    /* Main terminal, contains map of windows */
    class Screen {
    private:
        WINDOW *main_win;
        std::map<int, Window *> windows;

    public:
        Screen();
        ~Screen();

        //Show stdscr on screen
        void refresh();
        //Draw all windows childrens
        void draw(bool reset=false);

        Window *get_win(int id);
        void add_win(int id, Window *win);
        void remove_win(int id);
        void remove_all_win();
    };
}
#endif

