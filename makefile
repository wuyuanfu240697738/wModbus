#设置编译器
CROSS_COMPILE ?= 
CC := $(CROSS_COMPILE)gcc

#这里使用awk排除了debug文件夹，读取剩下的文件夹
SUBDIRS=$(shell ls -l | grep ^d | awk '{if($$9 != "debug") print $$9}')

$(shell mkdir debug)

#inc文件所在的目录
DIR_INC = $(shell pwd)
#obj文件所在的目录
OBJ_DIR = $(shell pwd)/debug
#bin文件所在的目录
BIN_DIR = $(shell pwd)/OutAPP

CFLAGS = -g -Wall -I${DIR_INC}

#获取当前目录下的c文件集，转为o文件后放在下面的CUR_OBJS变量中
CUR_OBJS=${patsubst %.c, %.o, ${wildcard *.c}}

#将以下变量导出到子shell中，本次相当于导出到子目录下的makefile中
export CC CFLAGS OBJ_DIR

#注意这里的顺序，需要先执行SUBDIRS最后才能是DEBUG
all:$(SUBDIRS) $(CUR_OBJS) DEBUG
#递归执行子目录下的makefile文件，这是递归执行的关键
$(SUBDIRS):ECHO
	make -C $@
DEBUG:ECHO
	$(CC) $(CFLAGS) -o $(BIN_DIR) $(OBJ_DIR)/*.o
ECHO:
	@echo $(SUBDIRS)

#将c文件编译为o文件，并放在指定放置目标文件的目录中即OBJS_DIR
$(CUR_OBJS):%.o:%.c
	$(CC) -c $^ $(CFLAGS) -o $(OBJ_DIR)/$@

.PHONY : clean
clean:
	@rm -rf $(BIN_DIR)
	@rm $(OBJ_DIR)/*.o
	
	
