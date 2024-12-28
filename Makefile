TARGET=process_pool
SRC=$(wildcard *.cc)
FLAGS=-c -Wall -std=c++11
TARGETFLAG = -o
CC=g++
OBJ=$(SRC:.cc=.o)

# 生成目标文件
$(TARGET):$(OBJ)
	$(CC) $^ $(TARGETFLAG) $@

# 生成对应的.o文件
%.o:%.cc
	$(CC) $< $(FLAGS)

# 清理
.PHONY:clean
clean:
	rm -f $(OBJ) $(TARGET)