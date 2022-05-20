#ifndef _RECEIVE_PROCESS_H
#define _RECEIVE_PROCESS_H

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************
 * Defines.....
 *************************************************************************************/

typedef struct
{
    SOCKET          sockfd;
    char            user_ip[16];
} CLIENT;

/*************************************************************************************
 * Global 변수 정의
 *************************************************************************************/
 
char				program_name[64];
int 				process_id;

int					epoll_fd;
struct epoll_event	events[MAX_SERVER_USER];
struct timeval		timeover;

int				    server_port;
SOCKET			    server_sockfd;
CLIENT              client[MAX_SERVER_USER];

/*************************************************************************************
 * 함수 정의
 *************************************************************************************/

int   add_epoll(SOCKET fd);
int   del_epoll(SOCKET fd);

int   ReceiveRequest(SOCKET sockfd);


int   AcceptUser(SOCKET sockfd);
int   RemoveUser(SOCKET sockfd);
int   GetUserOffset(SOCKET sockfd);

void  InitServer();
void  interrupt(int);

#ifdef __cplusplus
}
#endif

#endif
