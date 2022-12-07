SRCS:=$(wildcard *.cc)
OBJECTS:=$(patsubst %.cc ,%.o,$(SRCS))
CC:=g++
SERVER:=main


$(SERVER):$(OBJECTS)
	$(CC) $^ -o $@ -g -llog4cpp -lpthread -lhiredis -lredis++ 
%.o:%.c
	$(CC) -c $^ -o $@ -g -llog4cpp -lpthread -lhiredis -lredis++ 
print:
	echo $(OBJECTS)
