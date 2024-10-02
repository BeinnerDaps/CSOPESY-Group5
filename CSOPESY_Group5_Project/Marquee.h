#ifndef MARQUEE_H
#define MARQUEE_H

#include <iostream>
#include <string>
using namespace std;

// Citation: https://stackoverflow.com/questions/61514117/i-made-a-moving-screensaver-in-c-console-but-there-is-a-bug-when-it-hits-the-c

class Marquee {

private:
    int width = 100;
    int height = 30;

    int Xpos = rand() % width + 1;
    int Ypos = rand() % height + 1;

    enum Horizontal { LEFT, RIGHT } curX = LEFT;
    enum Vertical { UP, DOWN } curY = UP;
    
    int prevX = 0;
    int prevY = 0;
 
public:

    Marquee() {}

    // Method for writing marquee on screen 
    void writeMarquee() const;

    // Method for moving marquee position
    void moveMarquee();

    // Method for changing marquee text
    void editMarquee();

    void getTerminalSize(int& height, int& width);
};

#endif

