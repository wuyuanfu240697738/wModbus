#子目录的Makefile直接读取其子目录就行
SUBDIRS=$(shell ls -l | grep ^d | awk '{print $$9}')

#以下同根目录下的makefile的相同代码的解释
CUR_OBJS=${patsubst %.c, %.o, ${wildcard *.c}}

all:$(SUBDIRS) $(CUR_OBJS)

$(SUBDIRS):
	make -C $@

$(CUR_OBJS):%.o:%.c
	$(CC) -c $^ $(CFLAGS) -o $(OBJ_DIR)/$@

	
	
