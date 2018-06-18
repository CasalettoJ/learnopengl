BINDIR := bin
DIRS := . systems components
FILES := $(foreach DIR,$(DIRS),$(wildcard $(DIR)/*.cpp $(DIR)/*/*.cpp))

build:
	mkdir -p $(BINDIR);
	g++ -Wall -Wextra -pedantic -std=c++14 -o $(BINDIR)/OpenGl $(FILES) -lSDL2 -lSDL2_image -lSDL2_ttf -lGLEW -framework OpenGl -O2;
	cp -R assets/ $(BINDIR)/assets/;

clean:
	rm -rf $(BINDIR)/*

.PHONY: clean build
