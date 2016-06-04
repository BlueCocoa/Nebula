#
# Makefile
#
#

PREFIX ?= arm-none-eabi-
CC  = $(PREFIX)gcc
CPP = $(PREFIX)g++
AS  = $(CC)
LD  = $(PREFIX)ld
AR  = $(PREFIX)ar

ARCH ?= -march=armv7-a -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard

INCLUDE = include/
OBJECTS = objects/

AFLAGS += $(ARCH)
CFLAGS += $(ARCH) -O2 -fno-builtin -ffreestanding -nostdinc -nostdlib -I INCLUDE
CPPFLAGS += $(CFLAGS) -fno-exceptions -fno-rtti -nostartfiles -std=c++0x

AS_SOURCES = $(wildcard *.s)
CPP_SOURCES = $(wildcard *.cpp)

AS_KERNEL_OBJS = $(patsubst %.s, $(OBJECTS)%.o, $(AS_SOURCES))
CPP_KERNEL_OBJS = $(patsubst %.cpp, $(OBJECTS)%.o, $(CPP_SOURCES))

TARGET = kernel.img

$(OBJECTS)%.o: %.s
	$(AS) $(AFLAGS) -c -o $@ $<

$(OBJECTS)%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJECTS)%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

$(TARGET): $(AS_KERNEL_OBJS) $(CPP_KERNEL_OBJS)
	$(LD) -o kernel.elf -Map kernel.map -T memmap $(AS_KERNEL_OBJS) $(CPP_KERNEL_OBJS)
	$(PREFIX)objcopy kernel.elf -O binary $(TARGET)

clean :
	-rm -f $(AS_KERNEL_OBJS)
	-rm -f $(CPP_KERNEL_OBJS)
	-rm -f $(TARGET)
