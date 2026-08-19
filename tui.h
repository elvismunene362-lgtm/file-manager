#ifndef TUI_H
#define TUI_H

#include "file_manager.hpp"
#include <ncurses.h>
#include <vector>
#include <string>

class TUI{
public:
   TUI(FileManager& fm);
   ~TUI();

   void run();

private:
    FileManager& filemanager;

    std::vector<std::string> entries;
    int selected = 0;
    int scrolloffset = 0;

    void initcolors();
    void loaddirectory();
    void draw();
    void handleinput(int ch);
    void encryptselected();
    void showmessage(const std::string& msg);
};
#endif