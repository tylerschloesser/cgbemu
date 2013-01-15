cmake -H. -Bbuild -G"MSYS Makefiles" -DCMAKE_C_FLAGS="-mwindows"
make --directory=build