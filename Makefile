BIN_PTH := ./bin/
BUILD_PTH := ./build/
DATA_PTH := ./data/

HEADER_PTH := ./include/
SRC_PTH := ./src/

TEST_SRC_PTH := ./test/src/
TEST_GTEST_PTH := ./test/src/gtest/
TEST_GTEST_INT_PTH := ./test/src/gtest/internal/
TEST_BIN_PTH := ./test/bin/

EXEC := agenda

CC := g++ -O3 -std=c++11 -I$(HEADER_PTH)
CCT := g++ -g -std=c++11 -I$(HEADER_PTH) -I$(TEST_SRC_PTH) -I$(TEST_GTEST_PTH) -lgtest -lgtest_main -lpthread

build : $(BUILD_PTH)Agenda.o $(BUILD_PTH)AgendaUI.o $(BUILD_PTH)AgendaService.o $(BUILD_PTH)Storage.o $(BUILD_PTH)Meeting.o $(BUILD_PTH)Date.o $(BUILD_PTH)User.o
	@mkdir -p $(BIN_PTH)
	@mkdir -p $(BUILD_PTH)
	$(CC) -o $(BIN_PTH)$(EXEC) $(BUILD_PTH)Agenda.o $(BUILD_PTH)AgendaUI.o $(BUILD_PTH)AgendaService.o $(BUILD_PTH)Storage.o $(BUILD_PTH)Meeting.o $(BUILD_PTH)Date.o $(BUILD_PTH)User.o

$(BUILD_PTH)Agenda.o : $(SRC_PTH)Agenda.cpp $(BUILD_PTH)AgendaUI.o
	@mkdir -p $(BUILD_PTH); 
	$(CC) -c -o $@ $<

$(BUILD_PTH)AgendaUI.o : $(SRC_PTH)AgendaUI.cpp $(HEADER_PTH)AgendaUI.hpp $(BUILD_PTH)AgendaService.o
	@mkdir -p $(BUILD_PTH); 
	$(CC) -c -o $@ $<

$(BUILD_PTH)AgendaService.o : $(SRC_PTH)AgendaService.cpp $(HEADER_PTH)AgendaService.hpp $(BUILD_PTH)Storage.o
	@mkdir -p $(BUILD_PTH); 
	$(CC) -c -o $@ $<

$(BUILD_PTH)Storage.o : $(SRC_PTH)Storage.cpp $(HEADER_PTH)Storage.hpp $(BUILD_PTH)Meeting.o $(BUILD_PTH)User.o
	@mkdir -p $(BUILD_PTH); 
	$(CC) -c -o $@ $<

$(BUILD_PTH)Meeting.o : $(SRC_PTH)Meeting.cpp $(HEADER_PTH)Meeting.hpp $(HEADER_PTH)Path.hpp $(BUILD_PTH)Date.o
	@mkdir -p $(BUILD_PTH); 
	$(CC) -c -o $@ $<

$(BUILD_PTH)Date.o : $(SRC_PTH)Date.cpp $(HEADER_PTH)Date.hpp
	@mkdir -p $(BUILD_PTH); 
	$(CC) -c -o $@ $<

$(BUILD_PTH)User.o : $(SRC_PTH)User.cpp $(HEADER_PTH)User.hpp
	@mkdir -p $(BUILD_PTH); 
	$(CC) -c -o $@ $<

run :
	@$(BIN_PTH)$(EXEC)

lint : 
	cpplint --filter=-legal/copyright,-build/header_guard,-build/include,-readability/streams src/*.hpp src/*.cpp

clean :
	rm -f ./bin/*
	rm -f ./build/*
	rm -f ./test/bin/*

test-user : $(SRC_PTH)User.cpp $(HEADER_PTH)User.hpp $(TEST_SRC_PTH)UserTest.cpp
	$(CCT) -o $(TEST_BIN_PTH)test-user $(SRC_PTH)User.cpp $(TEST_SRC_PTH)UserTest.cpp

test-date : $(SRC_PTH)Date.cpp $(HEADER_PTH)Date.hpp $(TEST_SRC_PTH)DateTest.cpp
	$(CCT) -o $(TEST_BIN_PTH)test-date $(SRC_PTH)Date.cpp $(TEST_SRC_PTH)DateTest.cpp

test-meeting : $(SRC_PTH)Meeting.cpp $(HEADER_PTH)Meeting.hpp $(SRC_PTH)Date.cpp $(HEADER_PTH)Date.hpp $(SRC_PTH)User.cpp $(HEADER_PTH)User.hpp $(HEADER_PTH)Path.hpp $(TEST_SRC_PTH)MeetingTest.cpp
	$(CCT) -o $(TEST_BIN_PTH)test-meeting $(SRC_PTH)Meeting.cpp $(SRC_PTH)Date.cpp $(SRC_PTH)User.cpp $(TEST_SRC_PTH)MeetingTest.cpp

test-storage : $(SRC_PTH)Storage.cpp $(HEADER_PTH)Storage.hpp $(SRC_PTH)Meeting.cpp $(HEADER_PTH)Meeting.hpp $(SRC_PTH)Date.cpp $(HEADER_PTH)Date.hpp $(SRC_PTH)User.cpp $(HEADER_PTH)User.hpp $(HEADER_PTH)Path.hpp $(TEST_SRC_PTH)StorageTest.cpp
	$(CCT) -o $(TEST_BIN_PTH)test-storage $(SRC_PTH)Storage.cpp $(SRC_PTH)Meeting.cpp $(SRC_PTH)Date.cpp $(SRC_PTH)User.cpp $(TEST_SRC_PTH)StorageTest.cpp $(TEST_SRC_PTH)utility.cpp

test-service : $(SRC_PTH)Storage.cpp $(HEADER_PTH)Storage.hpp $(SRC_PTH)Meeting.cpp $(HEADER_PTH)Meeting.hpp $(SRC_PTH)Date.cpp $(HEADER_PTH)Date.hpp $(SRC_PTH)User.cpp $(HEADER_PTH)User.hpp $(HEADER_PTH)Path.hpp $(TEST_SRC_PTH)AgendaServiceTest.cpp
	$(CCT) -o $(TEST_BIN_PTH)test-service $(SRC_PTH)AgendaService.cpp $(SRC_PTH)Storage.cpp $(SRC_PTH)Meeting.cpp $(SRC_PTH)Date.cpp $(SRC_PTH)User.cpp $(TEST_SRC_PTH)AgendaServiceTest.cpp $(TEST_SRC_PTH)utility.cpp