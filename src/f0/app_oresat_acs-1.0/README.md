# ACS Doxygen test

This is a test of the Doxygen automatic documentation generator.

---------------------
Here is some random documentation

blarg
blarg2

---------------------
Here is some relevant documentation.

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
