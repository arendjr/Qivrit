#-------------------------------------------------
#
# Project created by QtCreator 2011-10-22T17:58:58
#
#-------------------------------------------------

QT += core gui widgets

TARGET = Qivrit
TEMPLATE = app

SOURCES += \
    lesson.cpp \
    lessonselectiondialog.cpp \
    main.cpp \
    mainwindow.cpp \
    rawlesson.cpp

HEADERS  += \
    lesson.h \
    lessonselectiondialog.h \
    mainwindow.h \
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
