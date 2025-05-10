QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


CONFIG += link_pkgconfig
PKGCONFIG += fluidsynth  #You must Download them

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fluidsynth_initial.cpp \
    keyboard.cpp \
    main.cpp \
    mainwindow.cpp \
    mappingmanager.cpp \
    notesenseanswer.cpp \
    notesensesetting.cpp \
    pianokeys.cpp \
    randomnotegenerator.cpp \
    startwindow.cpp

HEADERS += \
    fluidsynth_initial.h \
    keyboard.h \
    mainwindow.h \
    mappingmanager.h \
    notesenseanswer.h \
    notesensesetting.h \
    pianokeys.h \
    randomnotegenerator.h \
    startwindow.h

FORMS += \
    mainwindow.ui \
    notesensesetting.ui \
    startwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    soundfont.qrc
