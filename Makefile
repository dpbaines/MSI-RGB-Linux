all:
	gcc rgb_interface.c main.c -O1 -lX11 -lm -o rgb