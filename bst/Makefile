CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -g
TARGET = bst

all: $(TARGET)

$(TARGET): $(TARGET).o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TARGET).o: $(TARGET).cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean