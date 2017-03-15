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
