# Filename:	Makefile
# Author:   Alex Mikhalev
# Date:     June 29, 2015

NAME		= 7SegClock

CC          = avr-gcc
AR          = avr-ar
OBJDUMP		= avr-objdump
OBJCOPY		= avr-objcopy
RM			= rm -rf
MCU			= atmega328p
CPUFLAGS    =-mmcu=${MCU} -DF_CPU=16000000UL
CFLAGS      =-Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields ${CPUFLAGS} -I ${SRC_DIR}

SRC_DIR     = ./src
BUILD_DIR   = ./build

SRCS        = $(wildcard ${SRC_DIR}/*.c)
OBJS        = $(subst ${SRC_DIR}/,${BUILD_DIR}/,${SRCS:.c=.o})
DEPS		= $(subst ${SRC_DIR}/,${BUILD_DIR}/,${SRCS:.c=.d})
OUTPUT_ELF  = ${BUILD_DIR}/${NAME}.elf
OUTPUT_HEX	= ${BUILD_DIR}/${NAME}.hex
OUTPUT_LSS	= ${BUILD_DIR}/${NAME}.lss

.PHONY: all disassemble size
.DEFAULT_GOAL := all

-include $(DEPS)

all: ${OUTPUT_HEX} disassemble size
	
disassemble: ${OUTPUT_LSS}

clean:
	@echo ' '
	@echo '=== Removing "${BUILD_DIR}" and all subdirectories ===='
	@echo ' '
	${RM} ${BUILD_DIR}

${BUILD_DIR}:
	@echo ' '
	@echo '=== Creating "${BUILD_DIR}" directory ===='
	@echo ' '
	mkdir ${BUILD_DIR}

${BUILD_DIR}/%.o: ${SRC_DIR}/%.c ${BUILD_DIR}
	@echo ' '
	@echo '=== Compiling "$<" using "${CC}" ===='
	@echo ' '
	${CC} ${CFLAGS} -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"

${OUTPUT_ELF}: ${OBJS}
	@echo ' '
	@echo '=== Linking "${OUTPUT_ELF}" ===='
	@echo ' '
	${CC} -Wl,-Map,${NAME}.map ${CPUFLAGS} -o "${OUTPUT_ELF}" $(OBJS)
	
${OUTPUT_HEX}: ${OUTPUT_ELF}
	@echo ' '
	@echo '=== Copying binary to "${OUTPUT_HEX}" in binary format ===='
	@echo ' '
	${OBJCOPY} -R .eeprom -R .fuse -R .lock -R .signature -O ihex ${OUTPUT_ELF} "${OUTPUT_HEX}"

${OUTPUT_LSS}: ${OUTPUT_ELF}
	@echo ' '
	@echo '=== Disassembling "${OUTPUT_ELF}" to ${OUTPUT_LSS} ===='
	@echo ' '
	${OBJDUMP} -h -S ${OUTPUT_ELF} >"${OUTPUT_LSS}"
	
size: ${OUTPUT_ELF}
	@echo ' '
	@echo '=== Printing output size ===='
	@echo ' '
	avr-size --format=avr --mcu=${MCU} ${OUTPUT_ELF}
