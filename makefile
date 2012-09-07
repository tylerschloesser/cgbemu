a.out: main.c debug.c memory.c graphics.c cpu.c
	gcc main.c debug.c memory.c graphics.c cpu.c -lSDL -lpthread -ldl -lm
