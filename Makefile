BIN_PTH := ./bin/
BUILD_PTH := ./build/
DATA_PTH := ./data/

HEADER_PTH := ./include/
SRC_PTH := ./src/

TEST_SRC_PTH := ./test/

EXEC := agenda

CC := g++ -g -std=c++11 -I$(HEADER_PTH)

build : AgendaService.o AgendaUI.o Date.o Meeting.o Path.o Storage.o User.o
	$(CC) -o $(BIN_PTH)$(EXEC) $(BUILD_PTH)AgendaService.o $(BUILD_PTH)AgendaUI.o $(BUILD_PTH)Date.o $(BUILD_PTH)Meeting.o $(BUILD_PTH)Storage.o $(BUILD_PTH)User.o

AgendaService.o : $(SRC_PTH)AgendaService.cpp $(HEADER_PTH)AgendaService.hpp Storage.o
	$(CC) -c -o $(BUILD_PTH)$@ $<

AgendaUI.o : $(SRC_PTH)AgendaUI.cpp $(HEADER_PTH)AgendaUI.hpp AgendaService.o
	$(CC) -c -o $(BUILD_PTH)$@ $<

Date.o : $(SRC_PTH)Date.cpp $(HEADER_PTH)Date.hpp
	$(CC) -c -o $(BUILD_PTH)$@ $<

Meeting.o : $(SRC_PTH)Meeting.cpp $(HEADER_PTH)Meeting.hpp $(HEADER_PTH)Path.hpp Date.o
	$(CC) -c -o $(BUILD_PTH)$@ $<

Storage.o : $(SRC_PTH)Storage.cpp $(HEADER_PTH)Storage.hpp Meeting.o User.o
	$(CC) -c -o $(BUILD_PTH)$@ $<

User.o : $(SRC_PTH)User.cpp $(HEADER_PTH)User.hpp
	$(CC) -c -o $(BUILD_PTH)$@ $<

lint : 
	cpplint --filter=-legal/copyright,-build/header_guard,-build/include,-readability/streams src/*.hpp src/*.cpp

unittest : $(SRC_PTH)Date.cpp $(HEADER_PTH)Date.hpp $(SRC_PTH)Meeting.cpp $(HEADER_PTH)Meeting.hpp $(HEADER_PTH)Path.hpp $(SRC_PTH)Storage.cpp $(HEADER_PTH)Storage.hpp $(SRC_PTH)User.cpp $(HEADER_PTH)User.hpp test/main.cpp
	g++ -g -std=c++11 -o unittest -lgtest -lgtest_main -lpthread -I$(HEADER_PTH) -I./test/gtest -I./test -I./test/gtest/internal $(SRC_PTH)Date.cpp $(SRC_PTH)Meeting.cpp $(SRC_PTH)Storage.cpp $(SRC_PTH)User.cpp test/main.cpp