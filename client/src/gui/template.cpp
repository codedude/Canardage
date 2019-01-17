
#include "gui/gui.hpp"
#include "gui/template.hpp"

#include "utils.hpp"
#include <game/data.hpp>
#include <game/rules.hpp>
#include <ncurses.h>
#include <string>
#include <list>


namespace Gui {

//Win_MenuTitle
    Win_MenuTitle::Win_MenuTitle() :
    Menu(24, 5, (MAX_WIDTH-24)/2, (MAX_HEIGHT-5)/2 - 2, 0, true) {
        PrintObj play(5, 1, BW, "Jouer");
        PrintObj quit(7, 1, BW, "Quitter");
        add_entry(0, 1, 1, play);
        add_entry(1, 1, 3, quit);
        entries[0].x = (width-entries[0].obj.width)/2;
        entries[1].x = (width-entries[1].obj.width)/2;
    }

    void Win_MenuTitle::select_entry(int key) {
        int id = -1;

        switch(key) {
            case KEY_UP:
            case KEY_DOWN:
                id = (selected_entry==0)?1:0;
            break;

            default: break;
        }

        if(id != -1) {
            hover(selected_entry);
            selected_entry = id;
            hover(selected_entry);
        }
    }

    void Win_MenuTitle::draw_content() {
        for(auto it=entries.begin(); it!=entries.end(); it++) {
            wattron(win, COLOR_PAIR(it->second.obj.color));
            mvwprintw(win, it->second.y, it->second.x, "%s",
                it->second.obj.get_str());
            wattroff(win, COLOR_PAIR(it->second.obj.color));
        }
        hover(selected_entry);
    }


//Win_MenuColor
    Win_MenuColor::Win_MenuColor() :
    Menu(26, 13, (MAX_WIDTH-20)/2, (MAX_HEIGHT-10)/2 - 2, 0, true) {
        PrintObj duck(5, 4, RED,
            "##   \n"
            " #  #\n"
            " ####\n"
            "  ## \n"
        );

        for(int i=0; i<6; i++) {
            duck.color = RED + i;
            add_entry(i, (i%3)*8 +2, (i/3)*6 +1, duck);
        }
    }

    void Win_MenuColor::select_entry(int key) {
        int id = -1;

        switch(key) {
            case KEY_UP:
            case KEY_DOWN:
                id = (selected_entry + 3) % 6;
            break;

            case KEY_RIGHT:
                id = (selected_entry + 1)%3 + (selected_entry/3)*3;
            break;
            case KEY_LEFT:
                id = (selected_entry + 5)%3 + (selected_entry/3)*3;
            break;

            default: break;
        }

        if(id != -1) {
            hover(selected_entry);
            selected_entry = id;
            hover(selected_entry);
        }
    }

    void Win_MenuColor::draw_content() {
        for(auto it=entries.begin(); it!=entries.end(); it++) {
            wattron(win, COLOR_PAIR(it->second.obj.color));
            it->second.obj.print(win, it->second.x, it->second.y);
            wattroff(win, COLOR_PAIR(it->second.obj.color));
        }
        hover(selected_entry);
    }

//Win_Console
    void Win_Console::add_entry(std::string msg) {
        if((int)msgs.size() == n) {
            msgs.pop_front();
        }
        msgs.push_back(msg);
    }

    void Win_Console::draw_content() {
        int i=1, j=1;

        for(auto it=msgs.begin(); it!=msgs.end(); it++) {
            for(unsigned int c=0; c<it->size(); c++) {
                if(i == height-1) return;
                mvwprintw(win, i, j++, "%c", it->at(c));
                if(j == width-1) {
                    j = 1;
                    i++;
                }
            }
            i++;
            j = 1;
        }
    }

//Win_Info_Players
    void Win_Info_Players::draw_content() {
        int y = 1, x = 10, step = 15;
        for(unsigned int i=0; i<data->p_color.size(); i++) {
            if(data->p_color[i] == -1) continue;
            wattron(win, COLOR_PAIR(data->p_color[i]));
            mvwprintw(win, y, x+step, "J%u : %d/5", i, 5-data->p_grave[i]);
            wattroff(win, COLOR_PAIR(data->p_color[i]));
            step += 15;
        }
    }

//Win_Info_Party
    void Win_Info_Party::draw_content() {
        mvwprintw(win, 1, 1, "# x%d", data->card_duck);

    }

//Win_Hand
        Win_Hand::Win_Hand(Game::Data *data) :
    Menu(42, 6, 56, 28, 0, true), data(data), selectable(true) {
        update();
    }

    void Win_Hand::update() {
        entries.clear();
        for(unsigned int i=0; i<data->hand.size(); i++) {
            PrintObj s(Game::Rules::card_name(data->hand[i]).size(), 1, BG ,
                Game::Rules::card_name(data->hand[i]));
            add_entry(i, (i%2)*20+2, (i/2)*3+1, s);
        }
    }
    void Win_Hand::select_entry(int key) {
        int id = -1;

        switch(key) {
            case KEY_UP:
            case KEY_DOWN:
                id = (selected_entry + 2) % 4;
            break;

            case KEY_RIGHT:
                id = (selected_entry + 1)%2 + (selected_entry/2)*2;
            break;
            case KEY_LEFT:
                id = (selected_entry + 3)%2 + (selected_entry/2)*2;
            break;

            default: break;
        }

        if(id != -1) {
            hover(selected_entry);
            selected_entry = id;
            hover(selected_entry);
        }
    }

    void Win_Hand::draw_content() {
        update();
        for(auto it=entries.begin(); it!=entries.end(); it++) {
            wattron(win, COLOR_PAIR(it->second.obj.color));
            it->second.obj.print(win, it->second.x, it->second.y);
            wattroff(win, COLOR_PAIR(it->second.obj.color));
        }
        hover(selected_entry);
    }

//Card
    //Rouge ou caché 3x3
    std::string const Card::target_s =
        " # \n"
        "###\n"
        " # \n"
    ;
    //Grisé 10x10
    std::string const Card::border_s =
        "##########\n"
        "#        #\n"
        "#        #\n"
        "#        #\n"
        "#        #\n"
        "#        #\n"
        "#        #\n"
        "##########\n"
    ;
    //Grisé 10x10
    std::string const Card::none_s =
        "        \n"
        "        \n"
        "        \n"
        "        \n"
        "        \n"
        "        \n"
    ;
    //Canard 10x10 -> 6 couleurs
    std::string const Card::duck_s =
        "        \n"
        " ##   # \n"
        "  #  ## \n"
        "  ##### \n"
        "   ###  \n"
        "        \n"
    ;
    //Eau 10x10 -> bleu
    std::string const Card::water_s =
        "        \n"
        "        \n"
        "~ ~ ~ ~ \n"
        "~~~~~~~~\n"
        "~~~~~~~~\n"
        "~~~~~~~~\n"
    ;
    //Canard + planqué 10x10 -> 2 couleurs différentes
    //2eme couleur 6-7, sep = 5, 1ere couleur = 1-3
    std::string const Card::duck_double_s =
        " ##   # \n"
        "  #  ## \n"
        "  ##### \n"
        "@@@@@@@@\n"
        "   ##   \n"
        "  ####  \n"
    ;

    void Card::print(WINDOW *win, int x, int y) {
        if(*color != -1) {
            int s = 2, t = 5;
            //Double couleur
            if(*hidden != -1) {
                wattron(win, COLOR_PAIR(*color));
                for(unsigned int i=0; i<duck_double_s.size(); i++) {
                    if(duck_double_s[i] == '@') {
                        wattroff(win, COLOR_PAIR(*color));
                        wattron(win, COLOR_PAIR(BW));
                        for(;duck_double_s[i] != '\n';i++) {
                            mvwprintw(win, y+t, x+s, "%c", duck_double_s[i]);
                            s++;
                        }
                        s = 2;
                        t++;
                        wattroff(win, COLOR_PAIR(BW));
                        wattron(win, COLOR_PAIR(*hidden));
                        continue;
                    }
                    if(duck_double_s[i] == '\n') {
                        t++;
                        s = 2;
                        continue;
                    }
                    mvwprintw(win, y+t, x+s, "%c", duck_double_s[i]);
                    s++;
                }
                wattroff(win, COLOR_PAIR(*hidden));
            }
            //carte normale
            else {
                int attr_c;
                std::string str_c;
                //Eau
                if(*color == Game::Water) {
                    attr_c = COLOR_PAIR(BLUE);
                    str_c = water_s;
                }
                //Canard
                else {
                    attr_c = COLOR_PAIR(*color);
                    str_c = duck_s;
                }
                wattron(win, attr_c);
                for(unsigned int i=0; i<str_c.size(); i++) {
                    if(str_c[i] == '\n') {
                        t++;
                        s = 2;
                        continue;
                    }
                    mvwprintw(win, y+t, x+s, "%c", str_c[i]);
                    s++;
                }
                wattroff(win, attr_c);
            }
        }
        //Pas de carte : rien à afficher
        else { }
    }

//Slot
    void Slot::print(WINDOW *win) {
        int s, t;
        if(*target) {
            //Affiche la cible
            s = 4, t = 0;
            for(unsigned int i=0; i<Card::target_s.size(); i++) {
                if(Card::target_s[i] == '\n') {
                    t++;
                    s = 4;
                    continue;
                }
                wattron(win, COLOR_PAIR(RED));
                mvwprintw(win, y+t, x+s, "%c", Card::target_s[i]);
                wattroff(win, COLOR_PAIR(RED));
                s++;
            }
        }

        //Affiche la bordure de la carte
        int attr_c;
        if(*cover >= 0) {
            attr_c = COLOR_PAIR(CARD_GREY);
        }else{
            attr_c = COLOR_PAIR(CARD_WHITE);
        }
        s = 1, t = 4;
        wattron(win, attr_c);
        for(unsigned int i=0; i<Card::border_s.size(); i++) {
            if(Card::border_s[i] == '\n') {
                t++;
                s = 1;
                continue;
            }
            mvwprintw(win, y+t, x+s, "%c", Card::border_s[i]);
            s++;
        }
        wattroff(win, attr_c);

        card.print(win, x, y);
    }

    void Slot::hover(WINDOW *win) {
        wattron(win, COLOR_PAIR(WB));
        for(int t=0; t<10; t++) {
            for(int s=0; s<12; s++) {
                if(t != 0 && t != 9) {
                    if(s != 0 && s != 11) {
                        continue;
                    }
                }
                mvwprintw(win, y+t+3, x+s, " ");
            }
        }
        wattroff(win, COLOR_PAIR(WB));
    }
//Win_Board
    //A couvert -> bordure noire ? blanche en temps normal
    Win_Board::Win_Board(Game::Data *data) :
    Window(90, 17, 20, 6, true), data(data), selected_entry(0),
    selectable(false) {
        for(unsigned int i=0; i<data->board.size(); i++) {
            Card c_tmp((Game::Color*)&data->board[i]["over"],
                (Game::Color*)&data->board[i]["under"]);
            add_entry((int)i, (int)2+i*14, (int)2, (bool*)&data->board[i]["target"],
                (int*)&data->board[i]["cover"], c_tmp);
        }
    }

    void Win_Board::add_entry(int id, int x, int y, bool* target, int* cover,
        Card card) {
        entries[id] = Slot(x, y, target, cover, card);
    }
    void Win_Board::select_entry(int key) {
        int id = -1;

        switch(key) {
            case KEY_RIGHT:
                id = (selected_entry + 1) % 6;
            break;
            case KEY_LEFT:
                id = (selected_entry + 5) % 6;
            break;

            default: break;
        }

        if(id != -1) {
            hover(win, selected_entry);
            selected_entry = id;
            hover(win, selected_entry);
        }
    }

    void Win_Board::draw_border() {
        wbkgd(win, COLOR_PAIR(BG));
        wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE,
            ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    }
    void Win_Board::draw_content() {
        for(auto it=entries.begin(); it!=entries.end(); it++) {
            it->second.print(win);
        }
        if(selectable) {
            hover(win, selected_entry);
        }
    }

//Win_Popup_Card
    Win_Popup_Card::Win_Popup_Card(Game::Data *data) :
    Menu(60, 9, 38, 17, 0, false), data(data), selectable(false), c_id(-1) {
        PrintObj b1(5, 1, BW, "Jouer"), b2(8, 1, BW, "Echanger"),
        b3(6, 1, BW, "Retour");
        add_entry(0, 51, 2, b1);
        add_entry(1, 50, 4, b2);
        add_entry(2, 51, 6, b3);
    }

    void Win_Popup_Card::select_entry(int key) {
        int id = -1;

        switch(key) {
            case KEY_UP:
                id = (selected_entry + 2) % 3;
            break;
            case KEY_DOWN:
                id = (selected_entry + 1) % 3;
            break;

            default: break;
        }

        if(id != -1) {
            hover(selected_entry);
            selected_entry = id;
            hover(selected_entry);
        }
    }
    void Win_Popup_Card::draw_border() {
        wbkgd(win, COLOR_PAIR(BW));
        wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE,
            ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    }
    void Win_Popup_Card::draw_content() {
        //Dessine le menu
        for(auto it=entries.begin(); it!=entries.end(); it++) {
            wattron(win, COLOR_PAIR(it->second.obj.color));
            mvwprintw(win, it->second.y, it->second.x, "%s",
                it->second.obj.get_str());
            wattroff(win, COLOR_PAIR(it->second.obj.color));
        }
        hover(selected_entry);

        //Dessine le texte;
        print_mid(1, Game::Rules::card_name(c_id));
        //Dessine la desc
        int s = 5, t = 3;
        for(int i=0; i<(int)Game::Rules::card_desc(c_id).size(); i++) {
            mvwprintw(win, t, s, "%c", Game::Rules::card_desc(c_id).at(i));
            s++;
            if(s == 42) {
                s = 1;
                t++;
            }
        }
    }

    void Win_Popup_Card::show(int pc_id) {
        activate();
        enable_select();
        c_id = data->hand[pc_id];
    }

    void Win_Popup_Card::unshow() {
        disable_select();
        deactivate();
    }

//Win_Player
    Win_Player::Win_Player(Game::Data *data) :
    Window(12, 3, 2, 2, true), data(data) {}

    void Win_Player::draw_border() {
        wbkgd(win, COLOR_PAIR(BG));
        wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    }

    void Win_Player::draw_content() {
        wattron(win, COLOR_PAIR(data->color));
        mvwprintw(win, 1, 1, "Joueur : %d", data->id);
        wattroff(win, COLOR_PAIR(data->color));
    }
}

