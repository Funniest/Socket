#Echo Server
###에코 서버란?
앞서 Hello Socket으로 그냥 Socket이 무엇인지 간단히 보았습니다.

Hello Socket의 소스코드는 단순히 서버를 열고 클라이언트가 보여주면, "Hello Client"를 보내주고 종료합니다.

이번에는 조금 더 상향된 간단한 Echo server를 제작해 보았습니다.

Echo란 아래 그림처럼 이야기를 하면 돌아오는 느낌입니다.

![Alt text](https://github.com/Funniest/study/blob/master/Socket/Echo_Socket/img/echo.PNG)

Hello Socket의 소스와 달라진 부분은 크게 없습니다.

###소스코드
본 프로그램은 Visual studio 2015환경에서 작성되었습니다.
####Server
```
#include <stdio.h>
#include <winsock2.h>
#include <string.h>

#define PORT 9999

void ErrorMessage(char *str) {
	printf("%s\n", str);
	exit(1);
}

int main(int argc, char *args[]) {
	WSADATA wsadata;
	SOCKET serverSock, clientSock;
	SOCKADDR_IN serverAddr, clientAddr;
	int clientSize = 0, recvSize = 0;
	char message[128] = { 0, };

	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
		ErrorMessage("WSAStartup Error!");

	serverSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(PORT);

	if (bind(serverSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		ErrorMessage("Bind Error!");

	if (listen(serverSock, 5) == SOCKET_ERROR)
		ErrorMessage("Listen Error!");

	clientSize = sizeof(clientAddr);
	clientSock = accept(serverSock, (SOCKADDR *)&clientAddr, &clientSize);

	if (clientSock == INVALID_SOCKET)
		ErrorMessage("Accept Error!");

	while (1) {
		recvSize = recv(clientSock, message, sizeof(message) - 1, 0);
		if (recvSize == -1)
			break;

		if (!strcmp(message, "q") || !strcmp(message, "Q"))
			break;

		send(clientSock, message, strlen(message), 0);
		memset(message, 0, sizeof(message));
	}
	strcpy(message, "Hello Client!");
	send(clientSock, message, sizeof(message), 0);

	closesocket(clientSock);
	closesocket(serverSock);
	WSACleanup();
	return 0;
}
```

####Client
```
#include <stdio.h>
#include <winsock2.h>
#include <string.h>

#define IP "127.0.0.1"
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
		printf("send server (q or Q exit): ");
		scanf("%s", message);
		send(sock, message, sizeof(message) - 1, 0);
		
		if (!strcmp(message, "q") || !strcmp(message, "Q"))
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
