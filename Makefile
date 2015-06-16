#-------------------------------
# Makefile for HSAJVM (g++)
#-------------------------------

CC = g++
CFLAGS = -static-libgcc -static-libstdc++
TARGET = hsajvm.exe

SRC = hsajvm.cpp javaclass.cpp
OBJ = ${SRC:.cpp=.o}

.PHONY: clean

all: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(TARGET)
	
%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	del *.o
	del $(TARGET)