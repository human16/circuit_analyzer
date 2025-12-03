CXX = g++

CXXFLAGS = -Wall -std=c++11
SRCS = simulator.cpp testing.cpp

OBJS = simulator.o testing.o

testing: $(OJBS)
	$(CXX) $(CXXFLAGS) $< -o $@ 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

testing.o: simulator.hpp

TARGET_DEL = testing simulator.o testing.o

clean:
	rm -f $(TARGET_DEL) $(OBJS)