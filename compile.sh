g++ -c visualiser.cpp
g++ -c main.cpp
g++ main.o visualiser.o -o Visualisation -lGL -lglut -lGLEW -lGLU libfmodex64.so
rm visualiser.o main.o
