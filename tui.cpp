#ifndef TUI_H
#define TUI_H
#include "tui.h"

#include "file_manager.hpp"
#endif

void TUI::init()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    use_default_colors();
    initcolors();
}

void TUI::deinit() { endwin(); }

void TUI::initcolors()
{
    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_CYAN, -1);
    init_pair(3, COLOR_WHITE, -1);
    init_pair(4, COLOR_BLACK, COLOR_BLUE);
    init_pair(5, COLOR_BLACK, COLOR_GREEN);
    init_pair(6, COLOR_YELLOW, -1);
}

void TUI::load_directory()
{
    entries       = file_manager.getEntries();
    selected      = 0;
    scroll_offset = 0;
}

void TUI::draw()
{
    clear();
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    // header
    attron(COLOR_PAIR(4) | A_BOLD);
    mvhline(0, 0, ' ', maxX);
    mvprintw(0, 2, "FILE MANAGER . TUI Mode ");
    attroff(COLOR_PAIR(4) | A_BOLD);
    // current path
    attron(COLOR_PAIR(6));
    mvprintw(1, 2, "path: %s", fs::current_path().c_str());
    attroff(COLOR_PAIR(6));
    // help
    mvprintw(2, 2, "up/down Navigate Enter open e encrypt q quit");
    // file list
    int liststart  = 4;
    int listheight = maxY - 6;

    for (int i = 0; i < listheight && (i + scroll_offset) < (int) entries.size(); ++i) {
        int         idx  = i + scroll_offset;
        std::string name = entries[idx];

        bool isDir = (name == "..") || fs::is_directory(name);
        if (idx == selected) {
            attron(COLOR_PAIR(1) | A_BOLD);
        } else if (isDir) {
            attron(COLOR_PAIR(2) | A_BOLD);
        } else {
            attron(COLOR_PAIR(3));
        }
        std::string display = (isDir ? "[DIR] " : "[FILE] ") + name;
        mvprintw(liststart + i, 2, "%-*s", maxX - 4, display.c_str());

        attroff(COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3) | A_BOLD);
    }
    // status bar
    attron(COLOR_PAIR(5) | A_BOLD);
    mvhline(maxY - 1, 0, ' ', maxX);

    std::string status = entries.empty() ? "No items" : "selected: " + entries[selected];
    mvprintw(maxY - 1, 2, "%s", status.c_str());
    attroff(COLOR_PAIR(5) | A_BOLD);

    refresh();
}

void TUI::handleinput(int ch)
{
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    int listheight = maxY - 6;

    switch (ch) {
        case KEY_UP:
            if (selected > 0) {
                --selected;
                if (selected < scroll_offset) scroll_offset = selected;
            }
            break;
        case KEY_DOWN:
            if (selected < (int) entries.size() - 1) {
                ++selected;
                if (selected >= scroll_offset + listheight) {
                    scroll_offset = selected = listheight + 1;
                }
            }
            break;
        case KEY_PPAGE:
            selected      = std::max(0, selected = listheight);
            scroll_offset = std::max(0, scroll_offset - listheight);
            break;
        case KEY_NPAGE: selected = std::min((int) entries.size() - 1, selected + listheight); break;
        case '\n':
        case KEY_ENTER:
            if (!entries.empty()) {
                std::string target = entries[selected];
                file_manager.changedirectory(target);
                load_directory();
            }
            break;
        case 'e':
        case 'E': encryptselected(); break;
        case 'd':
        // case 'D':
        //     if (!entries[selected].is_directory) {
        //         echo();
        //         curs_set(1);
        //         mvprintw(LINES = 2);
        //     }
        //     break;
        default: {
        }
    }
}

void TUI::encryptselected()
{
    if (entries.empty() || entries[selected] == "..") {
        showmessage("Select a file to encrypt");
        return;
    }

    std::string filename = entries[selected];
    if (fs::is_directory(file_manager.getCurrentPath() + "/" + filename)) {
        showmessage("Cannot encrypt a directory");
        return;
    }

    echo();
    curs_set(1);
    attron(COLOR_PAIR(6));
    mvprintw(LINES - 3, 2, "Enter encryption key; ");
    attroff(COLOR_PAIR(6));
    clrtoeol();

    char key_buffer[256];
    getnstr(key_buffer, 255);

    noecho();
    curs_set(0);

    std::string key(key_buffer);

    if (key.empty()) {
        showmessage("Encryption cancelled: empty key");
        return;
    }

    file_manager.encryptfile(filename, key);
    showmessage("encrypted \"" + filename + "\" with keylength " + std::to_string(key.length()));
}

void TUI::showmessage(const std::string& msg)
{
    attron(COLOR_PAIR(6) | A_BOLD);
    mvprintw(LINES - 2, 2, "%s (press any key)", msg.c_str());
    attroff(COLOR_PAIR(6) | A_BOLD);
    refresh();
    getch();
}

void TUI::run()
{
    load_directory();

    int ch;
    while (true) {
        draw();
        ch = getch();
        if (ch == 'q' || ch == 'Q') {
            break;
        }
        handleinput(ch);
    }
}
