CHAINPATH=
CHAIN=$(CHAINPATH)arm-none-eabi
CFLAGS=-std=c99 -Wall -mfpu=neon -mhard-float -mcpu=cortex-a8
AFLAGS=-mfpu=neon
IPATH=-Iinc/
SRC=src/
OBJ=obj/
BIN=bin/
TFTP=/tftpboot/

all: app

app: start.o main.o drivers.o gpio.o system.o uart.o
	$(CHAIN)-ld $(OBJ)start.o $(OBJ)main.o $(OBJ)drivers.o $(OBJ)gpio.o $(OBJ)system.o $(OBJ)uart.o -T $(SRC)memmap.ld -o $(OBJ)main.elf
	$(CHAIN)-objcopy $(OBJ)main.elf $(BIN)spl.boot -O binary
	cp $(BIN)spl.boot /tftpboot/appInterrupt.bin
	ln -s $(TFTP)appInterrupt.bin $(TFTP)app

start.o: $(SRC)start.s
	$(CHAIN)-as $(AFLAGS) $(SRC)start.s -o $(OBJ)start.o

main.o: $(SRC)main.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)main.c -o $(OBJ)main.o

gpio.o: $(SRC)gpio.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)gpio.c -o $(OBJ)gpio.o

uart.o: $(SRC)uart.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)uart.c -o $(OBJ)uart.o

system.o: $(SRC)system.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)system.c -o $(OBJ)system.o

drivers.o: $(SRC)drivers.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)drivers.c -o $(OBJ)drivers.o
                                        
copy:
	cp $(BIN)spl.boot /tftpboot/appInterrupt.bin

clean:
	rm -rf $(OBJ)*.o
	rm -rf $(OBJ)*.elf
	rm -rf $(BIN)*.boot
	rm -rf $(TFTP)appInterrupt.bin
	rm -rf $(TFTP)app

dump:
	$(CHAIN)-objdump -D $(OBJ)main.elf
