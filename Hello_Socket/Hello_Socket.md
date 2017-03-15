#Socket Programming
###소켓 프로그래밍?
두대 이상의 컴퓨터 간의 통신을 할수있게 하는 프로그래밍이라고 생각합니다.

이런 소켓들의 구현은 어떻게 해야할까요?

C언어의 소켓 구현은 아래 그림과 같이 서버, 클라 모두 Socket을 생성한 뒤

서버는 Bind를 통하여 서버로 등록 한 뒤 Listen으로 접속요청을 확인합니다.

그 후 클라이언트가 들어올 때 까지 기다립니다.

클라이언트는 서버의 주소를 입력하여 Connect하고, 서버는 요청을 Accept하면 본격 적으로 서버와 클라이언트의 sned/recv가 이루어집니다.

할 일이 끝나면 서버와 클라이언트는 close합니다

우측 서버, 좌측 클라이언트

![Alt text](https://github.com/Funniest/study/blob/master/Socket/Hello_Socket/img/Server_Client.PNG)

[참고]http://forum.falinux.com/zbxe/index.php?document_srl=429387&mid=C_LIB


1 : 1로 통신을 하는 간단한 서버와 클라이언트를 코딩하여 보았습니다.

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
	int clientSize = 0;
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

	if (recv(sock, message, sizeof(message) - 1, 0) == -1)
		ErrorMessage("Recv Error!");

	printf("Message from server: %s \n", message);

	closesocket(sock);
	WSACleanup();

	return 0;
}
```
