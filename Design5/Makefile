CC = gcc
CFLAGS = -Wall
OBJS = main.o disk.o file.o
TARGET = cacu
RM = rm -f

$(TARGET):$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)
# (将OBJS中所有扩展名为.o的文件替换成扩展名为.c的文件)
$(OBJS): %.o:%.c
	$(CC) -c $^ -o $@ $(CFLAGS)
clean:
	-$(RM) $(TARGET) $(OBJS)　