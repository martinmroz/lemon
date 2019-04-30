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

C_OBJS = $(C_SRCS:.c=.o)

%.o: %.c
	cc $(CFLAGS) -c -o $@ $^

lemon: $(C_OBJS)
	cc -o $@ $^

clean:
	rm -f $(C_OBJS)
	rm -f lemon
