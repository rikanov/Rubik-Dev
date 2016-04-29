#ifndef EDITOR_H
#define EDITOR_H

#include "editor_buffer.h"

class Editor
{
private:
    int x, y;
    char mode;
    Buffer* buff;
    String status, filename;

    String tos(int);

    // Cursor movement
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    void deleteLine();                  
    void deleteLine(int);               

    void saveFile();                   

public:
    Editor();                         
    Editor(String);                    

    char getMode() {return mode;}

    void handleInput(int);              
    void printBuff();
    void printStatusLine();             // vim-like status bar
    void updateStatus();                // Updates the status line 
};

#endif
