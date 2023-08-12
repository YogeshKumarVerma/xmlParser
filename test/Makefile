CC       = gcc
CFLAGS   = -g -Wall -Wimplicit -std=c99
DEFINES  = -DPRIVATE=static

LDFLAGS  = -L.
AR_FLAGS = -rcs
AR       = ar $(AR_FLAGS)
BIN_NAME = xmlUtil.elf
OBJ_DIR  = obj
BIN_DIR  = bin
ROOT_DIR = ./$(PWD)/..
MY_DIR   = ./$(PWD)/..

SRC_DIR  = $(MY_DIR)/src \
	       $(MY_DIR)/test

INC_DIR  = $(MY_DIR)/inc

INCLUDE  = $(addprefix -I, $(INC_DIR)) \
           -I/usr/include/libxml2/

C_FILES  = $(MY_DIR)/src/xml_parser.c \
           $(MY_DIR)/test/test.c
#$(shell find $(SRC_DIR) -name '*.c')

H_FILES  = $(MY_DIR)/inc/xml_parser.h
#$(shell find $(INC_DIR) -name '*.h')

OBJECTS  = $(patsubst %.c, %.o, $(C_FILES))
LIBS     = -lxml2

vpath %.c $(SRC_DIR)

default: $(BIN_NAME)

clean:
	rm -rf $(OBJECTS)

cleanall:
	rm -rf $(OBJECTS)
	rm -rf $(BIN_NAME)

%.c:

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFINES) -c -o $@ $?

$(BIN_NAME): $(OBJECTS) $(H_FILES)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@ $(LIBS)
	@#$(AR) $@ $(OBJECTS)			