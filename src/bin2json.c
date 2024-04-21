#include <stdio.h>
#include <parse.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s binfile\n", argv[0]);

		return 1;
	}

	FILE *fh = fopen(argv[1], "rb");

	int ret = parseBin(fh);

	fclose(fh);

	switch (ret) {
	case -1:
		fprintf(stderr, "%s\n", "Bad Read");

		break;
	case -2:
		fprintf(stderr, "%s\n", "Bad Type");

		break;
	case -3:
		fprintf(stderr, "%s\n", "Bad Type Read");

		break;
	default:
		fprintf(stdout, "%s", "\n");
	}

	return -ret;
}
