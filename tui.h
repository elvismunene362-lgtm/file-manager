#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include "file_manager.hpp"
#endif

#include <ncurses.h>
#include <string>
#include <vector>

struct TUI {
    FileManager&             file_manager;
    std::vector<std::string> entries;
    int                      selected;
    int                      scroll_offset;

    void init();
    void deinit();
    void run();
    void initcolors();
    void load_directory();
    void draw();
    void handleinput(int ch);
    void encryptselected();
    void decrypt_selected();
    void showmessage(const std::string& msg);
};
