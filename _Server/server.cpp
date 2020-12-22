#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#define MAX_PACKET 0xff	//max TCP packets we can process
#define MAX_SOCKET 0x10	//max TCP clients allowed online

#define FLAG_QUIT 0x0000
#define FLAG_HELLO 0x0001

//------------------------------
//Following walkthrough of Stephen Meier SDL2.0 Tutorial-04: Networking 1
//url: https://stephenmeier.net/2015/12/23/sdl-2-0-tutorial-04-networking/

typedef struct {
	int in_use;	//if client is being used by an active socket
	int playing;	//boolean if the client is playing the game or not
	//other client variables if needed such as XPos, YPos, health? doors open, doors closed, etc.
} Client;

typedef struct {
	TCPsocket socket = NULL; // The socket identifier for hacker
} Hacker;

typedef struct {
	TCPsocket socket = NULL; // Socket identifier for agent
} Agent;

int next_ind = 0;	//index into sockets and clients arrays for next player connecting to server, increment after each client connects
TCPsocket server_socket;
Client clients[MAX_SOCKET];
SDLNet_SocketSet socket_set;
TCPsocket sockets[MAX_SOCKET];

void CloseSocket(int index){
	if(sockets[index] == NULL){
		fprintf(stderr, "ER: Attempted to delete a NULL socket.n");
        return;
	}

	if(SDLNet_TCP_DelSocket(socket_set, sockets[index]) == -1){
		fprintf(stderr, "ER: SDLNet_TCP_DelSocket: %sn", SDLNet_GetError());
        exit(-1);
	}

	memset(&clients[index], 0x00, sizeof(Client));
	SDLNet_TCP_Close(sockets[index]);
	sockets[index] = NULL;
}

int AcceptSocket(int index){
	if(sockets[index]) {
		fprintf(stderr, "ER: Overriding socket at index %d.\n", index);
		CloseSocket(index);
	}
	sockets[index] = SDLNet_TCP_Accept(server_socket);
	if(sockets[index] == NULL) return 0;
	
	clients[index].in_use = 1;
	if(SDLNet_TCP_AddSocket(socket_set, sockets[index]) == -1){
		fprintf(stderr, "ER: SDLNet_TCP_AddSocket: %s\n", SDLNet_GetError());
		exit(-1);
	}
	return 1;
}

uint8_t* RecvData(int index, uint16_t* length, uint16_t* flag){
	uint8_t temp_data[MAX_PACKET];
	// Get number of bytes received.
	int num_recv = SDLNet_TCP_Recv(sockets[index], temp_data, MAX_PACKET);

	
	if(num_recv <= 0) {	// If we received no data, either client disconnected or error.
		CloseSocket(index);
		const char* err = SDLNet_GetError();
		if(strlen(err) == 0) {
            printf("DB: client disconnected\n");
        } else {
            fprintf(stderr, "ER: SDLNet_TCP_Recv: %s\n", err);
        }
 
        return NULL;
    } else {	// Otherwise, parse the data received.
        int offset = 0;
        *flag = *(uint16_t*) &temp_data[offset];
        offset += sizeof(uint16_t);
 
        *length = (num_recv-offset);
 
        uint8_t* data = (uint8_t*) malloc((num_recv-offset)*sizeof(uint8_t));
        memcpy(data, &temp_data[offset], (num_recv-offset));
 
        return data;
	}
}

void SendData(int index, uint8_t* data, uint16_t length, uint16_t flag) {
	uint8_t temp_data[MAX_PACKET];

	int offset = 0;
	memcpy(temp_data+offset, &flag, sizeof(uint16_t));
	offset += sizeof(uint16_t);
	memcpy(temp_data+offset, data, length);
	offset += length;

	int num_sent = SDLNet_TCP_Send(sockets[index], temp_data, offset);
	printf("Sending data\n");
	if(num_sent < offset){
		fprintf(stderr, "ER: SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		CloseSocket(index);
	}
}

int main(int argc, char** argv){
	printf("Initializing...\n");
	if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_EVENTS) != 0){
		fprintf(stderr, "ER: SDL_Init: %sn", SDL_GetError());
		exit(-1);
	}

	if(SDLNet_Init() == -1){
		fprintf(stderr, "ER: SDLNet_Init: %sn", SDLNet_GetError());
		exit(-1);
	}

	IPaddress ip;
	printf("Resolving Host...\n");
	//using port 9999, feel free to change this if needed
	if(SDLNet_ResolveHost(&ip, NULL, 9999) == -1){
		fprintf(stderr, "ER: SDLNet_ResolveHost: %sn", SDLNet_GetError());
		exit(1);
	}

	printf("Opening Socket...\n");
	server_socket = SDLNet_TCP_Open(&ip);
	if(server_socket == NULL){
		fprintf(stderr, "ER: SDLNet_TCP_Open: %sn", SDLNet_GetError());
		exit(2);
	}

	printf("Allocating Sockets...\n");
	socket_set = SDLNet_AllocSocketSet(MAX_SOCKET+1);
	if(socket_set == NULL){
		fprintf(stderr, "ER: SDLNet_AllocSocketSet: %sn", SDLNet_GetError());
    	exit(-1);
	}

	printf("Adding Sockets...\n");
	if(SDLNet_TCP_AddSocket(socket_set, server_socket) == -1){
		fprintf(stderr, "ER: SDLNet_TCP_AddSocket: %sn", SDLNet_GetError());
    	exit(-1);
	}

	//main socket processing loop
	int running = 1;
	SDL_Event e;
	printf("Running Processing Loop...\n");
	while(running){
		// This little block allows us to catch SIGINT's from terminal.
		SDL_PollEvent(&e);
		if(e.type==SDL_QUIT){
			running = 0;
		}

		
		//printf("Checking Sockets...\n");
		int num_ready = SDLNet_CheckSockets(socket_set, 1000);	// block for 1000ms, seeing if a socket is ready to be processed.

		if(num_ready <= 0){
			//printf("No Sockets Ready...\n");
			//NONE of the sockets are ready to process, do server tasks
		} else{
			//printf("At Least One Socket Ready...\n");
			
			// Some of the sockets are ready to process, process these sockets
			// Is the server_socket ready for processing?
			if(SDLNet_SocketReady(server_socket)){
				printf("Attempting To Accept Socket...\n");
				int requesting_socket = AcceptSocket(next_ind);
				if(!requesting_socket){	// No Socket was accepted. Skip the rest of this code.
					printf("No Sockets Accepted...\n");
					num_ready--;
					continue;
				}
				
				// Find a new empty index
				int chk_count;
				printf("Finding New Empty Socket Index...\n");
				for(chk_count=0; chk_count<MAX_SOCKET;++chk_count){
					if(sockets[(next_ind+chk_count)%MAX_SOCKET] == NULL)
					{
						break;
					}
				}
				next_ind = (next_ind+chk_count)%MAX_SOCKET;
				printf("DB: new connection (next_ind = %d)\n", next_ind);
				num_ready--;	// One less connection ready to be processed
			}
			// Is a client_socket ready for processing?
			int ind;
			for(ind=0 ; (ind<MAX_SOCKET) && num_ready; ++ind){
				printf("Checking Client Socket...\n");
				// If socket doesn't exist or is not ready for processing
				if(sockets[ind]==NULL) continue;
				if(!SDLNet_SocketReady(sockets[ind])) continue;

				// A socket is ready to be processed. Process data received.
				printf("Ready for Processing!\n");
				uint8_t* data;
				uint16_t flag;
				uint16_t length;

				data = RecvData(ind, &length, &flag);
				//printf("Data Recv\n");
				if(data == NULL){	// Data received incorrectly
					num_ready--;
					continue;
				}
				//printf("Data Recv Success\n");
				switch(flag){
					case FLAG_HELLO: { // We will send hello string back to client
						uint16_t offset = 0;
						printf("Hacker has said hello!");
						// send_data is a buffer of MAX_PACKET bytes (8 bits in a byte)
						uint8_t send_data[MAX_PACKET] = "hello!\0\n";
						// Might need to include null terminator to offset.
						offset += strlen((char*)send_data)+1;

						SendData(ind, send_data, offset, FLAG_HELLO); 


						//printf("Sending Data...\n");
					} break;

					case FLAG_QUIT: { // Client will shut down server
						running = 0;
						printf("DB: shutdown by client id: %d\n", ind);
					} break;
				}

				free(data);
				num_ready--;
			}
		}
	
	}

	//cleanup
	if(SDLNet_TCP_DelSocket(socket_set, server_socket) == -1){
		fprintf(stderr, "ER: SDLNet_TCP_DelSocket: %sn", SDLNet_GetError());
    	exit(-1);
	} SDLNet_TCP_Close(server_socket);

	int i;
	for(i = 0; i < MAX_SOCKET; ++i){
		if(sockets[i] == NULL) continue;
		CloseSocket(i);
	}

	SDLNet_FreeSocketSet(socket_set);
	SDLNet_Quit();
	SDL_Quit();
}
