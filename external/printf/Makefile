
BASE = ../..

include $(BASE)/makefile.conf

# ignore double-promotion warning: "implicit conversion from 'float' to 'double'
# to match other operand of binary expression"
WARN_FLAGS := $(filter-out -Wdouble-promotion, $(WARN_FLAGS))

INCLUDES += -I$(BASE)/src/usart1

.PHONY: clean

OBJS = printf.o

all: $(OBJS)

clean:
	rm -f $(OBJS)

