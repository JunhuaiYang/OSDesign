CC = gcc
CFLAGS = -Wall
LDFLAGS = `pkg-config gtk+-2.0 --cflags --libs`

SRCS = MainWindow.c outline.c menu.c cpu.c memory.c module.c status.c process.c
TARGETS = systemmonitor

all:$(TARGETS)

# ^ 表示所有目标文件
$(TARGETS):$(SRCS)
	$(CC) $^ -o $@ $(LDFLAGS) $(CFLAGS)

clean:
	-rm -rf $(TARGETS) *~ .*swp

.PHONY: clean all