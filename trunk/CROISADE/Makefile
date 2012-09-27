#PROFILE = -g -pg
PROFILE = -g 
CXXFLAGS = $(PROFILE)	-O3 -Wall -Wextra -fmessage-length=0 -std=c++0x  -Iinc/ 

INCS = $(wildcard inc/*.h)

MAIN = croisade.cpp

SRC = $(wildcard src/*.cpp)

OBJS = $(patsubst src/%.cpp,obj/%.o,$(SRC))

LIBS =

TARGET = croisade

$(TARGET): $(MAIN) $(OBJS) $(INCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS) $(MAIN)

$(OBJS): obj/%.o : inc/%.h src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/$*.cpp 

obj/%.o:  
	echo $<

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
