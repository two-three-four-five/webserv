#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <map>
#include <vector>

#define BUF_SIZE 100
#define MAX_EVENTS 50
void error_handling(char *buf);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_sz;
	int str_len, i;
	char buf[BUF_SIZE];

	struct kevent *change_list;
	struct kevent event_list[MAX_EVENTS];
	struct kevent event;
	int kq, fd, ret;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	kq = kqueue();
	if (kq == -1)
		error_handling("kqueue() error");
	EV_SET(&event, serv_sock, EVFILT_READ, EV_ADD, 0, 0, NULL);
	ret = kevent(kq, &event, 1, NULL, 0, NULL);

	while (1)
	{
		int events = kevent(kq, NULL, 0, event_list, MAX_EVENTS, NULL);
		if (events == -1)
		{
			puts("kevent() error");
			break;
		}

		for (int i=0; i<events; i++)
		{
			if (event_list[i].ident == serv_sock)
			{
				adr_sz = sizeof(clnt_adr);
				clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
				EV_SET(&event, clnt_sock, EVFILT_READ, EV_ADD, 0, 0, NULL);
				ret = kevent(kq, &event, 1, NULL, 0, NULL);
				printf("connected clinet: %d\n", clnt_sock);
			}
			else
			{
				str_len = read(event_list[i].ident, buf, BUF_SIZE);
				if (str_len == 0)
				{
					EV_SET(&event, event_list[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
					ret = kevent(kq, &event, 1, NULL, 0, NULL);
					close(event_list[i].ident);
					printf("closed client: %d\n", event_list[i].ident);
				}
				else
				{
					write(event_list[i].ident, buf, str_len);
				}
			}
		}
	}
	close(serv_sock);
	close(kq);
	return (0);
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}

