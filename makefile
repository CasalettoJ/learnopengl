BINDIR := bin

build:
	mkdir -p $(BINDIR);
	g++ -Wall -Wextra -pedantic -std=c++14 -o $(BINDIR)/OpenGl $(wildcard *.cpp) -lSDL2 -lSDL2_image -lSDL2_ttf -lGLEW -framework OpenGl -O2;
	cp -R assets/ $(BINDIR)/assets/;

clean:
	rm -rf $(BINDIR)/

.PHONY: clean build
