TEMPLATE = app
CONFIG += console c++2a strict_c++
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
  build_instructions.h \
  post_processing.h

SOURCES += \
  build_instructions.cpp \
  main.cpp \
  post_processing.cpp

DISTFILES += \
  page_header.txt

for(file, DISTFILES) {
  exists($$file) {
    binfile="$$PWD/bin/$$file"".o"
    distfile="$$file"
    system(rm -f $$binfile)
    system(ld --relocatable --format=binary --output=$$binfile $$distfile)
#    system(objcopy --input binary --output elf32-i386 --binary-architecture i386 $$distfile $$binfile)
    #QMAKE_LFLAGS += -Wl,--no-warn-execstack -Wl,--format=binary -Wl,$$binfile -Wl,--format=default
    OBJECTS += $$binfile
  }
}

QMAKE_LFLAGS += -Wl,--no-warn-execstac
