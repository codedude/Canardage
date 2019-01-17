
#include "gui/gui.hpp"


namespace Gui {
    void PrintObj::print(WINDOW *win, int x, int y) {
        int s = 0, t = 0;
        for(unsigned int i=0; i<str.size(); i++) {
            if(str[i] == '\n') {
                t++;
                s = 0;
                continue;
            }
            mvwprintw(win, y+t, x+s, "%c", str[i]);
            s++;
        }
    }

    void Window::draw(bool reset) {
        if(!active) return ;
        clear();
        if(reset) {
            delwin(win);
            win = newwin(height, width, y, x);
        }
        draw_border();
        draw_content();
    }

    void Menu::hover(int id) {
        for(auto i=0; i<entries[id].obj.height; i++) {
            wmove(win, entries[id].y+i, entries[id].x-1);
            wchgat(win, entries[id].obj.width+2, A_REVERSE|A_BOLD,
                entries[id].obj.color, NULL);
        }

    }

    Screen::Screen() {
        initscr();
        main_win = stdscr;

        curs_set(0);
        cbreak();
        nodelay(main_win, TRUE);
        keypad(main_win, TRUE);
        noecho();

        if(has_colors() == FALSE) {
            endwin();
            throw std::invalid_argument("Le terminal ne prend pas en charge"
            " les couleurs\n");
        }
        start_color();
    /*
        COLOR_BLACK   0
        COLOR_RED     1
        COLOR_GREEN   2
        COLOR_YELLOW  3
        COLOR_BLUE    4
        COLOR_MAGENTA 5
        COLOR_CYAN    6
        COLOR_WHITE   7
    */
        init_pair(BG, COLOR_BLACK, COLOR_CYAN);

        init_pair(BW, COLOR_BLACK, COLOR_WHITE);
        init_pair(WB, COLOR_WHITE, COLOR_BLACK);

        init_pair(CARD_WHITE, COLOR_WHITE, COLOR_CYAN);
        init_pair(CARD_GREY, COLOR_BLACK, COLOR_CYAN);

        init_pair(RED, COLOR_RED, COLOR_WHITE);
        init_pair(GREEN, COLOR_GREEN, COLOR_WHITE);
        init_pair(BLUE, COLOR_BLUE, COLOR_WHITE);
        init_pair(ORANGE, COLOR_CYAN, COLOR_WHITE);
        init_pair(YELLOW, COLOR_YELLOW, COLOR_WHITE);
        init_pair(PINK, COLOR_MAGENTA, COLOR_WHITE);

        refresh();
    }
    Screen::~Screen() {
        remove_all_win();
        ::refresh();
        ::endwin();
    }

    //Show stdscr on screen
    void Screen::refresh() {
        ::refresh();
        for(auto it=windows.begin(); it != windows.end(); it++) {
            (it->second)->refresh();
        }
    }
    //Draw all windows childrens
    void Screen::draw(bool reset) {
        for(auto it=windows.begin(); it != windows.end(); it++) {
            (it->second)->draw(reset);
        }
        refresh();
    }
    Window *Screen::get_win(int id) {
        return windows[id];
    }
    void Screen::add_win(int id, Window *win) {
        windows[id] = win;
    }
    void Screen::remove_win(int id) {
        ::delwin(windows[id]->get_win());
        auto it = windows.find(id);
        windows.erase(it);
    }
    void Screen::remove_all_win() {
        ::clear();
        for(auto it=windows.begin(); it!=windows.end(); it++)
            remove_win(it->first);
    }
}

