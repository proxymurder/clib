CFLAGS=-Wall -Wextra
MAIN=main.c
OUT=build/watchd

DIR=$(PWD)/watchdir
FILE=${DIR}/watchfile

all: clean out run


clean:
	rm -f ${OUT}

out:
	gcc ${CFLAGS} ${MAIN} -o ${OUT}

run:
	./${OUT} ${FILE}	
