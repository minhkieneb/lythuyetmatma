CC = /usr/bin/gcc
ROOTDIR = .
SRCDIR = $(ROOTDIR)/src
GCCFLAGS = -o 
TARGET = ./output/hc128_generators

build:
	$(CC) $(GCCFLAGS) $(TARGET) $(SRCDIR)/main.c

clean:
	rm $(TARGET)