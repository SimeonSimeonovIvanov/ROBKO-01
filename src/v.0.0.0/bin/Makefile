###############################################################################
# Makefile for the project ROBKO
###############################################################################

## General Flags
PROJECT = ROBKO
MCU = atmega128
TARGET = ROBKO.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2         -std=gnu99               -DF_CPU=16000000UL -Os -fsigned-char
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=ROBKO.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"D:\Developments\Robotics\Robotic Arm\ROBKO 01\src\v.0.0.0\FreeMODBUS" -I"D:\Developments\Robotics\Robotic Arm\ROBKO 01\src\v.0.0.0\FreeMODBUS\port" -I"D:\Developments\Robotics\Robotic Arm\ROBKO 01\src\v.0.0.0\FreeMODBUS\modbus\rtu" -I"D:\Developments\Robotics\Robotic Arm\ROBKO 01\src\v.0.0.0\FreeMODBUS\modbus\tcp" -I"D:\Developments\Robotics\Robotic Arm\ROBKO 01\src\v.0.0.0\FreeMODBUS\modbus\include" -I"D:\Developments\Robotics\Robotic Arm\ROBKO 01\src\v.0.0.0\net" -I"D:\Developments\Robotics\Robotic Arm\ROBKO 01\src\v.0.0.0\sys" -I"D:\Developments\Robotics\Robotic Arm\ROBKO 01\src\v.0.0.0\app" -I"D:\Developments\Robotics\Robotic Arm\ROBKO 01\src\v.0.0.0\arch" -I"D:\Developments\Robotics\Robotic Arm\ROBKO 01\src\v.0.0.0\net\enc424j600" -I"D:\Developments\Robotics\Robotic Arm\ROBKO 01\src\v.0.0.0\i2c" -I"D:\Developments\Robotics\Robotic Arm\ROBKO 01\src\v.0.0.0\rtc" -I"D:\Developments\Robotics\Robotic Arm\ROBKO 01\src\v.0.0.0\rtc\ds1307" 

## Libraries
LIBS = -lm 

## Objects that must be built in order to link
OBJECTS = main.o portevent.o portserial.o porttimer.o mb.o mbfunccoils.o mbfuncdiag.o mbfuncdisc.o mbfuncholding.o mbfuncinput.o mbfuncother.o mbutils.o mbcrc.o mbrtu.o ROBKO_01.o arp.o ethernet.o icmp.o ip.o net.o tcp.o tcp_queue.o udp.o spi.o clock.o timer.o enc424j600.o porttcp.o mbtcp.o dhcp_client.o ds1307.o i2c.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) ROBKO.hex ROBKO.eep ROBKO.lss size

## Compile
main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

portevent.o: ../FreeMODBUS/port/portevent.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

portserial.o: ../FreeMODBUS/port/portserial.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

porttimer.o: ../FreeMODBUS/port/porttimer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mb.o: ../FreeMODBUS/modbus/mb.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mbfunccoils.o: ../FreeMODBUS/modbus/functions/mbfunccoils.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mbfuncdiag.o: ../FreeMODBUS/modbus/functions/mbfuncdiag.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mbfuncdisc.o: ../FreeMODBUS/modbus/functions/mbfuncdisc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mbfuncholding.o: ../FreeMODBUS/modbus/functions/mbfuncholding.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mbfuncinput.o: ../FreeMODBUS/modbus/functions/mbfuncinput.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mbfuncother.o: ../FreeMODBUS/modbus/functions/mbfuncother.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mbutils.o: ../FreeMODBUS/modbus/functions/mbutils.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mbcrc.o: ../FreeMODBUS/modbus/rtu/mbcrc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mbrtu.o: ../FreeMODBUS/modbus/rtu/mbrtu.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ROBKO_01.o: ../ROBKO_01.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

arp.o: ../net/arp.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ethernet.o: ../net/ethernet.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

icmp.o: ../net/icmp.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ip.o: ../net/ip.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

net.o: ../net/net.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

tcp.o: ../net/tcp.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

tcp_queue.o: ../net/tcp_queue.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

udp.o: ../net/udp.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

spi.o: ../arch/spi.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

clock.o: ../sys/clock.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timer.o: ../sys/timer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

enc424j600.o: ../net/enc424j600/enc424j600.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

porttcp.o: ../FreeMODBUS/port/porttcp.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mbtcp.o: ../FreeMODBUS/modbus/tcp/mbtcp.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

dhcp_client.o: ../app/dhcp_client.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ds1307.o: ../rtc/ds1307/ds1307.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

i2c.o: ../i2c/i2c.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) ROBKO.elf dep/* ROBKO.hex ROBKO.eep ROBKO.lss ROBKO.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

