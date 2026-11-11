# SimImmuneResponse: educational simulation of the human immune response
TEMPLATE = app
TARGET = SimImmuneResponse
QT += core gui widgets
CONFIG += c++17 warn_on

include(src/core.pri)

HEADERS += \
  src/main_window.h \
  src/sim_widget.h \
  src/sprite_images.h
SOURCES += \
  src/main.cpp \
  src/main_window.cpp \
  src/sim_widget.cpp \
  src/sprite_images.cpp
RESOURCES += src/resources.qrc
