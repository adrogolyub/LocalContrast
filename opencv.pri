INCLUDEPATH *= $(OPENCV_DIR)/build/include
OPENCV_VERSION = 249
PLATFORM = x64

LIBS *= -L$(OPENCV_DIR)/build/$$PLATFORM/vc11/lib

CONFIG(debug, debug|release) {
    LIBS *= -lopencv_core$${OPENCV_VERSION}d -lopencv_highgui$${OPENCV_VERSION}d -lopencv_imgproc$${OPENCV_VERSION}d
} else {
    LIBS *= -lopencv_core$$OPENCV_VERSION -lopencv_highgui$$OPENCV_VERSION -lopencv_imgproc$$OPENCV_VERSION
}
