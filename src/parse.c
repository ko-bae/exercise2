#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define  TYPE_UNKNOWN      0x00
#define  TYPE_OBJECT       0x01
#define  TYPE_ARRAY        0x02
#define  TYPE_STRING       0x03
#define  TYPE_NUMBER_INT   0x04
#define  TYPE_NUMBER_FLOAT 0x05
#define  TYPE_BOOL_TRUE    0x06
#define  TYPE_BOOL_FALSE   0x07
#define  TYPE_NULL         0x08

const int BAD_READ = -1;
const int BAD_TYPE = -2;
const int BAD_READ_TYPE = -3;

int parseBin(FILE*);
int parseObject(FILE*);

int readLen(FILE *fh, uint32_t *len) {
	uint8_t buf[4];

	if (!fread(buf, 4, 1, fh) && ferror(fh)) {
		return BAD_READ;
	}

	*len = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];

	return 0;
}

int parseString(FILE *fh) {
	uint32_t len = 0;

	int ret = readLen(fh, &len);
	if (ret != 0) {
		return ret;
	}

	char *buf = malloc(len);

	if (!fread(buf, len, 1, fh) && ferror(fh)) {
		return BAD_READ;
	}

	fputs("\"", stdout);

	for (int i = 0; i < len; i++) {
		switch (buf[i]) {
		case '\n':
			fputs("\\n", stdout);
		case '\\':
		case '\"':
			fputs("\\", stdout);
		default:
			fprintf(stdout, "%c", buf[i]);
		}
	}

	fputs("\"", stdout);

	return 0;
}

int readUint64(FILE *fh, uint64_t *num) {
	uint8_t buf[8];

	if (!fread(buf, 8, 1, fh) && ferror(fh)) {
		return BAD_READ;
	}

	*num = ((uint64_t)(buf[0]) << 56) | ((uint64_t)(buf[1]) << 48) | ((uint64_t)(buf[2]) << 40) | ((uint64_t)(buf[3]) << 32) | ((uint64_t)(buf[4]) << 24) | ((uint64_t)(buf[5]) << 16) | ((uint64_t)(buf[6]) << 8) | (uint64_t)(buf[7]);

	return 0;
}

int parseNumberInt(FILE *fh) {
	uint64_t num = 0;

	int ret = readUint64(fh, &num);
	if (ret != 0) {
		return ret;
	}

	fprintf(stdout, "%ld", (int64_t)num);

	return 0;
}

int parseNumberFloat(FILE *fh) {
	uint64_t num = 0;

	int ret = readUint64(fh, &num);
	if (ret != 0) {
		return ret;
	}

	fprintf(stdout, "%lf", *((double*)(void*)&num));

	return 0;
}

int parseArray(FILE *fh) {
	uint32_t len = 0;

	int ret = readLen(fh, &len);
	if (ret != 0) {
		return ret;
	}

	fputs("[", stdout);

	for (int i = 0; i < len; i++) {
		if (i) {
			fprintf(stdout, "%s", ",");
		}

		int ret = parseBin(fh);
		if (ret != 0) {
			return ret;
		}
	}

	fputs("]", stdout);

	return 0;
}

int parseObject(FILE *fh) {
	uint32_t len = 0;

	int ret = readLen(fh, &len);
	if(ret != 0) {
		return ret;
	}

	fputs("{", stdout);

	for (int i = 0; i < len; i++) {
		if (i) {
			fprintf(stdout, "%s", ",");
		}

		int ret = parseString(fh);
		if (ret != 0) {
			return ret;
		}

		fprintf(stdout, "%s", ":");

		ret = parseBin(fh);
		if (ret != 0) {
			return ret;
		}
	}

	fputs("}", stdout);

	return 0;
}

int parseBin(FILE *fh) {
	uint8_t type = TYPE_UNKNOWN;

	if (!fread(&type, 1, 1, fh) && ferror(fh)) {
		return BAD_READ_TYPE;
	}

	switch (type) {
	case TYPE_OBJECT:
		return parseObject(fh);
	case TYPE_ARRAY:
		return parseArray(fh);
	case TYPE_STRING:
		return parseString(fh);
	case TYPE_NUMBER_INT:
		return parseNumberInt(fh);
	case TYPE_NUMBER_FLOAT:
		return parseNumberFloat(fh);
	case TYPE_BOOL_TRUE:
		fputs("true", stdout);

		return 0;
	case TYPE_BOOL_FALSE:
		fputs("false", stdout);

		return 0;
	case TYPE_NULL:
		fputs("null", stdout);

		return 0;
	}

	return BAD_TYPE;
}
