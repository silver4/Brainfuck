CC = gcc
CFLAGS = -O2 -s -fomit-frame-pointer -march=native -Wall -Wextra -ansi -pedantic

brainfuck: src/main.c src/main.h
  gcc -o brainfuck src/main.c $(CFLAGS)
