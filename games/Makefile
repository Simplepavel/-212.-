CXX = g++
TARGET = durak
SOURCES = main.cpp durak.cpp general/deck.cpp general/player.cpp general/card.cpp

$(TARGET): $(SOURCES)
	$(CXX) -o $(TARGET) $(SOURCES)

run: $(TARGET)
	./$(TARGET)


.PHONY: run