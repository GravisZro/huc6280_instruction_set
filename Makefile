ifneq ($(VERBOSE),true)
  QUIET:=@
endif

# compiler options
ifndef CC
CC:=gcc
endif

ifndef CXX
CXX:=g++
endif

ifndef CPP_STANDARD
  CPP_STANDARD:=-std=c++20
endif

ifndef C_STANDARD
  C_STANDARD:=-std=c11
endif

# set CFLAGS
ifdef FLAGS
  CFLAGS=$(FLAGS)
endif

ifdef DEFINES
  CFLAGS += $(foreach var, $(DEFINES),-D$(var))
else # else use default options
endif

ifeq (,$(findstring -DLOCALE=,$(CFLAGS)))
  CFLAGS += -DLOCALE=US
endif


ifndef SOURCE_PATH
  SOURCE_PATH=.
endif

ifndef BUILD_PATH
  BUILD_PATH=$(SOURCE_PATH)/bin
endif

# define what to build
ifndef BINARY
  BINARY=huc6280_instruction_set
endif

SOURCES = \
	instruction_set.cpp \
	build_instructions.cpp \
	post_processing.cpp

OBJS := $(SOURCES:.s=.o)
OBJS := $(OBJS:.c=.o)
OBJS := $(OBJS:.cpp=.o)
OBJS := $(foreach f,$(OBJS),$(BUILD_PATH)/$(f))
SOURCES := $(foreach f,$(SOURCES),$(SOURCE_PATH)/$(f))

# !!! FIXME: Get -Wall in here, some day.
#CFLAGS += -w -fno-builtin -fno-strict-aliasing -fno-operator-names -fno-rtti -ffreestanding

# includes ...

.PHONY: all OUTPUT_DIR

$(BUILD_PATH)/%.o: $(SOURCE_PATH)/%.c
	@echo [Compiling]: $<
	$(QUIET) $(CC) -c -o $@ $< $(C_STANDARD) $(CFLAGS)

$(BUILD_PATH)/%.o: $(SOURCE_PATH)/%.s
	@echo [Assembling]: $<
	$(QUIET) $(CC) $(CPP_STANDARD) $(CFLAGS) -DELF -x assembler-with-cpp -o $@ -c $<


$(BUILD_PATH)/%.o: $(SOURCE_PATH)/%.cpp
	@echo [Compiling]: $<
	$(QUIET) $(CXX) -c -o $@ $< $(CPP_STANDARD) $(CFLAGS)

$(BUILD_PATH)/%.a: $(SOURCE_PATH)/%.a
	cp $< $@
	ranlib $@

$(BINARY): OUTPUT_DIR $(OBJS)
	@echo [ Linking ]: $@
	$(QUIET) $(CXX) -o $@ $(OBJS) $(LDFLAGS) $(CPP_STANDARD)

index.html: $(BINARY)
	@echo [ Writing Output ]: $@
	$(QUIET) ./$(BINARY) > $@

html: index.html $(BINARY)
	@echo [ DONE ]

OUTPUT_DIR:
	@echo -n "Creating build directory"
	$(QUIET) mkdir -p $(BUILD_PATH)
	@echo " DONE."

clean:
	rm -f $(BINARY)
	rm -rf $(BUILD_PATH)
