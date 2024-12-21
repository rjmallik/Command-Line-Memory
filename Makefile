EXECBIN  = memory
SOURCES  = $(wildcard *.c)
OBJECTS  = $(SOURCES:%.c=%.o)
FORMATS  = $(SOURCES:%.c=%.fmt)

CC       = clang
FORMAT   = clang-format
CFLAGS   = -Wall -Wpedantic -Werror -Wextra

.PHONY: all clean format

all: $(EXECBIN)

$(EXECBIN): $(OBJECTS)
	$(CC) -o $@ $^

%.o : %.c %.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXECBIN) $(OBJECTS) *.log mem_size

format: $(FORMATS)

%.fmt: %.c
	$(FORMAT) -i $<
	touch $@

