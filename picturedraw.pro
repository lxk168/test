TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
INSTALLS += target
target.path=/userdata/usr/local/

LIBS +=  -lpthread -lsqlite3 -lm -lcurl -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui  -lzmq -lczmq

SOURCES += \
        main.c \
    hcopencv.cpp \
    HCFileOperation.c \
    HCJson.c

HEADERS += \
    common.h \
    HCFileOperation.h \
    hcopencv.h \
    HCJson.h
