BUILD_DIR=build/
TEST_APPLICATION=$(BUILD_DIR)/test_array_builder
INCLUDES = -I include/

CC=gcc
CFLAGS=-Os -Wall -std=c11 $(INCLUDES)

src = $(wildcard src/*.c)
obj = $(src:.c=.o)

test_src = $(wildcard test/*.c)
test_obj = $(test_src:.c=.o)

test: $(obj) $(test_obj)
	mkdir -p $(BUILD_DIR)
	$(CC) -o $(TEST_APPLICATION) $(INCLUDES) $^
	./$(TEST_APPLICATION)

clean:
	rm -rf $(BUILD_DIR)
	rm $(obj) $(test_obj)
