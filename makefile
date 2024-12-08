# Compiler and flags
CC = gcc
CFLAGS = -g -Wall -pg -Wextra -Werror -I. -IRegression
LDFLAGS = -lm

# Source files
SRCS = Test/test.c Regression/Linear.c EDA/DataAnalysis.c Regression/model_performance_with_regularization.c
OBJS = $(SRCS:.c=.o)

# Targets
all: build_dir build/test

build_dir: 
	mkdir -p build

build/test: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f  $(OBJS) build/test
	rmdir build

.PHONY: all clean
