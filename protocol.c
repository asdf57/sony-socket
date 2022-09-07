#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "protocol.h"
#include "utils.h"

struct Request {
	unsigned char header[2];
	unsigned char type;
	char command[5];
	int parameters;
	unsigned char footer;
};

void *
send_request(void *p_sockfd)
{
	int i, n;
	char cmd[BUFSIZ];
	char *tokens[MAX_TOKENS];
	char request[25];
	time_t ltime;
	struct tm result;
	char stime[32];

	while (1) {
		n = read(STDIN_FILENO, cmd, BUFSIZ);

		if (n <= 0)
			break;

		cmd[n] = '\0';

		tokens[0] = strtok(cmd, " \n\t");
		for (i = 1; tokens[i-1] != NULL; i++) {
			tokens[i] = strtok(NULL, " \n\t");
		}

		if (tokens[0] == NULL) {
			fprintf(stderr, "No tokens detected!\n");
			exit(1);
		}

		if (build_request(tokens, request) < 0)
			continue;

		send(*(int*)p_sockfd, request, 24, 0);

		ltime = time(NULL);
		localtime_r(&ltime, &result);
		asctime_r(&result, stime);

		for (i = 0; stime[i] != '\n'; i++);
		stime[i] = '\0';

		printf("[SEND] %s: %s", stime, request);
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

		printf("[READ] %s: %s", stime, buf);
	}
}

int
get_input_base(char *mode)
{
	if (strcasecmp(mode, "hdmi") == 0)
		return 100000000;
	else if (strcasecmp(mode, "composite") == 0)
		return 300000000;
	else if (strcasecmp(mode, "component") == 0)
		return 400000000;
	return 0;
}

int
build_request(char *tokens[], char *request_str)
{
	Request request;
	char *endptr;
	int mode_id;

	memwrite(request.header, 0x532A);
	request.footer = 0x0A;

	if (strcasecmp(tokens[0], "setIrccCode") == 0) {
		request.type = 'C';
		strcpy(request.command, "IRCC");
		request.parameters = (int)strtol(tokens[1], &endptr, 10);
	}
	else if (strcasecmp(tokens[0], "setPowerStatus") == 0) {
		request.type = 'C';
		strcpy(request.command, "POWR");
		request.parameters = (int)strtol(tokens[1], &endptr, 10);
	}
	else if (strcasecmp(tokens[0], "setAudioVolume") == 0) {
		request.type = 'C';
		strcpy(request.command, "VOLU");
		request.parameters = (int)strtol(tokens[1], &endptr, 10);
	}
	else if (strcasecmp(tokens[0], "setAudioMute") == 0) {
		request.type = 'C';
		strcpy(request.command, "AMUT");
		request.parameters = (int)strtol(tokens[1], &endptr, 10);
	}
	else if (strcasecmp(tokens[0], "setInput") == 0) {
		request.type = 'C';
		strcpy(request.command, "INPT");
		mode_id = (int)strtol(tokens[2], &endptr, 10);
		request.parameters = get_input_base(tokens[1]) + mode_id;
	}
	else if (strcasecmp(tokens[0], "setPictureMute") == 0) {
		request.type = 'C';
		strcpy(request.command, "PMUT");
		request.parameters = (int)strtol(tokens[1], &endptr, 10);
	}
	else if (strcasecmp(tokens[0], "setSceneSetting") == 0) {
		printf("setSceneSetting\n");
	}
	else if (strcasecmp(tokens[0], "togglePowerStatus") == 0) {
		request.type = 'C';
		strcpy(request.command, "TPOW");
		request.parameters = -1;
	}
	else if (strcasecmp(tokens[0], "getPowerStatus") == 0) {
		request.type = 'E';
		strcpy(request.command, "POWR");
		request.parameters = -1;
	}
	else if (strcasecmp(tokens[0], "getAudioVolume") == 0) {
		request.type = 'E';
		strcpy(request.command, "VOLU");
		request.parameters = -1;
	}
	else if (strcasecmp(tokens[0], "getAudioMute") == 0) {
		request.type = 'E';
		strcpy(request.command, "AMUT");
		request.parameters = -1;
	}
	else if (strcasecmp(tokens[0], "getInput") == 0) {
		request.type = 'E';
		strcpy(request.command, "INPT");
		request.parameters = -1;
	}
	else if (strcasecmp(tokens[0], "getPictureMute") == 0) {
		request.type = 'E';
		strcpy(request.command, "PMUT");
		request.parameters = -1;
	}
	else if (strcasecmp(tokens[0], "getBroadcastAddress") == 0) {
		// request.type = 'E';
		// strcpy(request.command, "BADR");
		// request.parameters = -1;
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

	if (request.parameters == -1) {
		sprintf(request_str, "%.2s%c%4s################%c", request.header, request.type, request.command, request.footer);
	}
	else {
		sprintf(request_str, "%.2s%c%4s%016d%c", request.header, request.type, request.command, request.parameters, request.footer);
	}

	return 0;
}
