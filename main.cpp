#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include "file_manager.hpp"
#endif

#ifndef TUI_H
#define TUI_H
#include "tui.h"
#endif

int main()
{
    FileManager fm  = {.current_path = fs::current_path()};
    TUI         tui = {.file_manager = fm};
    tui.init();
    tui.run();
    tui.deinit();
    return 0;
}
