CFLAGS=-Wall -Wextra
MAIN=src/main.c
OUT=build/watchd

DIR=$(PWD)/test/watch_this_dir
FILE=${DIR}/file

all: clean out run


clean:
	rm -f ${OUT}

out:
	gcc ${CFLAGS} ${MAIN} -o ${OUT}

run:
	./${OUT} ${FILE}	
