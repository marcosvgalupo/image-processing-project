CC=gcc
#CCFLAGS=-Wall
CCFLAGS=-g
LDFLAGS=-lm 
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
TARGET=main

ifeq ($(OS), Windows_NT)
   LIB_DIR = ..\\utils
   LIB = $(LIB_DIR)\\imagelib.o
else
   LIB_DIR = ../utils
   LIB = $(LIB_DIR)/imagelib.o
endif

all: $(LIB) $(TARGET)

$(TARGET): $(LIB) $(OBJECTS)
	$(CC) $(CCFLAGS) -o $@ $(OBJECTS) $(LIB) $(LDFLAGS)

$(LIB): $(LIB_DIR)/imagelib.c
	$(CC) $(CCFLAGS) -c -o $@ $< 

%.o: %.c %.h;\
    $(CC) $(CCFLAGS) -c $<

%.o: %.c;\
    $(CC) $(CCFLAGS) -c $<

clean:;\
    rm -f *.o *-result.p*m $(TARGET);\
    rm -f ../testes/*-result.p*m;\
    rm -f $(LIB)
