# Multi Process
### 멀티 프로세스 소켓
지금까지 만든 2개의 프로젝트는 1 : 1 연결을 하였습니다.

이번에는 조금 더 나아가 다중으로 1 : * 연결을 해보겠습니다.

1 : * 연결을 하기위해서는 대표적으로 쓰레드와, 프로세스로 하는  방법등이있는데,

먼저 멀티 프로세스를 이용하는 방법을 보겠습니다.

멀티 프로세스 소켓이 돌아가는 것을 간단히 그려보았습니다.

아래 그림처럼 클라이언트에서 요청이 들어오면 새로 프로세스를 만들어서 요청을 처리하는 구조입니다.

![Alt text](https://github.com/Funniest/Socket/blob/master/Multi_Process/img/Multi_Process.PNG)

Echo의 클라이언트를 사용하였습니다.

### 소스코드
Server는 Ubuntu 32bit nano에디터로 작성되었습니다.

Client는 Visual studio 2015환경에서 작성되었습니다.

#### Server
```
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 124
#define IP "127.0.0.1"
#define PORT 9999

void ErrorMessage(char *str){
	printf("%s\n", str);
	exit(1);
}

int main(int argc, char *argv[]){
	int server_fd, client_fd;
	int client_len;
	int pid;

	struct sockaddr_in client_addr, server_addr;
	
	char message[MAX_SIZE] = {0,};
	
	if((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		ErrorMessage("Socket Error!");

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		ErrorMessage("Bind Error!");

	if(listen(server_fd, 5) == -1)
		ErrorMessage("Listen Error!");
	printf("listen!\n");
	//SIGCHLD: child가stop or exit되었을때parent에게전달되는신호(ignore)
	//child 프로세스가 종료되었을 때 좀비가 되는 것을 방지 하는 코드
	signal(SIGCHLD, SIG_IGN);

	client_len = sizeof(client_addr);
	printf("sizeof \n");
	while(1){
		client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
		if(client_fd == -1)
			ErrorMessage("Accept Error!");
		printf("client accept!\n");
		pid = fork();
		if(pid == 0) {
			while(1){
				memset(message, 0, MAX_SIZE);
				if(read(client_fd, message, MAX_SIZE - 1) <= 0){
					close(client_fd);
					exit(0);
				}
				if(!strcmp(message, "exit")){
					close(client_fd);
					break;
				}
				
				printf("Message : %s\n", message);
				write(client_fd, message, strlen(message));
			}
			break;
		}
		if(pid == -1){
			ErrorMessage("Fork Error!");
		}
	}
	if(pid != 0)
		close(server_fd);
}
```

#### Client
```
#include <stdio.h>
#include <winsock2.h>
#include <string.h>

#define IP "192.168.111.132"
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
		
		if (!strcmp(message, "exit"))
			break;

		memset(message, 0, sizeof(message));
		if (recv(sock, message, sizeof(message) - 1, 0) == -1)
			ErrorMessage("Recv Error!");

		printf("Message from server: %s \n", message);
		memset(message, 0, sizeof(message));
	}

	closesocket(sock);
	WSACleanup();

	return 0;
}
```
