all: app

app: main.o stdled.o stdcommon.o
	@arm-linux-gcc -static obj/main.o obj/stdled.o obj/stdcommon.o -o bin/led

main.o:
	@arm-linux-gcc -static  -Wall -Iinc -c src/main.c -o obj/main.o

stdled.o:
	@arm-linux-gcc -static  -Wall -Iinc -c src/stdled.c -o obj/stdled.o

stdcommon.o:
	@arm-linux-gcc -static  -Wall -Iinc -c src/stdcommon.c -o obj/stdcommon.o

clean:
	@rm -rf bin/*
	@rm -rf obj/*