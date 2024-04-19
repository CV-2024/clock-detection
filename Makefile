########################################
##
## Makefile
## LINUX compilation 
##
##############################################





#FLAGS
# C++FLAG = -g -std=c++17
C++FLAG = -g -std=c++11


MATH_LIBS = -lm

EXEC_DIR=.


.cc.o:
	g++ $(C++FLAG) $(INCLUDES)  -c $< -o $@


#Including
INCLUDES=  -I. 

#-->All libraries (without LEDA)
LIBS_ALL =  -L/usr/lib -L/usr/local/lib 


#First Program (ListTest)
Cpp_OBJ_P1=image.o main.o
PROGRAM_1=main


$(PROGRAM_1): $(Cpp_OBJ_P1)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ_P1) $(INCLUDES) $(LIBS_ALL)

all: 
	make $(PROGRAM_1)

clean:
	(rm -f *.o;rm main)

(:
