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


GREEN  := \033[0;32m
BLUE   := \033[0;34m
RESET  := \033[0m
define print_green
	@printf "$(GREEN)%s$(RESET)\n" "$(1)"
endef
define print_blue
	@printf "$(BLUE)%s$(RESET)\n" "$(1)"
endef


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

.PHONY: info clean libs libs-clean