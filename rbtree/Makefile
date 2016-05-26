CC = g++-4.9 
CFLAGS = -std=c++11
#CFLAGS += -g
RM = rm -f

OBJECTS = test_rbt.o

all: test_rbt

test_rbt: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) $(CFLAGS) -MM $< > $*.d

-include $(OBJECTS:.o=.d)

test: test_rbt
	./test_rbt
clean:
	$(RM) test_rbt test_rbt.o test_rbt.d
