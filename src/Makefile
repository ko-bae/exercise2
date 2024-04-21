binmake: bin2json.c parse.c
	$(CC) -o bin2json bin2json.c parse.c

test:
	tests/run a
	tests/run b
	tests/run c
	tests/run d
	tests/run e

clean:
	rm -f bin2json
