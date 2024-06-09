appname := chess

CXX := g++
CXXFLAGS := -Wall -g

srcfiles := $(shell find . -maxdepth 2 -name "*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(appname)

$(appname): $(objects)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(appname) $(objects) 
	rm -f $(objects)

clean:
	rm -f $(objects)
	rm chess
