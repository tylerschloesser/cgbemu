cd %~dp0src
gcc debug.c tools.c main.c cpu.c memory.c graphics.c -o ../bin/main.exe -lmingw32 -lSDLmain -lSDL -mwindows -g -static-libgcc
IF NOT ERRORLEVEL 1 cd %~dp0bin && main
PAUSE