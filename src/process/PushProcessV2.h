#ifndef _PUSH_PROCESS_V2_H
#define _PUSH_PROCESS_V2_H

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************
 * Defines.....
 *************************************************************************************/

#define MQTT_PUSHER     "/usr/bin/mosquitto_pub"
#define NODE            "/usr/local/nodejs/bin/node"
#define FCM_PREFIX      "/home/www/MQTTPusher/node/fcmV2"

typedef struct
{
    char            topic[MAX_BUFFER];
    char            message[MAX_BUFFER];
    char            fcm_message[MAX_BUFFER];

    char            topic_appname[64];
    char            topic_type[64];
    char            topic_type2[64];
    char            topic_lang[8];
    char            topic_userkey[32];

    char            message_title[512];
    char            message_body[MAX_BUFFER];
    char            message_link[256];

    char            fcm_token[256];
    char            user_lang[8];
} MQTT_DATA;

/*************************************************************************************
 * Global 변수 정의
 *************************************************************************************/
 
char				program_name[64];
int 				process_id;

sql_context			ctx;

/*************************************************************************************
 * 함수 정의
 *************************************************************************************/

int   MakeFCMMessage(MQTT_DATA* mdata);
int   PushDataToFCM(MQTT_DATA* mdata);

int   ParseTopic(MQTT_DATA* mdata);
int   ParseMessage(MQTT_DATA* mdata);
int   GetTokenAndUserLang(MQTT_DATA* mdata);

int   GetReadLineNum();
void  WriteReadLineNum(int);

void  InitServer();
void  interrupt(int);

#ifdef __cplusplus
}
#endif

#endif
