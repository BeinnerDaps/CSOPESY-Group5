// MODEL - MARQUEE

#include "Marquee.h"


// Method for writing marquee on screen 
void Marquee::writeMarquee() const{
    for (int y = 0; y < height + 2; y++) {
        for (int x = 0; x < width + 2; x++) {
            if (y == 0 || y == height + 1) { cout << "-"; } 
            else if (x == 0 || x == width + 1) { cout << "|"; }
            else if (x == Xpos && y == Ypos) { cout << "0"; }
            else { cout << " "; }
        }
        cout << endl;
    }
}


// Method for moving marquee position
void Marquee::moveMarquee() {
    if (Xpos == 1 || Xpos == width) { curX = (prevX) ? LEFT : RIGHT; } // 0 is leftward direction
    if (Ypos == 1 || Ypos == height) { curY = (prevY) ? UP : DOWN; } // 0 is upward direction
    
    Xpos = (curX == LEFT) ? Xpos - 1 : Xpos + 1;
    Ypos = (curY == UP) ? Ypos - 1 : Ypos + 1;

    prevX = curX;
    prevY = curY;
}


// Method for changing marquee text
void Marquee::editMarquee() {

}

void Marquee::getTerminalSize(int& rows, int& cols) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int ret = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    if (ret) {
        cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    } else {
        cols = 80;
        rows = 25;
    }
}