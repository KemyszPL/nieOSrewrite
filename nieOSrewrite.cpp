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
        WINDOW * loadbox = newwin(3, 10, 12, 19); // this window exists ONLY for the loading border
        refresh();
        wrefresh(win);
        wrefresh(loadbox);
        mvwprintw(win, 9, 33, "nieOS");
        wrefresh(win);
        box(loadbox, 0, 0);
        wrefresh(loadbox);
        getch();
    }
    return 0;
}
