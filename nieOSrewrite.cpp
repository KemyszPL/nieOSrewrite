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

/* possible editor arguments:
 * 0 - open editor with no file
 * 1 - tries to open dir.com
 * 2 - opens aliases.dat
 */

void editor(int fileToOpen) {
    WINDOW * editborder = newwin(21, 80, 0, 0);
    WINDOW * editcomline = newwin(3, 80, 21, 0);
    WINDOW * editcontent = newwin(22, 78, 1, 1);
    box(editborder, 0, 0);             /* does the setup for basically a program withn a program (wait... nieOS *is* a program withn a program... program withn a program-ception!)*/
    wrefresh(editborder);
    char* editcom = new char[160];
    scrollok(editcomline, 1);
    idlok(editcomline, 1);
    wmove(editcomline, 0, 0);
    switch (fileToOpen) {
        case 1:
            mvwprintw(editborder, 0, 25, "nieOS editor - dir.com (read only)");
            mvwprintw(editcontent, 0, 0, "Sȼ:࣎Vރش񑨐nޭ�뎬h솁򂴜탕ҕj𛁵𝟥ֈkߞ[񸵦�@6򣯏񭿪۫0\nԠّU𬣩k7􀥇뺜ռ动sϓpX߲{-⮧򣹉񀘧Ô�ǋ񧇼񞔆d󊓞F𺿎ƂЩ󈚱뢨ÂoRP񙢱*mꜸ􏥔ᆪ4ԉ󱹡ϼᮻ񪁾k혂뵧㱉f⿡ؤǖś䯵>񱈩Ș򥾗\n>򚐠ꉅȢ!骁񔘐-󅓃%E$;ɾ𧶶𥩣xmڤ#𚷗nҤқ馾񣞔ೄ킭򧣅Ѝ\n>ہ򔍷ґ򗵡p{򌶑i!y񥑶以ҷ秛󑛭藂񣛫跠𨀚󆲡Ô󯣺Ƹ͖֡񹍖㥧giѓ\n񪐄􂼬񶵉h󺤚𰄸 _⻏ѯŤ૜UΫq΍􎧢ﺨĤ들1󑡃󡢒{k⨗󧼻􃤐򬗮XoE󴖿˝􄯶ƭԃ-X񕑎̅䷞𽧳ᔞ9І^U窨栉®Ɉ䇯óƺށΩ񑙗∅+󛧍򣏬M𶏷ʏ穋I񄫫Ϗ騝ڒ򏄯k㹿p貞O񂮬񯏗%~񵡗@ʷ񿨆򼌒ِZ쫶8ߝ");
            wrefresh(editborder);
            wrefresh(editcontent);
            break;
            
        case 2:
            mvwprintw(editborder, 0, 25, "nieOS editor - aliases.dat (read only)");
            mvwprintw(editcontent, 0, 0, "help=help.com\nedit=edit.com\ndir=dir.com\nls=dir.com");
            wrefresh(editborder);
            wrefresh(editcontent);
            break;

        default:
            mvwprintw(editborder, 0, 25, "nieOS editor - no file (read only)");
            wrefresh(editborder);
            break;
    }
    while(true) {
        wprintw(editcomline, "EDIT: ");
        wrefresh(editcomline);
        wgetstr(editcomline, editcom);
        if (strcmp(editcom, "exit") == 0) {
            wclear(editborder);
            wclear(editcomline);
            wrefresh(editborder);
            wrefresh(editcomline);
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
        mvwprintw(win, 23, 69, "Build 40");
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
        mvwprintw(win, 23, 69, "Build 40");
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
        while (true) {
            char* command = new char[160];
            wprintw(win, "C:/");
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
                wprintw(win, "17:00  28.08.2020    <FOLDER>    OS\n14:03  28.08.2020          1K    exit.com\n14:04  28.08.2020        200K    dir.com\n14:24  28.08.2020          1M    aliases.dat\n17:00  28.08.2020          4M    edit.com\n17:00  28.08.2020        200K    help.com\n");
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
