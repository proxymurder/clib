OUT=xor
MAIN=main.c

all: cleanup build run

cleanup:
	rm -f ${OUT}

build:
	gcc -Wall -Wextra -o ${OUT} ${MAIN}

run:
	./${OUT}
