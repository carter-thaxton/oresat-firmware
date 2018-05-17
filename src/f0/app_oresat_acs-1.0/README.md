# ACS Doxygen test

This is a test of the Doxygen automatic documentation generator.
Everything in the README.md will show up on GitHub, and will also
be used as the main page of the Doxygen documentation HTML files.

---------------------
## How to update documentation

run this command from this directory:
```
doxygen Doxyfile
```


---------------------
## Here is some other relevant documentation.

If building for a specific board, update the `BOARD =` line to the board
defined in the `boards` directory

Then, open the Makefile and edit the line `PROJECT =` to whatever name you chose:
```
PROJECT   = app_<name>
```

Commands for interacting with the can bus
```
sudo slcand -o -c -f -s6 /dev/serial/by-id/*CANtact*-if00
sudo ip link set slcan0 up
candump slcan0
```
