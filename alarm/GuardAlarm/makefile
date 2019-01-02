TARGET = alarm.exe

SRC = src
INC = inc
BIN = bin
OBJ = obj

SOURCE = $(wildcard $(SRC)/*.cpp)
OBJECT = $(patsubst %,$(OBJ)/%, $(notdir $(SOURCE:.cpp=.o)))

CC = g++

$(BIN)/$(TARGET) : $(OBJECT)
	$(CC) -o $@ $^

$(OBJ)/%.o : $(SRC)/%.cpp
	$(CC) -I$(INC) -c $< -o $@

.PHONY : run clean

run : $(BIN)/$(TARGET)
	$(BIN)/$(TARGET)

clean : 
	rm -f $(OBJECT) $(BIN)/$(TARGET)