# The simulation core: plain C++17, no Qt or GUI dependencies
INCLUDEPATH += $$PWD
HEADERS += \
  $$PWD/enums.h \
  $$PWD/location.h \
  $$PWD/sim_immune_response.h \
  $$PWD/sim_state.h \
  $$PWD/sprite.h
SOURCES += \
  $$PWD/enums.cpp \
  $$PWD/location.cpp \
  $$PWD/sim_immune_response.cpp \
  $$PWD/sim_state.cpp \
  $$PWD/sprite.cpp
