#include <stdio.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>	   //write

#define PORT_NUMBER 60038

int main(int argc, char *argv[])
{

	int socket_desc, client_sock, c, read_size; //socket_desc is the variable where the socket is held.Client_sock is the variable where the client_socket is kept and read_size is the variable that holds the size of the message from the client
	struct sockaddr_in server, client; // struct structure that holds the addresses of the server and client addresses
	char client_message[20];// keeps the message from clinet

	//Variables created to use the Popen function
	FILE *command;
	FILE *command2;
	//variables created to send the correct answer resulting from the popen function to the client
	char answer[13];
	char answer2[12];
	char answer3[] = " ";

	// Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket"); // error when socket is not created
	}
	puts("Socket created");

	// Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT_NUMBER);

	// Bind
	if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		// print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");

	// Listen
	listen(socket_desc, 3);

	// Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

	// accept connection from an incoming client
	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	puts("Connection accepted");

	// Receive a message from client

	while ((read_size = recv(client_sock, client_message, 20, 0)) > 0) //loop created to continuously receive requests from the client we connect with telnet
	{

		if (strlen(client_message) == 17) //We grouped the requests according to the size of the request we received from the client
		{

			if (strncmp(client_message, "GET_DAY_OF_WEEK", 15) == 0) // and here we checked if client_message and get_day_of_week are equal as strings
			{
				command = popen("date +%A", "r");//According to the client's request, we have sent the necessary parameters to the popen function.
				while (fgets(answer, 13, command) != NULL)
				{
					send(client_sock, answer, strlen(answer), 0);// send the correct answer to the client side with send
				}
				//We reset the answer and client_message that we will send to the user every time so that there is no confusion later.
				memset(&answer[0], '\0', sizeof(answer));
				pclose(command);//close command
				memset(&client_message[0], '\0', sizeof(client_message));
			}
			else
			{
			// MESSAGE SENT TO THE CLIENT WHEN  ENTERED THE INCORRECT REQUEST
				send(client_sock, "INCORRECT REQUEST\n", 19, 0);
				memset(&client_message[0], '\0', sizeof(client_message));
			}
		}
		else if (strlen(client_message) == 10)
		{

			if (strncmp(client_message, "GET_DATE", 8) == 0) // and here we checked whether client_message and getDate are equal as strings
			{
				command = popen("date +%D", "r");//According to the client's request, we have sent the necessary parameters to the popen function.
				while (fgets(answer, 13, command) != NULL)
				{
					send(client_sock, answer, strlen(answer), 0);// send the correct answer to the client side with send
				}
				memset(&answer[0], '\0', sizeof(answer));
				pclose(command);
				memset(&client_message[0], '\0', sizeof(client_message));
			}

			else if (strncmp(client_message, "GET_TIME", 8) == 0) // and here we checked whether client_message and getTime are equal as strings
			{

				command = popen("date +%r", "r");//According to the client's request, we have sent the necessary parameters to the popen function.
				while (fgets(answer, 13, command) != NULL)
				{
					send(client_sock, answer, strlen(answer), 0);
				}
				memset(&answer[0], '\0', sizeof(answer));
				pclose(command);
				memset(&client_message[0], '\0', sizeof(client_message));
			}
			else
			{

			// MESSAGE SENT TO THE CLIENT WHEN  ENTERED THE INCORRECT REQUEST
				send(client_sock, "INCORRECT REQUEST\n", 19, 0);
				memset(&client_message[0], '\0', sizeof(client_message));
			}
		}
		else if (strlen(client_message) == 15)
		{

			

			if (strncmp(client_message, "GET_TIME_DATE", 13) == 0)// and here we checked whether client_message and getTimeDate are equal as strings
			{

				command = popen("date +%r", "r");
				command2 = popen("date +%D", "r");
				while (fgets(answer, 25, command) != NULL)
				{
				}
				answer[strlen(answer) - 1] = '\0';
				while (fgets(answer2, 12, command2) != NULL)
				{
				}

				strcat(answer, answer3);
				strcat(answer, answer2);
				send(client_sock, answer, strlen(answer), 0);
				memset(&answer[0], '\0', sizeof(answer));
				pclose(command);
				memset(&client_message[0], '\0', sizeof(client_message));
			}
			else if (strncmp(client_message, "GET_TIME_ZONE", 13) == 0)// and here we checked whether client_message and getTimeZone are equal as strings
			{
				

				command = popen("date +%:z", "r");
				while (fgets(answer, 13, command) != NULL)
				{
					send(client_sock, answer, strlen(answer), 0);
				}
				memset(&answer[0], '\0', strlen(answer));
				pclose(command);
				memset(&client_message[0], '\0', sizeof(client_message));
			}
			else
			{
				
			// MESSAGE SENT TO THE CLIENT WHEN  ENTERED THE INCORRECT REQUEST
				send(client_sock, "INCORRECT REQUEST\n", 19, 0);
				memset(&client_message[0], '\0', sizeof(client_message));
			}
		}
		else if (strlen(client_message) == 14)
		{

			if (strncmp(client_message, "CLOSE_SERVER", 12) == 0)// and here we checked whether client_message and close server are equal as strings
			{

				send(client_sock, "GOOD BYE\n", 9, 0);
				memset(&client_message[0], '\0', sizeof(client_message));
				//closing sockets so that the port can be used again
				close(socket_desc);
				close(client_sock);
			}
			break;
		}
		else if ((strlen(client_message) != 15) && (strlen(client_message) != 10) && (strlen(client_message) != 17) && (strlen(client_message) != 14))
		{
			// MESSAGE SENT TO THE CLIENT WHEN  ENTERED THE INCORRECT REQUEST
			send(client_sock, "INCORRECT REQUEST\n", 19, 0);
			memset(&client_message[0], '\0', sizeof(client_message));
		}
	}

	if (read_size == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if (read_size == -1)
	{
		perror("recv failed");
	}
	//closing sockets so that the port can be used again
	close(socket_desc); 
	close(client_sock);

	return 0;
}
