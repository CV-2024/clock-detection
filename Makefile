#######################################
##
## Makefile
## LINUX compilation 
##
##############################################

#FLAGS
C++FLAG = -g -std=c++11

# Libraries
MATH_LIBS = -lm

# Directories
EXEC_DIR=.

# Object file rule
.cc.o:
	g++ $(C++FLAG) $(INCLUDES)  -c $< -o $@

# Include directories
#INCLUDES= -I. -I/usr/local/include/opencv4 # EVERYONE ELSE USES THIS ONE
#INCLUDES = -I. -I/mnt/c/Users/drewr/Desktop/clock-detection/usr/share/opencv4/include
 INCLUDES = -I. -I/usr/include/opencv4 # DREW USES THIS ONE

# Libraries
LIBS_ALL = -L/usr/lib -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs


# Program specifics
Cpp_OBJ_P1=ClockDetection.o p1.o
PROGRAM_1=p1

Cpp_OBJ_P2=ClockDetection.o p2.o
PROGRAM_2=p2


# Build program
$(PROGRAM_1): $(Cpp_OBJ_P1)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ_P1) $(INCLUDES) $(LIBS_ALL)

$(PROGRAM_2): $(Cpp_OBJ_P2)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ_P2) $(INCLUDES) $(LIBS_ALL)

# Default target
all: 
	make $(PROGRAM_1)
	make $(PROGRAM_2)

# Clean
clean:
	(rm -f *.o;rm p1; rm p2)



# // download: brew install opencv (for Mac)
# // run:
# // g++ -std=c++11 -I/usr/local/Cellar/opencv/4.9.0_7/include -I/usr/local/Cellar/opencv/4.9.0_7/include/opencv4 main.cc ClockDetection.cc -o main -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc
# // ./main
# //Drew running on Windows with VcXsrv, type "export DISPLAY=localhost:0" no quotes to activate the XLaunche img viewer, then run program as normal: ./main
