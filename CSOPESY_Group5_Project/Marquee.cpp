// MODEL - MARQUEE

#include "Marquee.h"

void Marquee::start() {
    thread marqueeThread(&Marquee::marqueeLoop, this);
    thread inputThread(&Marquee::editMarquee, this);

    marqueeThread.join();
    inputThread.join();
}

void Marquee::marqueeLoop() {
    while (run) {
        moveMarquee();
        writeMarquee();
        this_thread::sleep_for(chrono::milliseconds(refreshRate));
    }
}


// Method for writing marquee on screen 
void Marquee::writeMarquee() {
    lock_guard<mutex> lock(textMutex);

    ostringstream buffer;
    clearScreen();

    buffer << martitle << endl;
    for (int y = 0; y < height + 2; y++) {
        for (int x = 0; x < width + 2; x++) {
            if ((y == 0 && x == 0) || (y == 0 && x == width + 1) || (y == height + 1 && x == 0) || (y == height + 1 && x == width + 1)) { buffer << "+"; }
            else if (y == 0 || y == height + 1) { buffer << "-"; }
            else if (x == 0 || x == width + 1) { buffer << "|"; }
            else if (x == Xpos && y == Ypos) { x += (int)(text.length()) - 1; buffer << text; }
            else { buffer << " "; }
        }
        buffer << endl;
    }
    buffer << "\nEnter new marquee text (type 'exit' to stop): " << pollinput;
    cout << buffer.str();
}


// Method for moving marquee position
void Marquee::moveMarquee() {
    if (Xpos == 1 || Xpos == width - ((int)(text.length()) - 1)) { curX = (prevX == RIGHT) ? LEFT : RIGHT; } // 0 is leftward direction
    if (Ypos == 1 || Ypos == height) { curY = (prevY == DOWN) ? UP : DOWN; } // 0 is upward direction
    
    Xpos = (curX == LEFT) ? Xpos - 1 : Xpos + 1;
    Ypos = (curY == UP) ? Ypos - 1 : Ypos + 1;

    prevX = curX;
    prevY = curY;
}


// Method for changing marquee text
void Marquee::editMarquee() { 
    char key;
    while (run) {
        key = _getch();

        lock_guard<mutex> lock(textMutex);

        switch (key) {
            case '\b':  // Backspace Key
                if (!pollinput.empty()) { pollinput.pop_back(); }
                break;
            case ' ':   // Space Key
                pollinput += ' ';
                break;
            case '\r':  // Enter Key
                if (pollinput == "exit") { run = false; menuView();} else { text = pollinput; }
                pollinput = "";
                break;
            default:
                pollinput += key;
                break;
        }
        this_thread::sleep_for(chrono::milliseconds(pollRate));
    }
}