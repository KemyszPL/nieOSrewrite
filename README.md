# nieOSrewrite

This is a rewrite of nieOS using ncurses and (hopefully) more efficient methods.

Features:
  - The login window
  - "Command interpreter"
  - Iris assistant (Work in progress)
  
Commands:
  - exit
  - help
  - edit
  - dir/ls
  - CDPlay

## Logging in
It might not be obvious at first, but the username and password that you have to enter is "nie".

## Compiling

To compile nieOS you must install `ncurses` and add the `-lncurses` flag to the linker. It also currently uses libraries that make it only work on Linux so there's that. More detailed instructions can be found elsewhere™️.
