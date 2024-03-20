CFLAGS=-Wall -Wextra
MAIN=main.c
OUT=build/watchd

FILE=/abs/path/to/file

all: clean out run


clean:
	rm -f ${OUT}

out:
	gcc ${CFLAGS} ${MAIN} -o ${OUT}

run:
	./${OUT} ${FILE}
