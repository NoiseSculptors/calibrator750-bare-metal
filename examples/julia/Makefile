
NAME = julia
BASE = ../..

OBJS = \
       $(BASE)/drivers/display/ssd1315/ssd1315.o \
       $(BASE)/src/delay/delay.o \
       $(BASE)/src/usart1/usart1.o \
       $(BASE)/external/printf/printf.o \
       $(BASE)/src/init/init.o

include ../../makefile.conf

OPTIMIZE_FLAG = -Os

LDLIBS += -lm

INCLUDES+=-I$(BASE)/external/printf \
          -I$(BASE)/src/usart1 \
          -I$(BASE)/drivers/display/ssd1315 

.PHONY : all clean

all : $(NAME).bin

clean :
	rm -f *.elf *.map *.o *bin

