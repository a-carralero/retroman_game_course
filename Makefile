APP     := game
CXX     := g++
CC      := gcc
SRC_DIR := src
BUILD_DIR := build
LIB_DIR := libs
LIBS    := $(LIB_DIR)/picoPNG/libpicopng.a $(LIB_DIR)/tinyPTC/libtinyptc.a -lX11
INCDIRS := -I$(SRC_DIR) -I$(LIB_DIR)

CFLAGS:= -Wall -Wextra -pedantic $(INCDIRS) -MMD -MP
LINKFLAGS:=

ifdef RELEASE
	CFLAGS += -O3
else
	CFLAGS += -g -O0 -fno-omit-frame-pointer
endif

ifdef SANITIZER
	CFLAGS += -fsanitize=address 
	LINKFLAGS:= -fsanitize=address
endif


CXXFLAGS  := $(CFLAGS) -std=c++17

SRCS := $(sort $(shell find $(SRC_DIR) -type f \( -name "*.cpp" -o -name "*.c" \)))
OBJS := $(patsubst $(SRC_DIR)%.cpp,$(BUILD_DIR)%.o,$(SRCS))
OBJS := $(patsubst $(SRC_DIR)%.c,$(BUILD_DIR)%.o,$(OBJS))
DEPS := $(patsubst %.o,%.d,$(OBJS))

include colors.mk

$(APP) : $(OBJS)
	$(call print_blue,Linking $(APP))
	$(CXX) $(OBJS) -o $(APP) $(LIBS) $(LINKFLAGS)

-include $(DEPS)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(call print_green,Compiling $<)
	$(CXX) -c $< $(CXXFLAGS) -o $@

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(call print_green,Compiling $<)
	$(CC) -c $< $(CFLAGS) -o $@ 

info:
	$(info $(SRCS))
	$(info $(OBJS))
	$(info $(DEPS))
	
clean:
	rm -f $(APP)
	rm -rf $(BUILD_DIR)

libs:
	$(MAKE) -C $(LIB_DIR)

libs-clean:
	$(MAKE) -C $(LIB_DIR) clean

help:
	@echo "Opciones disponibles:"
	@echo "make [RELEASE][SANITIZER]"
	@echo "make clean"
	@echo "make libs"
	@echo "make libs-clean"
	
.PHONY: info clean libs libs-clean help