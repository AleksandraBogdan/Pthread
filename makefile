COMPILER=g++ -std=c++0x
CFLAGS=-Wall -Werror
EXECUTABLE=lab3

SOURCES=$(shell find . -name "*.cpp")

all: $(SOURCES)
	$(COMPILER) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE) -lpthread
clean:
	rm -f *.o $(EXECUTABLE)
