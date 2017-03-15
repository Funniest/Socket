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
