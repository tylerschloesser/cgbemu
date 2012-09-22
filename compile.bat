cd %~dp0src
gcc debug.c tools.c main.c cgb/cpu.c cgb/memory.c cgb/graphics.c cgb/gameboy.c cgb/joypad.c -o ../bin/main.exe -lmingw32 -lSDLmain -lSDL -mwindows -g -static-libgcc
IF NOT ERRORLEVEL 1 cd %~dp0bin && main
PAUSE