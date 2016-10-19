CPPFLAGS = -ggdb3 -Wall -std=c++11
OPENCVFLAGS = `pkg-config opencv --cflags --libs`
BIN = getStats
SOURCE = getStats.cpp
HEADERS = stats.hpp getStats.hpp

$(BIN): $(SOURCE) $(HEADERS)
	g++ $(CPPFLAGS) $(OPENCVFLAGS) $(SOURCE) -o $(BIN)

clean:
	@rm -rf $(BIN)
