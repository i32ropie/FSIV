CPPFLAGS = -ggdb3 -Wall -Wextra -std=c++11
OPENCVFLAGS = `pkg-config opencv --cflags --libs`
BIN = getStats
# OJECT = getStats.o
SOURCE = getStats.cpp
HEADERS = stats.hpp getStats.hpp

$(BIN): $(SOURCE) $(HEADERS)
	g++ $(CPPFLAGS) $(OPENCVFLAGS) $(SOURCE) -o $(BIN)

clean:
	@rm -rf $(BIN)
