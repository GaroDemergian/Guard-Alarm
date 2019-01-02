TARGET = alarm.exe

SRC = src
INC = inc
BIN = bin
OBJ = obj

SOURCE = $(wildcard $(SRC)/*.cpp)
OBJECT = $(patsubst %,$(OBJ)/%, $(notdir $(SOURCE:.cpp=.o)))

CC = g++

RED = \033[1;31m
GREEN = \033[1;32m
BLUE = \033[1;34m
YELLOW = \033[1;33m
NC = \033[1;0m

$(BIN)/$(TARGET) : $(OBJECT)
	@echo "$(RED)Linking...\n$(NC)"
	$(CC) -o $@ $^
	@echo "$(Done)Done!$(NC)"

$(OBJ)/%.o : $(SRC)/%.cpp
	@echo "$(GREEN)Compiling...\n$(NC)"
	$(CC) -I$(INC) -c $< -o $@

.PHONY : help run clean

run : $(BIN)/$(TARGET)
	@echo "$(BLUE)Running...\n$(NC)"
	$(BIN)/$(TARGET)


clean : 
	rm -f $(OBJECT) $(BIN)/$(TARGET)

help :
	@echo "src: $(SOURCE)"
	@echo "obj: $(OBJECT)"