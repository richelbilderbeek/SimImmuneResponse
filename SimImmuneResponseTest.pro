# Unit tests of the SimImmuneResponse simulation core
TEMPLATE = app
TARGET = SimImmuneResponseTest
QT += core gui widgets testlib
CONFIG += c++17 warn_on console testcase

include(src/core.pri)

HEADERS += \
  src/sim_widget.h \
  src/sprite_images.h
SOURCES += \
  src/sim_widget.cpp \
  src/sprite_images.cpp \
  tests/test_sim_immune_response.cpp
RESOURCES += src/resources.qrc
