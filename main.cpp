#include "file_manager.hpp"
#include "tui.h"
#include <iostream>
#include <string>

int main(){
    FileManager fm;
    TUI interface(fm);
    interface.run();
    return 0;
}