PROGNAME = starter

a.exe: ${PROGNAME}.cpp
	E:/mingw/bin/g++ -Wall -std=c++11 -o a.exe -g -I E:/mingw/include ${PROGNAME}.cpp


run: a.exe
	.\a.exe

clean:
	del /Q a.exe
