CC=gcc
CFLAGS=-Os -Wall -std=c11
BUILD_DIR=build/
TEST_APPLICATION=$(BUILD_DIR)/test_array_builder

test: test.h array_builder.c array_builder.h test_array_builder.c
	mkdir -p $(BUILD_DIR)
	$(CC) -o $(TEST_APPLICATION) array_builder.c test_array_builder.c
	./$(TEST_APPLICATION)

clean:
	rm -rf $(BUILD_DIR)
