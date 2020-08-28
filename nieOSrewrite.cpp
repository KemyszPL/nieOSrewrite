//
//  main.cpp
//  nieOSrewrite
//
//  Created by KemyszPL on 25/08/2020.
//

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <sys/select.h>
#include <unistd.h>
#include <errno.h>
#include <chrono>
#include <thread>

bool g_has_terminal = false; // Check this global variable before ncurses calls

bool ensure_debugger_attached_woraround(int timeout_ms)
{
    fd_set fd_stdin;
    FD_ZERO(&fd_stdin);
    FD_SET(STDIN_FILENO, &fd_stdin);
    struct timespec timeout = { timeout_ms / 1000, (timeout_ms % 1000) * 1000000 };

    do
    {
        errno = 0;
    }
    while (pselect(STDIN_FILENO + 1, &fd_stdin, NULL, NULL, &timeout, NULL) < 0 && errno == EINTR);

    if (errno != 0)
    {
        fprintf(stderr, "Unexpected error %d", errno);
        return false;
    }
    return true;
}

void sleep(int time) // defines sleep() command
{
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

int login() {
    curs_set(1); // shows the cursor
    echo(); // makes user input visible
    char* userName = new char[160];
    char* userPassword = new char[160];
    WINDOW * loginwin = newwin(10, 30, 7, 24);
    int logfail(0);
    box(loginwin, 1, 0);
    mvwprintw(loginwin, 3, 5, "User:");
    wrefresh(loginwin);
    mvwgetstr(loginwin, 3, 10, userName);
    curs_set(0);
    if (strcmp(userName, "nie") == 0) {
        curs_set(1);
        mvwprintw(loginwin, 5, 5, "Password:");
        wrefresh(loginwin);
        mvwgetstr(loginwin, 5, 14, userPassword);
        curs_set(0);
        if (strcmp(userPassword, "nie") == 0) {
            mvwprintw(loginwin, 8, 6, "Welcome to nieOS");
            wrefresh(loginwin);
            logfail = 0;
        }
    }
    else {
        mvwprintw(loginwin, 8, 4, "Wrong username/password.");
        wrefresh(loginwin);
        logfail = 1;
    }
    return logfail;
}

void doNothing() {
    
}

int main(int argc, const char * argv[]) {
    // insert code here...
    if (!ensure_debugger_attached_woraround(700))
        return 1;

    char *term = getenv("TERM");

    g_has_terminal = (term != NULL);

    if (g_has_terminal)
        g_has_terminal = (initscr() != NULL);

    // Some ncurses code. Maybe you should terminate if g_has_terminal is not set

    if (g_has_terminal)
    {
        // creates a new window
        int height, width, start_y, start_x;    // sets parameters for the new window
        height = 24;
        width = 80;
        start_y = start_x = 0;
        
        WINDOW * win = newwin(height, width, start_y, start_x);
        WINDOW * loadbox = newwin(3, 40, 12, 19); // this window exists ONLY for the loading border
        refresh();
        wrefresh(win);
        wrefresh(loadbox);
        curs_set(0); // makes the cursor hidden
        mvwprintw(win, 9, 33, "nieOS");
        wrefresh(win);
        box(loadbox, 0, 0);
        mvwprintw(loadbox, 1, 1, "#");
        wrefresh(loadbox);
        sleep(200);
        wprintw(loadbox, "####"); // hooray for fake loading
        wrefresh(loadbox);
        sleep(300);
        wprintw(loadbox, "###");
        wrefresh(loadbox);
        sleep(370);
        wprintw(loadbox, "#####");
        wrefresh(loadbox);
        sleep(140);
        wprintw(loadbox, "#");
        wrefresh(loadbox);
        sleep(574);
        wprintw(loadbox, "######");
        wrefresh(loadbox);
        sleep(300);
        wprintw(loadbox, "##");
        wrefresh(loadbox);
        sleep(300);
        wprintw(loadbox, "#");
        wrefresh(loadbox);
        sleep(200);
        wprintw(loadbox, "####");
        wrefresh(loadbox);
        sleep(123);
        wprintw(loadbox, "###");
        wrefresh(loadbox);
        sleep(100);
        wprintw(loadbox, "#");
        wrefresh(loadbox);
        sleep(100);
        wprintw(loadbox, "#");
        wrefresh(loadbox);
        sleep(100);
        wprintw(loadbox, "#");
        wrefresh(loadbox);
        sleep(100);
        wprintw(loadbox, "#");
        wrefresh(loadbox);
        sleep(100);
        wprintw(loadbox, "#");
        wrefresh(loadbox);
        sleep(100);
        wprintw(loadbox, "#");
        wrefresh(loadbox);
        sleep(100);
        wprintw(loadbox, "#");
        wrefresh(loadbox);
        sleep(100);
        wprintw(loadbox, "#");
        wrefresh(loadbox); // end of fake loading
        sleep(431);
        wclear(loadbox);
        wclear(win);
        wrefresh(loadbox);
        wrefresh(win);
        move(0, 0);
        switch (login()) {
            case 0:
                doNothing();
                break;

            case 1:
                throw std::exception();
                break;
            
            default:
                break;
        }
        getch();
    }
    return 0;
}
