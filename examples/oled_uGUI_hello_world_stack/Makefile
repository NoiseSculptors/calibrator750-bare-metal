
NAME = oled_uGUI_hello_world

BASE = ../..

OBJS = $(BASE)/src/init/init.o \
       $(BASE)/src/usart1/usart1.o \
       $(BASE)/src/delay/delay.o \
       $(BASE)/drivers/display/ssd1315/ssd1315.o \
       $(BASE)/drivers/display/ssd1315_UGUI/ssd1315_UGUI.o \
       $(BASE)/external/printf/printf.o \
       $(BASE)/external/UGUI/ugui.o


include $(BASE)/makefile.conf

OPTIMIZE_FLAG = -Os

LDLIBS += -lm

INCLUDES+=-I$(BASE)/src/xprintf \
          -I$(BASE)/src/usart1 \
          -I$(BASE)/external/UGUI \
          -I$(BASE)/external/printf \
          -I$(BASE)/drivers/display/ssd1315 

.PHONY : all clean

all : $(NAME).bin

clean :
	rm -f *.elf *.map *.o *bin

