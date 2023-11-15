CC = /usr/bin/gcc
ROOTDIR = .
SRCDIR = $(ROOTDIR)/src
GCCFLAGS = -o 
TARGET = ./output/hc128_generators
OUTPUT = ./output/data.txt

build:
	$(CC) $(GCCFLAGS) $(TARGET) $(SRCDIR)/main.c

generate: build
	$(TARGET) > $(OUTPUT)

clean:
	rm $(TARGET)