echo "Installing Open GL Dependencies"
sudo apt-get install libglu1-mesa-dev mesa-common-dev freeglut3-dev
echo -e

echo "Compiling visualiser.cpp"
g++ -c visualiser.cpp
echo -e

echo "Compiling main.cpp"
g++ -c main.cpp
echo -e

echo "Creating executable"
g++ main.o visualiser.o -o Visualisation -lGL -lglut -lGLEW -lGLU libfmodex64.so -Wall
echo -e

rm visualiser.o main.o

echo "Run the application using $./Visualisation "
