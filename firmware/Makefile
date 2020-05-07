MCU?=atmega32u4
CC:=avr-gcc
CFLAGS=-O2 -mmcu=$(MCU) -DF_CPU=16000000UL -DARCH=ARCH_AVR8
LDFLAGS=-Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -MMD -MP -MF

all: main.hex

%.elf: %.c
	$(CC) $(CFLAGS) $< -o $@

# test.elf: test.o
# 	$(CC) $(LDFLAGS) $< -o $@

%.hex: %.elf
	avr-objcopy -I elf32-avr -O ihex $< $@
	avr-size --format=avr --mcu=$(MCU) $<
