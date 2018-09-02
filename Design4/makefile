# CC       = gcc
# CFLAGS   =  
# LDFLAGS  =  `pkg-config gtk+-2.0 --cflags --libs`

# SRCS = MainWindow.c outline.c
# TARGETS = $(patsubst %.c, %, $(SRCS))

# all:$(TARGETS)

# $(TARGETS):$(SRCS)
# 	$(CC) $^ -o $@.o $(LDFLAGS) $(CFLAGS)

# clean:
# 	-rm -rf $(TARGETS).o 

# .PHONY: clean all

# CC       = gcc
# 加入头文件搜索路径
# CFLAGS = 
# OBJS = MainWindow.o outline.o
# LDFLAGS  =  `pkg-config gtk+-2.0 --cflags --libs`

# TARGET = main

# $(TARGET):$(OBJS)
# 	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS) $(LDFLAGS)
# $(OBJS):%.o:%.c　
# 	$(CC) -c $(CFLAGS) $< -o $@  $(LDFLAGS)
#  clean:
# 	-$(RM) $(TARGET) $(OBJS)　

CC = gcc
CFLAGS = 
LDFLAGS = `pkg-config gtk+-2.0 --cflags --libs`

SRCS = outline.c MainWindow.c
TARGETS = app

all:$(TARGETS)

$(TARGETS):$(SRCS)
	$(CC) $^ -o $@ $(LDFLAGS) $(CFLAGS)

clean:
	-rm -rf $(TARGETS) *~ .*swp

.PHONY: clean all