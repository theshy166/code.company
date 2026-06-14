# Makefile for TRNG tools

#CC = $(CROSS_COMPILE)gcc
#CC = arm-mol-linux-uclibcgnueabihf-gcc
CC = arm-molv510-linux-gcc
PTHREAD_LIBS = -lpthread
WARNINGS = -Wall -Wextra
CFLAGS = $(WARNINGS) -g -I../include
LDFLAGS = $(PTHREAD_LIBS)

all: test_trng
%: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) test_trng
