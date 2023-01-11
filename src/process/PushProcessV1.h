#ifndef _PUSH_PROCESS_V1_H
#define _PUSH_PROCESS_V1_H

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************
 * Defines.....
 *************************************************************************************/

#define MQTT_PUSHER     "/usr/bin/mosquitto_pub"
#define FCM_PUSHER      "/usr/local/nodejs/bin/node /home/www/MQTTPusher/node/fcm.js"

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

int   PushDataToMQTT(MQTT_DATA* mdata);
int   PushDataToFCM(MQTT_DATA* mdata);

int   GetReadLineNum();
void  WriteReadLineNum(int);

void  InitServer();
void  interrupt(int);

#ifdef __cplusplus
}
#endif

#endif
