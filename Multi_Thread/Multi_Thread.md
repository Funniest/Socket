# Multi Thread
### 멀티 쓰레드 소켓
멀티 프로세스 소켓에 이어서 1 : * 연결을 할 수 있는 방법 중 하나입니다.

멀티 프로세스 소켓은 아래 그림 처럼 Client마다 하나의 프로세스가 생성되어 처리하게 됩니다.

이렇게 된다면 운영체제 입장에서 클라이언트 요청 하나당 하나의 프로세스를 생성하는 것은 비효율적입니다.

왜 부담이 되냐면, 운영체제에서 컨텍스트 스위칭이 일어나는데 이 작업은 프로세스가 많으면 많을 수록 부담을 주기 때문입니다.

그만큼 CPU할당 시간을 짧게 나누어서 프로세스 하나마다 주어야하고, 컨텍스트 스위칭이 그만큼 많이 일어나기 때문입니다.

![Alt text](https://github.com/Funniest/study/blob/master/Socket/Multi_Thread/img/Process.PNG)

반면, 멀티 쓰레드로 클라이언트를 처리하게 되면, 프로세스를 생성하는 것 보다 효율 적으로 처리할 수 있습니다.

왜냐하면, 프로세스보다 컨텍스트 스위칭의 과정이 간단하게 처리 되기 때문입니다.

그리고 생성 시간도 프로세스 생성 시간보다 몇십에서 최대 백배 정도 빠르다고 합니다.

아래 그림을 보면 알 수 있듯이 프로세스를 새로 생성하는게 아니라 프로세스 내에 쓰레드를 생성해서 요청을 처리합니다.

![Alt text](https://github.com/Funniest/study/blob/master/Socket/Multi_Thread/img/Thread.PNG)

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
#include <pthread.h>

#define MAX_SIZE 124
#define PORT 9999

void ErrorMessage(char *str){
	printf("%s\n", str);
	exit(1);
}

void *myFunc(void *arg){
	int fd;
	char message[MAX_SIZE] = {0,};
	
	fd = *((int *)arg);
	
	while(1){
		if(read(fd, message, MAX_SIZE) <= 0)
			ErrorMessage("Read Error!");
		
		if(!strcmp(message, "exit"))
			break;

		if(write(fd, message, strlen(message)) <= 0)
			ErrorMessage("Write Error!");
		
		printf("Client : %s\n", message);
		memset(message, 0, strlen(message));
	}
	close(fd);
}

int main(int argc, char *argv[]){
	int server_fd, client_fd;
	int client_len;
	struct sockaddr_in server_addr, client_addr;
	int thread_id;
	char message[MAX_SIZE] = {0,};

	if((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		ErrorMessage("Socekt Error!");
	printf("socket\n");

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);
	
	if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		ErrorMessage("Bind Erorr!");

	if(listen(server_fd, 5) == -1)
		ErrorMessage("Listen Error!");
	printf("Listen\n");

	client_len = sizeof(client_addr);
	while(1){
		client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
		if(client_fd == -1)
			ErrorMessage("Accept Error!");
		thread_id = pthread_create(&thread_id, NULL, myFunc, (void *)&client_fd);
		if(thread_id != 0)
			ErrorMessage("Thread Erorr!");
		pthread_detach(thread_id);
	}

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

[참고]http://iblog.or.kr/hungi/it/software/programing/737
