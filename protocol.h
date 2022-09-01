typedef struct Request Request;

int build_request(char*[], Request *);
int send_request(char *[]);

enum {
	SUCCESS = 0xffffffffffffffff,
	ERROR = 0x0,
	HEADER = 0x532A,
};