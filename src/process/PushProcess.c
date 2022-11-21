/*****************************************************
 * 프로그램ID	: PushProcess.c
 * 프로그램명	: 큐를 읽어서 메세지는 푸쉬한다
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include <ft_common.h>

#include <MQTTPusher_interface.h>
#include "PushProcess.h"

/*************************************************************************************
 * 구현 시작..
 *************************************************************************************/
 
int main(int argc, char** argv)
{
	FILE* fp;
    char filename[256], readbuf[MAX_BUFFER];
    int len, linenum, finish_line;

	int ptr;
	MQTT_DATA mdata;

	SetProgramName(argv[0]);
	
	/* 서버를 초기화 한다. */
	InitServer();

	/* 이전에 처리한 라인넘버를 가져온다 */
    finish_line = GetReadLineNum();
	
    /* 파일을 연다 */
    sprintf(filename, "%s/%s/%08d.que", mdb->program_home, DATA_PATH, mdb->system_date);
	while ( 1 )
	{
		if ( (fp = fopen(filename, "r")) )
			break;
		
		if ( errno != ENOENT )
		{
			Log("main: 파일여는 도중 에러가 발생하였습니다 filename[%s] errno[%d]\n", filename, errno);
			interrupt(0);
		}
		sleep(10);
	}

    linenum = 0;
	while ( 1 )
	{
		memset(readbuf, 0x00, MAX_BUFFER);
        if ( fgets(readbuf, MAX_BUFFER, fp) )
        {
            linenum++;
            if ( linenum <= finish_line) continue;

            len = strlen(readbuf);
            if ( readbuf[len-1] == '\n' ) readbuf[len-1] = 0;

            /* 읽어온 데이타를 파싱한다 */
			memset(&mdata, 0x00, sizeof(MQTT_DATA));
			ptr = get_next_token(readbuf, 0, mdata.topic, MPI_SEPERATOR);
			ptr = get_next_token(readbuf, ptr, mdata.message, MPI_SEPERATOR);

			/* MQTT에 Push한다 (MQTT는 FCM으로 대체한다) */
			//PushDataToMQTT(&mdata);

			/* FCM에 Push한다 */
			PushDataToFCM(&mdata);

			/* 라인넘버를 기록한다 */
            WriteReadLineNum(linenum);
        }
        else
            usleep(100000);
	}
    fclose(fp);
	
	interrupt(0);
	
	exit(EXIT_SUCCESS);
}

/**
 * MQTT에 데이타를 푸쉬한다
 */
int PushDataToMQTT(MQTT_DATA* mdata)
{
    char buffer[MAX_BUFFER*2];
	
	sprintf(buffer, "%s -h %s -p %d -u %s -P %s -t '%s' -m '%s'", MQTT_PUSHER, mdb->config.mqtt_host, mdb->config.mqtt_port, mdb->config.mqtt_user, mdb->config.mqtt_passwd, mdata->topic, mdata->message);
	system(buffer);

	Log("PushDataToMQTT: %s\n", buffer);

    return (0);
}

/**
 * FCM에 데이타를 푸쉬한다
 */
int PushDataToFCM(MQTT_DATA* mdata)
{
    char buffer[MAX_BUFFER*2];
	
	sprintf(buffer, "%s '%s' '%s'", FCM_PUSHER, mdata->topic, mdata->message);
	system(buffer);

	Log("PushDataToFCM: %s\n", buffer);

    return (0);
}

/**
 * 라인번호를 가져온다
 */
int GetReadLineNum()
{
	FILE* fp;
	char filename[256], buff[64];
	int linenum = 0;

	/* 라인파일을 연다 */
	sprintf(filename, "%s/%s/%s.%08d.line", mdb->program_home, DATA_PATH, program_name, mdb->system_date);
	if ( (fp = fopen(filename, "r")) == NULL ) return (0);

	/* 라인번호를 가져온다 */
	memset(buff, 0x00, 64);
	if ( fgets(buff, 64, fp) != NULL )
	{
		linenum = atoi(buff);
	}
	fclose(fp);

	return (linenum);
}

/**
 * 라인번호를 기록한다
 */
void WriteReadLineNum(int linenum)
{
	FILE* fp;
	char filename[256];

	/* 라인파일을 열고 기록한다 */
	sprintf(filename, "%s/%s/%s.%08d.line", mdb->program_home, DATA_PATH, program_name, mdb->system_date);
	fp = fopen(filename, "w");
	fprintf(fp, "%d", linenum);
	fclose(fp);
}

/*************************************************************************************
 * 초기화함수
 *************************************************************************************/

/**
 * 서버를 초기화한다.
 */ 
void InitServer()
{
	/* 공유메모리를 초기화 한다. */
	InitCommonMemory();
	

    /* Process를 등록한다. */
	if ( (process_id = RegistProcess(_PROC_PUSH_PROCESS_)) == -1 )
	{
		Log("InitServer: 프로세스 실행에 실패하였습니다.\n");
		exit(EXIT_FAILURE);
	}
	
	/* 시스널 핸들러 설정 */
	signal(SIGHUP, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, interrupt);
    signal(SIGTERM, interrupt);
    signal(SIGQUIT, interrupt);
    
	Log("InitServer: 서버 초기화! Process Start [%d]..................................\n", process_id);
}

/**
 * 서버를 종료한다.
 */
void interrupt(int sig)
{
	/* 프로세스 등록 해제 */
	RemoveProcess(_PROC_PUSH_PROCESS_);
	DeAttachShm((void*)mdb);
	
    exit(EXIT_SUCCESS);
}
