CBUILD=build

all: cleanup build crun

build:
	gcc main.c -o $(CBUILD)

cleanup:
	rm -f $(CBUILD)

crun:
	./$(CBUILD)