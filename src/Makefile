
subdirs=$(shell find . -mindepth 1 -maxdepth 1 -type d)

all:
	set -e; \
	for d in $(subdirs); do \
	    $(MAKE) -C $$d; \
	done

clean:
	for d in $(subdirs); do make -C $$d $@; done

