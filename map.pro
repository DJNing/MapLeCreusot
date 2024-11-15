QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

# disable deprecated APIs before Qt 6.0.0
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    src/SceneBuilder.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mapview.cpp \
    src/modeldata.cpp \
    src/projection.cpp \
    src/myalgorithm.cpp \
    src/mygraphbuilder.cpp \
    src/shortpath.cpp

HEADERS += \
    include/RenderEnum.h \
    include/SceneBuilder.h \
    include/mainwindow.h \
    include/mapview.h \
    include/model.h \
    include/modelDataHandler.h \
    include/modelDataStructure.h \
    include/modeldata.h \
    include/myalgorithm.h \
    include/mygraphbuilder.h \
    include/projection.h \
    include/renderitem.h \
    include/shortpath.h

FORMS += \
    ui/mainwindow.ui

INCLUDEPATH += \
    /usr/include \
    /usr/local/include

LIBS += -lz \
        -pthread \
        -lexpat

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L/usr/lib -lexpat
else:win32:CONFIG(debug, debug|release): LIBS += -L/usr/lib -lexpat
else:unix: LIBS += -L/usr/lib -lexpat

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/src