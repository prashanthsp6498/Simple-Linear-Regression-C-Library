# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I. -IRegression
LDFLAGS = -lm

# Source files
SRCS = Test/test.c Regression/Linear.c EDA/DataAnalysis.c
OBJS = $(SRCS:.c=.o)

# Targets
all: build/test

build/test: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) build/test

.PHONY: all clean
