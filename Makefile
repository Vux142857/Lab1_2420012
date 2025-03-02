TARGET_EXEC := calc

SRC_DIRS := .

SRCS := $(shell find $(SRC_DIRS) -name '*.c')

OBJS := $(SRCS:.c=.o)

CFLAGS := -g -Wall

$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET_EXEC)

.PHONY: all
all: $(TARGET_EXEC)