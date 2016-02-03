TEMPLATE = app

QT += qml quick widgets

CONFIG += c++11

SOURCES += main.cpp \
    uistream.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    uistream.h

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/WEOS/release/ -lWEOS_lib
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/WEOS/debug/ -lWEOS_lib

#INCLUDEPATH += $$PWD/WEOS
#DEPENDPATH += $$PWD/WEOS
