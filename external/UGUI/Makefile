
BASE = ../..

include ../../makefile.conf

# ignore missing prototypes and discarded-qualifiers (string literals as
# non-const) warning
WARN_FLAGS := $(filter-out \
              -Wmissing-prototypes \
              -Wwrite-strings \
              , $(WARN_FLAGS))

.PHONY: clean

OBJS = ugui.o

all: $(OBJS)

clean:
	rm -f $(OBJS)

