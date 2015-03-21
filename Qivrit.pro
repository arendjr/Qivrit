#-------------------------------------------------
#
# Project created by QtCreator 2011-10-22T17:58:58
#
#-------------------------------------------------

QT += core gui

TARGET = Qivrit
TEMPLATE = app

SOURCES += \
    lesson.cpp \
    lessonselectiondialog.cpp \
    main.cpp \
    mainwindow.cpp \
    qjson/json_driver.cc \
    qjson/json_parser.cc \
    qjson/json_scanner.cpp \
    rawlesson.cpp

HEADERS  += \
    lesson.h \
    lessonselectiondialog.h \
    mainwindow.h \
    qjson/json_driver.hh \
    qjson/json_parser.hh \
    qjson/json_scanner.h \
    rawlesson.h \
    shinkeyeventfilter.h

FORMS += \
    lessonselectiondialog.ui \
    mainwindow.ui

OTHER_FILES += \
    lessons/alphabet.json \
    lessons/colors.json \
    lessons/common1.json \
    lessons/common2.json \
    lessons/common3.json \
    lessons/pronounspossessives.json

RESOURCES += \
    resources.qrc
