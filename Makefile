# Filename:	Makefile
# Author:   Alex Mikhalev
# Date:     June 29, 2015

# General configuration
NAME		:= 7SegClock
MCU			:= atmega328p
F_CPU		:= 16000000UL

# Directories and files
SRC_DIR     := src
LIB_DIR		:= lib
BUILD_DIR   := build

SRCS        := $(wildcard $(SRC_DIR)/*.c)
OBJS        := $(subst $(SRC_DIR)/,$(BUILD_DIR)/,$(SRCS:.c=.o))
DEPS		:= $(subst $(SRC_DIR)/,$(BUILD_DIR)/,$(SRCS:.c=.d))
OUTPUT_ELF  := $(BUILD_DIR)/$(NAME).elf
OUTPUT_HEX	:= $(BUILD_DIR)/$(NAME).hex
OUTPUT_LSS	:= $(BUILD_DIR)/$(NAME).lss

# Tool configs
CC          := avr-gcc
AR          := avr-ar
OBJDUMP		:= avr-objdump
OBJCOPY		:= avr-objcopy
RM			:= rm -rf
LIBS		:=
INCLUDES	:=-I $(SRC_DIR)
CFLAGS      =-Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields
CFLAGS		+= -mmcu=$(MCU) -DF_CPU=$(F_CPU)
CFLAGS		+= $(INCLUDES)

# Libraries
DS1337_NAME := libds1337
DS1337_LIB	:= $(LIB_DIR)/$(DS1337_NAME)/build/$(DS1337_NAME).a
LIBS		+= $(DS1337_LIB)
INCLUDES 	+= -I $(LIB_DIR)/$(DS1337_NAME)/src
SUBDIRS		+= $(LIB_DIR)/$(DS1337_NAME)

$(DS1337_LIB):
	$(MAKE) -C $(LIB_DIR)/$(DS1337_NAME) all

.PHONY: all disassemble clean size
.DEFAULT_GOAL := all

# GCC generated dependencies
-include $(DEPS)

# Phony targets
all: $(OUTPUT_HEX) disassemble size
	
disassemble: $(OUTPUT_LSS)

clean:
	@echo
	@echo === Removing "$(BUILD_DIR)" and all subdirectories ====
	@echo
	$(RM) $(BUILD_DIR)
	$(foreach subdir,$(SUBDIRS),$(MAKE) -C $(subdir) clean)

size: $(OUTPUT_ELF)
	@echo
	@echo === Printing output size ====
	@echo
	avr-size --format=avr --mcu=$(MCU) $(OUTPUT_ELF)

# File targets
$(BUILD_DIR):
	@echo
	@echo === Creating "$(BUILD_DIR)" directory ====
	@echo
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo
	@echo === Compiling "$<" using "$(CC)" ====
	@echo
	$(CC) $(CFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"

$(OUTPUT_ELF): $(OBJS) $(LIBS)
	@echo
	@echo === Linking "$(OUTPUT_ELF)" ====
	@echo
	$(CC) -Wl,-Map,$(NAME).map -o "$(OUTPUT_ELF)" $(OBJS) $(LIBS)
	
$(OUTPUT_HEX): $(OUTPUT_ELF)
	@echo
	@echo === Copying binary to "$(OUTPUT_HEX)" in intel hex format ====
	@echo
	$(OBJCOPY) -R .eeprom -R .fuse -R .lock -R .signature -O ihex $(OUTPUT_ELF) "$(OUTPUT_HEX)"

$(OUTPUT_LSS): $(OUTPUT_ELF)
	@echo
	@echo === Disassembling "$(OUTPUT_ELF)" to "$(OUTPUT_LSS)" ====
	@echo
	$(OBJDUMP) -h -S $(OUTPUT_ELF) >"$(OUTPUT_LSS)"
