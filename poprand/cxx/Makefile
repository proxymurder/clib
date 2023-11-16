CBUILD=build

all: cleanup build crun

build:
	gcc main.cpp -o $(CBUILD)

cleanup:
	rm -f $(CBUILD)

crun:
	./$(CBUILD)