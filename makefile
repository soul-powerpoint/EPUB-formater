# CC = gcc
# CFLAGS = -Wall -Wextra -O2
# TARGET = formater

# SRC = ./src/formater.c
# OBJ = $(SRC:.c=.o)

# XML2_CFLAGS := $(shell pkg-config --cflags libxml-2.0)
# XML2_LIBS   := $(shell pkg-config --libs libxml-2.0)

# LDFLAGS = -lzip $(XML2_LIBS)

# $(TARGET): $(OBJ)
# 	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

# %.o: %.c
# 	$(CC) $(CFLAGS) $(XML2_CFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJ) $(TARGET)

CC       := gcc
CFLAGS   := -Wall -Wextra -O2
CPPFLAGS :=
DEPS   := $(OBJ:.o=.d)

# Target
TARGET := formater

SRC := $(wildcard src/*.c)
OBJ := $(SRC:.c=.o)

XML2_CFLAGS := $(shell pkg-config --cflags libxml-2.0 2>/dev/null)
XML2_LIBS   := $(shell pkg-config --libs   libxml-2.0 2>/dev/null)
ZIP_CFLAGS  := $(shell pkg-config --cflags libzip       2>/dev/null)
ZIP_LIBS    := $(shell pkg-config --libs   libzip       2>/dev/null)

CPPFLAGS += $(XML2_CFLAGS) $(ZIP_CFLAGS)

LDFLAGS  :=
LDLIBS   := $(XML2_LIBS) $(ZIP_LIBS)

ifeq ($(strip $(ZIP_LIBS)),)
  LDLIBS += -lzip
endif

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
# 	rm -f $(OBJ) $(TARGET)
	$(RM) $(OBJ) $(DEPS) $(TARGET)
