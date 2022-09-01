#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include "protocol.h"

enum {
	MAX_TOKENS = 128
};

int
main()
{
	int i, n;
	char cmd[BUFSIZ];
	char *tokens[MAX_TOKENS];

	n = read(STDIN_FILENO, cmd, BUFSIZ);
	cmd[n] = '\0';

	tokens[0] = strtok(cmd, " \n\t");
	for (i = 1; tokens[i] != NULL; i++) {
		tokens[i] = strtok(NULL, " \n\t");
	}

	if (tokens[0] == NULL) {
		fprintf(stderr, "No tokens detected!\n");
		exit(1);
	}

	send_request(tokens);

	exit(0);
}
