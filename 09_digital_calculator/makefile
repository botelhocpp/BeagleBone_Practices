# setenv appCalc.bin "setenv autoload no;setenv ipaddr 10.4.1.2; setenv serverip 10.4.1.1; tftp 0x80000000 /tftpboot/app;echo ***Booting to BareMetal ***;go 0x80000000"

CHAIN=$(CHAINPATH)arm-none-eabi
CFLAGS=-std=c99 -Wall -mfpu=neon -mhard-float -mcpu=cortex-a8
AFLAGS=-mfpu=neon
IPATH=-Iinc/
SRC=src/
OBJ=obj/
BIN=bin/
TFTP=/tftpboot/

all: clean app

app: start.o main.o gpio.o pad.o control_module.o clock_module.o interrupt.o uart.o cpu.o drivers.o lcd.o timers.o
	$(CHAIN)-ld $(OBJ)start.o $(OBJ)main.o $(OBJ)gpio.o $(OBJ)pad.o $(OBJ)control_module.o $(OBJ)clock_module.o $(OBJ)interrupt.o $(OBJ)uart.o $(OBJ)cpu.o $(OBJ)drivers.o $(OBJ)lcd.o $(OBJ)timers.o -T $(SRC)memmap.ld -o $(OBJ)main.elf
	$(CHAIN)-objcopy $(OBJ)main.elf $(BIN)spl.boot -O binary
	cp $(BIN)spl.boot $(TFTP)appCalc.bin
	ln -s $(TFTP)appCalc.bin $(TFTP)app

start.o: $(SRC)start.s
	$(CHAIN)-as $(AFLAGS) $(SRC)start.s -o $(OBJ)start.o

main.o: $(SRC)main.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)main.c -o $(OBJ)main.o
                                        
gpio.o: $(SRC)gpio.c                          
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)gpio.c -o $(OBJ)gpio.o
 
pad.o: $(SRC)pad.c                            
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)pad.c -o $(OBJ)pad.o

control_module.o: $(SRC)control_module.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)control_module.c -o $(OBJ)control_module.o

clock_module.o: $(SRC)clock_module.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)clock_module.c -o $(OBJ)clock_module.o

interrupt.o: $(SRC)interrupt.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)interrupt.c -o $(OBJ)interrupt.o

uart.o: $(SRC)uart.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)uart.c -o $(OBJ)uart.o

cpu.o: $(SRC)cpu.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)cpu.c -o $(OBJ)cpu.o
                                        
drivers.o: $(SRC)drivers.c                          
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)drivers.c -o $(OBJ)drivers.o
                                        
lcd.o: $(SRC)lcd.c                          
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)lcd.c -o $(OBJ)lcd.o
	
timers.o: $(SRC)timers.c                          
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)timers.c -o $(OBJ)timers.o

copy:
	cp $(BIN)spl.boot /tftpboot/appCalc.bin

clean:
	rm -rf $(OBJ)*.o
	rm -rf $(OBJ)*.elf
	rm -rf $(BIN)*.boot
	rm -rf $(TFTP)appCalc.bin
	rm -rf $(TFTP)app

dump:
	$(CHAIN)-objdump -D $(OBJ)main.elf
