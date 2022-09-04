#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "protocol.h"

struct Request {
	unsigned char header[2];
	unsigned char type;
	int command;
	char parameters;
};

void *
send_request()
{
	int i, n;
	char cmd[BUFSIZ];
	char *tokens[MAX_TOKENS];
	Request request;

	while (1) {
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

		if (build_request(tokens, &request) < 0)
			break;
	}
}

void *
read_request(void *p_sockfd)
{
	int i, n;
	char buf[BUFSIZ];
	time_t ltime;
	struct tm result;
	char stime[32];

	while (1) {
		n = recv(*(int*)p_sockfd, buf, BUFSIZ-1, 0);
		buf[n] = '\0';
		ltime = time(NULL);
		localtime_r(&ltime, &result);
		asctime_r(&result, stime);

		for (i = 0; stime[i] != '\n'; i++);
		stime[i] = '\0';

		printf("[READ] %s: %s\n", stime, buf);
	}
}

int
build_request(char *tokens[], Request *p_request)
{
	if (strcasecmp(tokens[0], "setIrccCode") == 0) {
		printf("setIrccCode\n");
	}
	else if (strcasecmp(tokens[0], "setPowerStatus") == 0) {
		printf("setPowerStatus\n");
	}
	else if (strcasecmp(tokens[0], "setIrccCode") == 0) {
		printf("setIrccCode\n");
	}
	else if (strcasecmp(tokens[0], "setAudioVolume") == 0) {
		printf("setAudioVolume\n");
	}
	else if (strcasecmp(tokens[0], "setAudioMute") == 0) {
		printf("setAudioMute\n");
	}
	else if (strcasecmp(tokens[0], "setInput") == 0) {
		printf("setInput\n");
	}
	else if (strcasecmp(tokens[0], "setPictureMute") == 0) {
		printf("setPictureMute\n");
	}
	else if (strcasecmp(tokens[0], "setSceneSetting") == 0) {
		printf("setSceneSetting\n");
	}
	else if (strcasecmp(tokens[0], "togglePowerStatus") == 0) {
		printf("togglePowerStatus\n");
	}
	else if (strcasecmp(tokens[0], "getPowerStatus") == 0) {
		printf("getPowerStatus\n");
	}
	else if (strcasecmp(tokens[0], "getAudioVolume") == 0) {
		printf("getAudioVolume\n");
	}
	else if (strcasecmp(tokens[0], "getAudioMute") == 0) {
		printf("getAudioMute\n");
	}
	else if (strcasecmp(tokens[0], "getInput") == 0) {
		printf("getInput\n");
	}
	else if (strcasecmp(tokens[0], "getPictureMute") == 0) {
		printf("getPictureMute\n");
	}
	else if (strcasecmp(tokens[0], "getBroadcastAddress") == 0) {
		printf("getBroadcastAddress\n");
	}
	else if (strcasecmp(tokens[0], "getMacAddress") == 0) {
		printf("getMacAddress\n");
	}
	else if (strcasecmp(tokens[0], "getSceneSetting") == 0) {
		printf("getSceneSetting\n");
	}
	else {
		fprintf(stderr, "Invalid command detected\n");
		return -1;
	}
}
