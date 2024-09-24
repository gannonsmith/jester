appname := chess

CXX := g++
CXXFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

srcfiles := $(shell find . -maxdepth 3 -name "*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(appname)

$(appname): $(objects)
	$(CXX) -o $(appname) $(objects) $(CXXFLAGS) 
	rm -f $(objects)

clean:
	rm -f $(objects)
	rm chess
