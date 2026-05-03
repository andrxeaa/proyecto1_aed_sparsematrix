CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -ISFML/include
LDFLAGS = -LSFML/lib -lsfml-graphics -lsfml-window -lsfml-system

SRC = main.cpp Spreadsheet.cpp Node.cpp FormulaEvaluator.cpp GUI.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = spreadsheet_app

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC) $(EXEC).exe
