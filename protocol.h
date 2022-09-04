typedef struct Request Request;

int build_request(char*[], Request *);
void * send_request();
void * read_request(void *);

enum {
	MAX_TOKENS = 128,
	SUCCESS = 0xffffffffffffffff,
	ERROR = 0x0,
	HEADER = 0x532A,
};