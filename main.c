#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "protocol.h"

int
main()
{
	int sockfd;
	struct sockaddr_in server;
	pthread_t tid_read, tid_send;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		perror("sock");
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(20060);
	server.sin_addr.s_addr = inet_addr("192.168.1.157");

	if (connect(sockfd, (struct sockaddr *) &server, sizeof(struct sockaddr_in)) < 0) {
		perror("connect");
		exit(EXIT_FAILURE);
	}

	if (pthread_create(&tid_read, NULL, read_request, &sockfd) != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	if (pthread_create(&tid_send, NULL, send_request, &sockfd) != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	if (pthread_join(tid_read, NULL) != 0) {
		fprintf(stderr, "pthread_join: could not join read_request thread\n");
		exit(EXIT_FAILURE);
	}

	if (pthread_join(tid_send, NULL) != 0) {
		fprintf(stderr, "pthread_join: could not join send_request thread\n");
		exit(EXIT_FAILURE);
	}

	if (shutdown(sockfd, SHUT_RDWR) < 0) {
		perror("shutdown");
		exit(EXIT_FAILURE);
	}

	exit(0);
}
