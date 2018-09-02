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

CC       = gcc
# 加入头文件搜索路径
CFLAGS = 
OBJS = MainWindow.o outline.o
LDFLAGS  =  `pkg-config gtk+-2.0 --cflags --libs`

TARGET = main

$(OBJS):%.o:%.c　
	$(CC) -c $(CFLAGS) $< -o $@  $(LDFLAGS)
$(TARGET):$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)
 clean:
	-$(RM) $(TARGET) $(OBJS)　