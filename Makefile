CC=gcc
CFLAGS=-Wall -Wextra -std=c99
TARGET=calc

all: $(TARGET)

$(TARGET): calc.c
	$(CC) $(CFLAGS) -o $(TARGET) calc.c

clean:
	rm -f $(TARGET) calc_ans.txt calc_history.txt
