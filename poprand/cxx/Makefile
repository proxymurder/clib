CBUILD=build

all: cleanup build crun

build:
	g++ main.cpp -o $(CBUILD)

cleanup:
	rm -f $(CBUILD)

crun:
	./$(CBUILD)