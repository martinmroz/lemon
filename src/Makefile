all: lemon

CFLAGS += -Wall
CFLAGS += -Wextra
# CFLAGS += -g
# CFLAGS += -O

C_SRCS = \
	action.c \
	build.c \
	configlist.c \
	error.c \
	main.c \
	msort.c \
	option.c \
	parse.c \
	plink.c \
	report.c \
	set.c \
	table.c \

INSTALL_DIR = /usr/local
INSTALL_DIR_BIN = $(INSTALL_DIR)/bin
INSTALL_DIR_SHARE = $(INSTALL_DIR)/share/lemon

C_OBJS = $(C_SRCS:.c=.o)

%.o: %.c
	cc $(CFLAGS) -c -o $@ $^

lemon: $(C_OBJS)
	cc -o $@ $^

install: lemon
	mkdir -p $(INSTALL_DIR) $(INSTALL_DIR_BIN) $(INSTALL_DIR_SHARE)
	cp lemon $(INSTALL_DIR_BIN)
	cp ../templates/skeleton.c $(INSTALL_DIR_SHARE)

clean:
	rm -f $(C_OBJS)
	rm -f lemon
