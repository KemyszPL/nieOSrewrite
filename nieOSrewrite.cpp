//
//  main.cpp
//  nieOSrewrite
//
//  Created by KemyszPL on 25/08/2020.
//  Some code was taken from Stack Overflow.
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

/* possible editor arguments:
 * 0 - open editor with no file
 * 1 - tries to open dir.com
 * 2 - opens aliases.dat
 * 3 - tries to open exit.com
 * 4 - tries to open edit.com
 * 5 - tries to open help.com
 * 6 - tries to open CDPlay.prg
 */

void editor(int fileToOpen) {
    WINDOW * editborder = newwin(21, 80, 0, 0);
    WINDOW * editcomline = newwin(3, 80, 21, 0);
    WINDOW * editcontent = newwin(22, 78, 1, 1);
    WINDOW * editerror = newwin(5, 43, 6, 20);
    box(editborder, 0, 0);             /* does the setup for basically a program withn a program (wait... nieOS *is* a program withn a program... program withn a program-ception!)*/
    wrefresh(editborder);
    char* editcom = new char[160];
    scrollok(editcomline, 1);
    idlok(editcomline, 1);
    wmove(editcomline, 0, 0);
    switch (fileToOpen) {
        case 1:
            mvwprintw(editborder, 0, 25, "nieOS editor - dir.com (read only)");
            mvwprintw(editcontent, 0, 0, "replace here text");
            wrefresh(editborder);
            wrefresh(editcontent);
            break;
            
        case 2:
            mvwprintw(editborder, 0, 25, "nieOS editor - aliases.dat (read only)");
            mvwprintw(editcontent, 0, 0, "help=help.com\nedit=edit.com\ndir=dir.com\nls=dir.com");
            wrefresh(editborder);
            wrefresh(editcontent);
            break;
            
        case 3:
            mvwprintw(editborder, 0, 25, "nieOS editor - exit.com (read only)");
            mvwprintw(editcontent, 0, 0, "쵤􇬽󐠋ॅ᭧؄竮ᦰ,j");
            wrefresh(editborder);
            wrefresh(editcontent);
            break;

        case 4:
            mvwprintw(editborder, 0, 25, "nieOS editor - edit.com (read only)");
            wrefresh(editborder);
            mvwprintw(editcontent, 0, 0, "©ńś©∑ęß†ß€į∑†®ęńś©∂ń©√©ś∂©ńś©∑Ō^ęńńŌ^ę∑ńń¨^∂ńś¨^ńńąś¨∂ńń¨Ō€ßń¨Ľ®Ńń€™Ľ∑Ōń∂¨Ľąń∂•    ńŌęń™Ń•Ľ®•∑ę∂śń^∂śąńńąśń∑^ęń∑ĶśžĘ„ŤžŃ„ĘŤŠŃ—ő£ŘŤŕÓ›ĘÁŕŘő„ľű žÓŤŽŚűžŃĄ‚ťÔŚń∂ąęś¨†ļń∑īęļ®īű‹Á¶™ļőÁ„£ŗĄŚžÓŚĄžĘÔŚųžŪťĄŚŽŪÓŽžŪūŚ«žÓŪūÓžŽĄūŚŪťÔ£„ĘŚÓ™Óő„ŚŤŻŹŃűžŤŃŽŚŤŃžťŚŽŃžŤ„Ń‹Ť£žŃ„ĘŚŤŠŽŪŗÁžśÓÁŤŪÁŤŪžŗŠŘÁŪÁłŪĘśŘŗ‚ÁŪłťÓGY)@*3gpyegy70tf&TY©¶†ńĽ¶†©ľ¶§ąńńąś∂©ń∑€ß¨ńęń¨Ľęń∂ś¨©ńś¨ń©ń¨∑ńęń¨Ľń∑ę¨Ľ®ń™€¨Ľ∑ńń∂¨Ľśńń¨Ľś∂ńń¨ąń∑¨Ľń€ę¨ńńśą¨Ľ∂ńń¨Ľń©ń¨Ľęń®¨©ń€™¨Ľßń®¨ńś∂©ĽńśĽ¨©ń¨ę∑Ľ®ń†¨ĽŌ€ńß®¨Ľń∂śąśńńĽ¨ń∂Ż©ńśęó∆†ń€ß^†ń®∑ń^∂śń©ľ^€ßĶń†^®ńĽś∂ń^ń∆¨ķ¨ī√ń†¨©īŻ∂ķ∆ŻķŌą^∆ķ¨ķ©ey89ļ©īĽ¶©•†ļńń®į∂∑įśßęr5tugiybhzjnīĽ©īĽ¶©Ľ†ń¶į®∂ßŌ");
            wrefresh(editcontent);
            box(editerror, 0, 0);
            mvwprintw(editerror, 2, 2, "The selected file is too big to display.");
            wrefresh(editerror);
            break;
            
        case 5:
            mvwprintw(editborder, 0, 25, "nieOS editor - help.com (read only");
            mvwprintw(editcontent, 0, 0, "©¨¨ńńś¨ś¨ą¨ķľ¨•ļĽ©Ľ¶ńŌ•†ń®§ń©ąī©īļīļąīļ∂ļķ¨∂ļąķ¨óļ∂w∑†Ōß®Ō™ß€®ę∑ľ∆∂ćļ");
            wrefresh(editborder);
            wrefresh(editcontent);
            break;
            
        case 6:
            mvwprintw(editborder, 0, 25, "nieOS editor - CDPlay.prg (read only)");
            mvwprintw(editcontent, 0, 0, "©ś©Żńę®Ļ©ń¨Ļ∑ąśńžŽžłĪŚŽńÓžŽĪÓ—ŽžýÝýÝŽųžÓųĘ„‚ťžŃ‹°ŤŘŕ‹ÓŤ°Řž◊ŃĆŹ◊ĆŹÚŃ◊ŁĆŃŚĄŃť„ŃőŤŃ„ťžÔŽŚłłŽŚ”»’ž”ŽŚžŽ—ÝžÝŽŚŁ");
            wrefresh(editborder);
            wrefresh(editcontent);
            break;
            
        default:
            mvwprintw(editborder, 0, 25, "nieOS editor - no file (read only)");
            wrefresh(editborder);
            break;
    }
    wprintw(editcomline, "nieOS editor, type help for a list of commands.\n");
    while(true) {
        wprintw(editcomline, "EDIT: ");
        wrefresh(editcomline);
        wgetstr(editcomline, editcom);
        if (strcmp(editcom, "exit") == 0) {
            wclear(editborder);
            wclear(editcomline);
            wclear(editcontent);
            wclear(editerror);
            wrefresh(editborder);
            wrefresh(editcomline);
            wrefresh(editcontent);
            wrefresh(editerror);
            break;
        }
        if (strcmp(editcom, "help") == 0) {
            wprintw(editcomline, "Available commands: exit\n");
        }
        else {
            wprintw(editcomline, "Unknown command.\n");
        }
    }
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
        mvwprintw(win, 23, 69, "Build 64");
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
        mvwprintw(win, 23, 69, "Build 64");
        wrefresh(loadbox);
        wrefresh(win);
        move(0, 0);
        switch (login()) {
            case 1:
                std::exit(0);
                break;
            
            default:
                break;
        }
        wmove(win, 0, 0);
        scrollok(win, 1);
        idlok(win, 1);
        curs_set(1);
        wprintw(win, "Welcome to nieOS!\nPlease submit a bug on GitHub if you notice any.\n");
        while (true) {
            char* command = new char[160];
            std::string commandstr(command);
            wprintw(win, "sda1/>");
            wrefresh(win);
            wgetstr(win, command);
            // help and the subcommands
            if (strcmp(command, "help") == 0 || strcmp(command, "help.com") == 0) {
                wprintw(win, "exit  help  dir  ls  edit  cd\n");
            }
            else if (strcmp(command, "help exit") == 0 || strcmp(command, "help.com exit") == 0) {
                wprintw(win, "Exits the 'system'");
            }
            else if (strcmp(command, "help help") == 0 || strcmp(command, "help.com help") == 0) {
                wprintw(win, "Helps you by displaying the available commands or the usage of a command.");
            }
            else if (strcmp(command, "exit") == 0 || strcmp(command, "exit.com") == 0) {
                std::exit(0);
            }
            else if (strcmp(command, "dir") == 0 || strcmp(command, "ls") == 0 || strcmp(command, "dir.com") == 0 || strcmp(command, "ls.com") == 0 ) {
                wprintw(win, "17:00  28.08.2020    <FOLDER>    OS\n14:03  28.08.2020          1K    exit.com\n14:04  28.08.2020        200K    dir.com\n14:24  28.08.2020          1M    aliases.dat\n17:00  28.08.2020          4M    edit.com\n17:00  28.08.2020        200K    help.com\n17:00  28.08.2020         10M    CDPlay.prg\n");
            }
            else if (strcmp(command, "CDPlay.prg") == 0 || strcmp(command, "CDPlay") == 0 || strcmp(command, "cdplay.prg") == 0 || strcmp(command, "cdplay") == 0) {
                wprintw(win, "No capable CD drives were detected.\n");
                sleep(1000);
                wrefresh(win);
            }
            else if (strcmp(command, "cee dee playe") == 0) {
                wprintw(win, "noe capabel cee dee draives wear detected!!!!!!!\n");
                sleep(1000);
                wrefresh(win);
            }
            // edit and the files it can open
            else if (strcmp(command, "edit") == 0 || strcmp(command, "edit.com") == 0) {
                editor(0);
                wclear(win);
            }
            else if (strcmp(command, "edit dir.com") == 0 || strcmp(command, "edit.com dir.com") == 0) {
                editor(1);
            }
            else if (strcmp(command, "edit aliases.dat") == 0 || strcmp(command, "edit.com aliases.dat") == 0) {
                editor(2);
            }
            else if (strcmp(command, "edit exit.com") == 0 || strcmp(command, "edit.com exit.com") == 0) {
                editor(3);
            }
            else if (strcmp(command, "edit edit.com") == 0 || strcmp(command, "edit.com edit.com") == 0) {
                editor(4);
            }
            else if (strcmp(command, "edit help.com") == 0 || strcmp(command, "edit.com help.com") == 0) {
                editor(5);
            }
            else if (strcmp(command, "edit CDPlay.prg") == 0 || strcmp(command, "edit.com CDPlay.prg") == 0) {
                editor(6);
            }
            // cd and the subcommands
            else if (strcmp(command, "cd") == 0 || strcmp(command, "cd.com") == 0) {
                wprintw(win, "Usage: cd <directory name>\n");
            }
            else if (strcmp(command, "cd OS") == 0 || strcmp(command, "cd.com OS") == 0) {
                wprintw(win, "Access denied.\n");
            }
            else {
                wprintw(win, "Command not found\n");
            }
        }
        getch();
    }
    return 0;
}
