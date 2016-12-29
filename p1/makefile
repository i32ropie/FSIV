CPPFLAGS = -ggdb3 -Wall -std=c++11
#OPENCVFLAGS = `pkg-config opencv --cflags --libs`
OPENCVFLAGS = -I/usr/local/include/opencv -I/usr/local/include  -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core
BIN = getStats
SOURCE = getStats.cpp
HEADERS = stats.hpp getStats.hpp

$(BIN): $(SOURCE) $(HEADERS)
	g++ $(CPPFLAGS) $(OPENCVFLAGS) $(SOURCE) -o $(BIN)

clean:
	@rm -rf $(BIN)
