 
DEFINES += DCONLIB_LIBRARY

SOURCES += $$PWD/dconlib.cpp

HEADERS += $$PWD/dconlib.h\
        $$PWD/dconlib_global.h

INCLUDEPATH += $$PWD

unix {
    target.path = /usr/lib
    INSTALLS += target
}

greaterThan(QT_MAJOR_VERSION, 4) {
    LIBS += -lQt5SerialPort
} else {
    LIBS += -lqtserialport
}
