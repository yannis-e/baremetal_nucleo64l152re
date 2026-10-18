# ============================================================
# Toolchain
# ============================================================

CROSS   := arm-none-eabi-
CC      := $(CROSS)gcc
OBJCOPY := $(CROSS)objcopy
SIZE    := $(CROSS)size

# ============================================================
# Project
# ============================================================

TARGET := firmware
BUILD  := build
LINKER := src/link.ld

# ============================================================
# Source files
# Automatically find all C files except standalone tests.
# ============================================================

SOURCES := $(shell find src -type f -name '*.c' \
            ! -path 'src/test/*')

# src/startup.c    -> build/startup.o
# src/app/main.c   -> build/app/main.o
# src/drivers/gpio.c -> build/drivers/gpio.o

OBJECTS := $(patsubst src/%.c,$(BUILD)/%.o,$(SOURCES))
DEPS    := $(OBJECTS:.o=.d)

# ============================================================
# CPU: STM32L152RE / ARM Cortex-M3
# ============================================================

CPUFLAGS := -mcpu=cortex-m3 -mthumb

# ============================================================
# Compiler flags
# ============================================================

CFLAGS := $(CPUFLAGS) \
          -std=c11 \
          -Wall -Wextra -Werror \
          -Wundef -Wshadow \
          -ffreestanding \
          -ffunction-sections \
          -fdata-sections \
          -fno-common \
          -g3 -Og \
          -Iinclude \
          -Isrc

# ============================================================
# Linker flags
# ============================================================

LDFLAGS := $(CPUFLAGS) \
           -T$(LINKER) \
           -nostdlib \
           -Wl,--gc-sections \
           -Wl,-Map=$(BUILD)/$(TARGET).map \
           -Wl,--print-memory-usage

# ============================================================
# Targets
# ============================================================

.PHONY: all clean flash size monitor

all: $(BUILD)/$(TARGET).bin

# ------------------------------------------------------------
# Link ELF firmware
# ------------------------------------------------------------

$(BUILD)/$(TARGET).elf: $(OBJECTS) $(LINKER)
	mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	$(SIZE) $@

# ------------------------------------------------------------
# Convert ELF to binary
# ------------------------------------------------------------

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# ------------------------------------------------------------
# Compile source files
# ------------------------------------------------------------

$(BUILD)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# ------------------------------------------------------------
# Header dependencies
# ------------------------------------------------------------

-include $(DEPS)

# ------------------------------------------------------------
# Display firmware size
# ------------------------------------------------------------

size: $(BUILD)/$(TARGET).elf
	$(SIZE) $<

# ------------------------------------------------------------
# Flash using ST-Link
# Requires st-flash to be installed.
# ------------------------------------------------------------

flash: $(BUILD)/$(TARGET).bin
	st-flash --reset write $< 0x08000000

# ------------------------------------------------------------
# Clean generated files
# ------------------------------------------------------------

clean:
	rm -rf $(BUILD)

# ------------------------------------------------------------
# UART serial monitor using picocom
# ------------------------------------------------------------

PORT ?= /dev/ttyACM0
BAUD ?= 115200

monitor:
	picocom -b $(BAUD) $(PORT)