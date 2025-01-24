# Makefile for compiling web server

CXX = g++

CXXFLAGS = -Wall -Wextra -Werror

TARGET = web_server

SRC = web_server.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)