TEMPLATE = app
CONFIG += console c++2a
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
  build_instructions.h \
  post_processing.h

SOURCES += \
  build_instructions.cpp \
  instruction_set.cpp \
  post_processing.cpp
