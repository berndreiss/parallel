CC = mpicc
CFLAGS = -Wall -O2

TARGET = main
TARGET_2 = main_serial

SRC = main.c
SRC_2 = main_serial.c

all: $(TARGET) $(TARGET_2)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_2): $(SRC_2)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) $(TARGET_2)

