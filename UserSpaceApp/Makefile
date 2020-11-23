TARGET ?= kicapp

CC = gcc
RM = rm -f
CFLAGS  ?= -g -s -Wall -Wstrict-prototypes
LIBS = -lm

all: $(TARGET)

clean: 
	$(RM) $(TARGET)

run:
	@echo "Userspace application: $(TARGET) running..."
	./$(TARGET)
	
$(TARGET): $(TARGET).c
	$(CC) -o $@ $(CFLAGS) $< $(LIBS)

