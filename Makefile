#######################################
##
## Makefile
## LINUX compilation 
##
##############################################

# FLAGS
C++FLAG = -g -std=c++11

# Libraries
MATH_LIBS = -lm

# Directories
EXEC_DIR=.

# Object file rule
.cc.o:
	g++ $(C++FLAG) $(INCLUDES)  -c $< -o $@

# Include directories
INCLUDES= -I. -I/usr/local/include/opencv4

# Libraries
LIBS_ALL = -L/usr/lib -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs

# Program specifics
Cpp_OBJ_P1=ClockDetection.o main.o
PROGRAM_1=main

# Build program
$(PROGRAM_1): $(Cpp_OBJ_P1)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ_P1) $(INCLUDES) $(LIBS_ALL)

# Default target
all: 
	make $(PROGRAM_1)

# Clean
clean:
	(rm -f *.o;rm main)
