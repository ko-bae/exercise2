bin2json
========

This simple C program converts a binary encoded JSON file into textual JSON.

Build
=====

To build simply run:

```bash
make
```

…to test the built executable you can use the following:

```bash
make test
```

Example
=======

The `example` directory contains a binary encoded JSON file containing movie data from the 2010's.

You can use `bin2json` to convert it into normal JSON using the following command:

```bash
bin2json movies.bin > movies.json
```
