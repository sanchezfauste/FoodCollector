# FoodCollector
Implementation of Computer Graphics and Multimedia project.

# Preparing the environment
In order to compile and run this project you need to install some dependences on your machine. Let's see how to prepare a Debian based distribution.

## Install the OpenGL library
You can install the library runing the following commands as `root`:
```
apt-get update
apt-get install freeglut3-dev
```

## Install the JPEG library
You can download this library from: http://www.ijg.org/

Once you downloaded the library extract it, enter to the `jpeg-9b` directory and run the following commands as `root`:
```
./configure
make
make install
ranlib /usr/local/lib/libjpeg.a
ldconfig /usr/local/lib
```

# Compile and execute the game
You can compile the game using the Makefile. You only need to enter to the root directory of the project and run:
```
make
```

To run the game execute the command:
```
./foodcollection <nRows> <nCols>
```
Notice that `nRows` is the number of rows of the map and `nCols` is the number of cols of the map.
