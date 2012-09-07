gcc src/debug.c src/tools.c src/main.c src/cpu.c src/memory.c src/graphics.c -o main.exe -lmingw32 -lSDLmain -lSDL -mwindows -g -static-libgcc
IF NOT ERRORLEVEL 1 main.exe
PAUSE