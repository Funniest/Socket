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
