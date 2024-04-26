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
INCLUDES= -I. -I/usr/local/include/opencv4
#INCLUDES = -I. -I/mnt/c/Users/drewr/Desktop/clock-detection/usr/share/opencv4/include
# INCLUDES = -I. -I/usr/include/opencv4

# Libraries
LIBS_ALL = -L/usr/lib -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs


# Program specifics
Cpp_OBJ_P1=ClockDetection.o p1.o
PROGRAM_1=p1


# Build program
$(PROGRAM_1): $(Cpp_OBJ_P1)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ_P1) $(INCLUDES) $(LIBS_ALL)

# Default target
all: 
	make $(PROGRAM_1)

# Clean
clean:
	(rm -f *.o;rm p1)


# ##MAKEFILE DREW USES ON WINDOWS#####
# # Compiler and flags
# CXX = g++
# CXXFLAGS = -g -std=c++11

# # Directories
# SRC_DIR = .
# BUILD_DIR = build
# EXEC_DIR = .

# # Include directories
# INCLUDES = -I$(SRC_DIR) -I/home/drewr/Desktop/opencv/include/opencv4

# # Libraries
# LIBS_ALL = -LG:/opencv/x64/vc16/lib \
#            -lopencv_core -lopencv_imgproc -lopencv_highgui \
#            -lopencv_imgcodecs

# # Program specifics
# CPP_SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
# OBJECTS = $(CPP_SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
# PROGRAM = main

# # Build directory
# $(shell mkdir -p $(BUILD_DIR))

# # Object file rule
# $(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
# 	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# # Build program
# $(EXEC_DIR)/$(PROGRAM): $(OBJECTS)
# 	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS_ALL)

# # Default target
# all: $(EXEC_DIR)/$(PROGRAM)

# # Clean
# clean:
# 	$(RM) $(BUILD_DIR)/*.o $(EXEC_DIR)/$(PROGRAM)
