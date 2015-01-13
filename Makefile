AS = arm-none-eabi-as
ASFLAGS = -mthumb -mcpu=cortex-m0 -march=armv6-m

CC = arm-none-eabi-gcc
CFLAGS = -nostdlib -Os $(ASFLAGS)

LDFLAGS = $(CFLAGS) -T payload.ld

OBJCOPY = arm-none-eabi-objcopy

main.bin: main
	$(OBJCOPY) -O binary $< $@

program: main.bin
	JLinkExe flash.jlink

.PHONY: clean
clean:
	rm -f main main.bin *.o
