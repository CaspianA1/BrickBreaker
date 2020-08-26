CC = gcc
OUT_NAME = BrickBreaker
FLAGS = -lncurses -o $(OUT_NAME)
all:
	$(CC) $(FLAGS) brick-breaker.c
