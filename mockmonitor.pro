######################################################################
# Automatically generated by qmake (3.0) ? 6 20 23:25:35 2014
######################################################################

TEMPLATE = app
TARGET = mockmonitor
INCLUDEPATH += .

# Input
HEADERS += mainwindow.h
FORMS += mockmonitor.ui
SOURCES += mainwindow.cpp mockmonitor.cpp

# Special configuration for QWT project.
# CONFIG += console embed_manifest_exe
QT += widgets serialport
INCLUDEPATH += C:\Qwt-6.1.4\include
LIBS += -LC:\Qwt-6.1.4\lib -lqwt
