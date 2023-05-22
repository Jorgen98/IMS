#
# Projekt: IMS
# Autor:   Pavol Dubovec, xdubov02
# Autor:   Juraj Lazúr, xlazur00
# Datum:   6.12.2020
# 
# Použitie:
#	- preklad:	make

CFLAGS++=-std=c++11 -pedantic -Wall -Wextra
make: make_o_c make_o_g make_final clear

make_o_c: cell.cpp cell.h
	g++ $(CFLAGS++) -c cell.cpp -o cell.o
make_o_g: grassland.cpp
	g++ $(CFLAGS++) -c grassland.cpp -o grassland.o
make_final: grassland.o cell.o
	g++ $(CFLAGS++) -c main.cpp -o main.o
	g++ $(CFLAGS++) grassland.o cell.o main.o -o ims
clear:
	rm -f *.o