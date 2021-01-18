#Utitlizing the demo Makefiles as presented in class
EXENAME = project
TEST = test
OBJS = main.o Project.o Image.o PNG.o HSLAPixel.o lodepng.o graph.o 

CXX = clang++
CXXFLAGS = $(CS225) -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libc++ -lc++abi -lm

all : $(EXENAME)

$(EXENAME) : output_msg $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)

main.o : main.cpp Project.h ImageData/Image.h cs225/PNG.h cs225/HSLAPixel.h Graph/graph.h Graph/edge.h
	$(CXX) $(CXXFLAGS) main.cpp

Project.o : Project.h Project.cpp
	$(CXX) $(CXXFLAGS) Project.cpp

graph.o : Graph/graph.h Graph/graph.cpp
	$(CXX) $(CXXFLAGS) Graph/graph.cpp

Image.o : ImageData/Image.h ImageData/Image.cpp
	$(CXX) $(CXXFLAGS) ImageData/Image.cpp

PNG.o : cs225/PNG.cpp cs225/PNG.h cs225/HSLAPixel.h cs225/lodepng/lodepng.h
	$(CXX) $(CXXFLAGS) cs225/PNG.cpp

HSLAPixel.o : cs225/HSLAPixel.h  cs225/HSLAPixel.cpp
	$(CXX) $(CXXFLAGS) cs225/HSLAPixel.cpp

lodepng.o : cs225/lodepng/lodepng.h cs225/lodepng/lodepng.cpp
	$(CXX) $(CXXFLAGS) cs225/lodepng/lodepng.cpp

# Custom Clang version enforcement logic:
ccred=$(shell echo -e "\033[0;31m")
ccyellow=$(shell echo -e "\033[0;33m")
ccend=$(shell echo -e "\033[0m")

IS_EWS=$(shell hostname | grep "ews.illinois.edu") 
IS_CORRECT_CLANG=$(shell clang -v 2>&1 | grep "version 6")
ifneq ($(strip $(IS_EWS)),)
ifeq ($(strip $(IS_CORRECT_CLANG)),)
CLANG_VERSION_MSG = $(error $(ccred) On EWS, please run 'module load llvm/6.0.1' first when running CS225 assignments. $(ccend))
endif
else

CLANG_VERSION_MSG = $(warning $(ccyellow) Looks like you are not on EWS. Be sure to test on EWS before the deadline. $(ccend))
endif

output_msg : ; $(CLANG_VERSION_MSG)

test: output_msg test.o PNG.o HSLAPixel.o lodepng.o Project.o graph.o Image.o
	$(LD) tests.o PNG.o HSLAPixel.o lodepng.o Project.o graph.o Image.o $(LDFLAGS) -o test

test.o : tests/tests.cpp cs225/catch/catch.hpp Project.h ImageData/Image.h Graph/graph.h Graph/edge.h cs225/PNG.h cs225/HSLAPixel.h
	$(CXX) $(CXXFLAGS) tests/tests.cpp

clean :
	-rm -f *.o $(EXENAME) test output-worldmap.png

.PHONY: all test clean output_msg
