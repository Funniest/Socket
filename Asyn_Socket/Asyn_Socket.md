# Asynchronous Socket
### 비동기 소켓이란?
지금까지쓰레드, 프로세스, 에코서버 등 올린 것들은 동기 서버입니다.

동기가 뭐냐 하면, 예를 들어 socket에서 Read/Recv를 하면 Send가 올때까지 기다리는 것을 동기라고 합니다.

비동기는 Read/Recv에서 send가 오지 않으면 그냥 다른 명령어를 실행하다 Send신호가 오면 그 때 Read/Recv를 합니다.

예는 아래 그림 처럼 동기 방법일때는 Recv에서 Send가 올때까지 기다렸는데, 비동기는 그렇지 않고

반복문을 계속 반복하다 도중에 Client가 Send를 보내면 데이터를 받습니다.

![Alt text](https://github.com/Funniest/study/blob/master/Socket/Asyn_Socket/img/Asyn.PNG)

사용할 함수는 #include <fcnt.h>에있는 fcntl()함수를 사용하여 non-blocking 소켓 프로그래밍을 할 것입니다.

### 소스코드
Server는 Ubuntu 32bit nano에디터로 작성되었습니다.

Client는 Visual studio 2015환경에서 작성되었습니다.
#### Server
```
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 9999
#define MAX_SIZE 128

void ErrorMessage(char *str){
	perror(str);
	exit(1);
}

int set_nonblock_socket(int fd){
	int flags;
	flags = fcntl(fd, F_GETFL, 0);
	if(flags == -1){
		perror("fcntl error");
		flags = 0;
	}
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main(int argc, char *argv[]){
	int server_fd, client_fd;
	int client_size, n;
	struct sockaddr_in server_addr, client_addr;
	
	char message[MAX_SIZE] = {0,};
	
	if((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		ErrorMessage("Socket Error");
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family =  AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);
	
	if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		ErrorMessage("Bind Errror");
	
	if(listen(server_fd, 5) == -1)
		ErrorMessage("Listen Error");
	
	client_size = sizeof(client_addr);
	while(1){
		printf("Ready?\n");
		memset(message, 0, MAX_SIZE);
		client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_size);
		set_nonblock_socket(client_fd);
		printf("Accept\n");
		while(1){
			if((n = read(client_fd, message, MAX_SIZE)) < 0){
				if(errno == EAGAIN){
				}
				else{
					printf("read Error : %s\n", errno);
					close(client_fd);
					break;
				}
			}
			else if(n == 0){
				printf("close %d\n",errno);
				close(client_fd);
				break;
			}
			else{
				printf("Read Data %s\n", message);
				if(!strcmp(message, "exit"))
					break;
				memset(message, 0, strlen(message));
			}
		}
		close(client_fd);
	}
	close(server_fd);
	return 0;
}

```
####Client
```
#include <stdio.h>
#include <winsock2.h>
#include <string.h>

#define IP "192.168.111.136"
#define PORT 9999

void ErrorMessage(char *str) {
	printf("%s\n", str);
	exit(1);
}

int main(int argc, char *args[]) {
	WSADATA wsadata;
	SOCKET sock;
	SOCKADDR_IN sockAddr;

	char message[128] = { 0, };
	
	if (WSAStartup(MAKEWORD(2, 2), &wsadata))
		ErrorMessage("WSAStartup Error!");

	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.S_un.S_addr = inet_addr(IP);
	sockAddr.sin_port = htons(PORT);

	if (connect(sock, (SOCKADDR *)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
		ErrorMessage("Connect Error!");
	
	while (1) {
		printf("send server (exit is \"exit\"): ");
		scanf("%s", message);
		send(sock, message, strlen(message), 0);
		if (!strcmp(message, "exit")) {
			break;
		}
	}
	closesocket(sock);
	WSACleanup();

	return 0;
}
```
