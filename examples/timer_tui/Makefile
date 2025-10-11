
NAME = timer_tui
BASE = ../..

OBJS = $(BASE)/src/init/init.o \
       $(BASE)/src/usart1/usart1.o \
       $(BASE)/external/printf/printf.o

include ../../makefile.conf

INCLUDES+=-I$(BASE)/external/printf -I$(BASE)/src/usart1

.PHONY : all clean

all : $(NAME).bin

clean :
	rm -f *.elf *.map *.o *bin

