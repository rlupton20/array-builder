#!/bin/sh

# Run unit tests
make clean
make test

# Check for memory leaks in the tests with valgrind
valgrind --leak-check=full --error-exitcode=1 build/test_array_builder
