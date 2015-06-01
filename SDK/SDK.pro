TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += release

HEADERS += HCNetSDK.h
SOURCES += ipcamera.cpp

linux:LIBS += -L ~/C++/SDK -lhcnetsdk -lHCCore -lhpr
