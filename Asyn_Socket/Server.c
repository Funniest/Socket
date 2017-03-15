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
