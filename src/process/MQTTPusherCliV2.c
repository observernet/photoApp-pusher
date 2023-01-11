/*****************************************************
 * 프로그램ID	: MQTTPusherCli.c
 * 프로그램명	: 파라미터를 받아 MQTTPusher에 전달한다
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>

#include <network.h>
#include <profile.h>
#include <ft_common.h>

#include <MQTTPusher_interface.h>

/*************************************************************************************
 * Global 변수 정의
 *************************************************************************************/
 
char				program_name[64];

/*************************************************************************************
 * 구현 시작..
 *************************************************************************************/

SOCKET ConnectServer()
{
    SOCKET sockfd;
    int server_port;

    char conf_file_name[256];
		
	/* 설정파일을 연다. */
	sprintf(conf_file_name, "%s/%s/%s", mdb->program_home, CONF_PATH, SERVER_CONFIG_FILE);
	if ( !OpenProfile(conf_file_name) )
	{
		printf("설정파일을 여는 도중 에러가 발생하였습니다. file[%s] errno[%d]\n", conf_file_name, errno);
		return (-1);
	}
	
	/* 설정파일의 값을 가져온다. */
	server_port = GetProfileInt("RECEIVE_PROCESS", "SERVER_PORT", 0);
	if ( server_port == 0 )
	{
		printf("서버 접속 정보가 없습니다.\n");
		return (-1);
	}
	
	/* 설정파일을 닫는다. */
	CloseProfile();

    // 서버연결
    if ( (sockfd = GetConnectSocket("127.0.0.1", server_port)) == -1 )
    {
        printf("서버 접속에 실패하였습니다!! host[%s:%d] errno[%d]\n", "127.0.0.1", server_port, errno);
        return (-1);
    }

    return (sockfd);
}

int main(int argc, char** argv)
{
    SOCKET sockfd;
    char sndbuf[MAX_BUFFER];

    SetProgramName(argv[0]);

    if ( argc != 3 )
    {
        printf("useage: %s 'topic' 'message'\n", program_name);
        exit(EXIT_FAILURE);
    }

    /* 공유메모리를 초기화 한다. */
	InitCommonMemory();

    /* 서버에 연결한다 */
    if ( (sockfd = ConnectServer()) == -1 )
        exit(EXIT_FAILURE);
    
    /* 보낼데이타를 세팅한다 */
    sprintf(sndbuf, "%0*ld%s%c%s%c%s", MPI_LENGTH_SIZE, strlen("v2") + strlen(argv[1]) + strlen(argv[2]) + 2, "v2", MPI_SEPERATOR, argv[1], MPI_SEPERATOR, argv[2]);

    /* 데이타를 전송한다 */
    if ( SendTCP(sockfd, sndbuf, strlen(sndbuf), 0) == -1 )
    {
        printf("데이타 전송에 실패하였습니다. errno[%d]\n", errno);
        exit(EXIT_FAILURE);
    }
    printf("요청 데이타 전송 [%ld:%s]\n", strlen(sndbuf), sndbuf);
    sleep(1);

    CloseSocket(sockfd);
    exit(EXIT_SUCCESS);
}
