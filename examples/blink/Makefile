
NAME = blink
BASE = ../..

OBJS = $(BASE)/src/init/init.o \
       $(BASE)/external/printf/printf.o \
       $(BASE)/src/delay/delay.o \
       $(BASE)/src/usart1/usart1.o

include ../../makefile.conf

INCLUDES+=-I$(BASE)/src/xprintf -I$(BASE)/src/delay -I$(BASE)/src/usart1

.PHONY : all clean

all : $(NAME).bin

clean :
	rm -f *.elf *.map *.o *bin

