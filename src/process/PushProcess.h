#ifndef _PUSH_PROCESS_H
#define _PUSH_PROCESS_H

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************
 * Defines.....
 *************************************************************************************/

#define MQTT_PUSHER     "/usr/bin/mosquitto_pub"

typedef struct
{
    char            topic[MAX_BUFFER];
    char            message[MAX_BUFFER];
} MQTT_DATA;

/*************************************************************************************
 * Global 변수 정의
 *************************************************************************************/
 
char				program_name[64];
int 				process_id;


/*************************************************************************************
 * 함수 정의
 *************************************************************************************/

int   GetReadLineNum();
void  WriteReadLineNum(int);

void  InitServer();
void  interrupt(int);

#ifdef __cplusplus
}
#endif

#endif
