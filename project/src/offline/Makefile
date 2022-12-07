SRCS:=$(wildcard *.cc)
OBJECTS:=$(patsubst %.cc ,%.o,$(SRCS))
CC:=g++
SERVER:=main


$(SERVER):$(OBJECTS)
	$(CC) $^ -o $@ -g -llog4cpp -pthread
%.o:%.c
	$(CC) -c $^ -o $@ -g -llog4cpp -pthread
print:
	echo $(OBJECTS)
