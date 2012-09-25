CXXFLAGS =	-O3 -Wall -fmessage-length=0 -std=c++0x  -Iinc/ 

OBJS =		croisade.cpp inc/Parameters.h

LIBS =

TARGET =	croisade

$(TARGET):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
