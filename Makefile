TARGET  =   okamera
CC      =   gcc
CFLAGS  =   -O3

.PHONY: install clean all default

default: $(TARGET)

all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@
	
clean:
	rm -f *.o
	rm -f $(TARGET)

install: okamera
	install -m 0755 okamera /usr/local/bin
	install -m 0755 res/okamera /etc/init.d
	mkdir -p /etc/okamera
	install -m 0644 res/okamera_config /etc/okamera
