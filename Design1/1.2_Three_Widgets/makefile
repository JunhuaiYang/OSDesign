CC       = gcc
CFLAGS   =  
LDFLAGS  =  `pkg-config gtk+-2.0 --cflags --libs`

SRCS = ShowNum.c Sum.c TimeShow.c main.c
TARGETS = $(patsubst %.c, %, $(SRCS))

all:$(TARGETS)

$(TARGETS):$(SRCS)
	$(CC) $@.c -o $@ $(LDFLAGS) $(CFLAGS)

clean:
	-rm -rf $(TARGETS) *~ .*swp

.PHONY: clean all