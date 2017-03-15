<<<<<<< HEAD
tset
=======
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
>>>>>>> 73417e4a94a5bf7976a676893b1a05b2908239a2
