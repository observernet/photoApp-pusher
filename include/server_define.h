/*****************************************************
 * 프로그램ID	: server_define.h
 * 프로그램명	: 서버의 일반정의를 가지고 있는다.
 *****************************************************/

#ifndef _SERVER_DEFINE_H
#define _SERVER_DEFINE_H

/******************************************************************************
 * System Common....
 ******************************************************************************/

#define _REAL_SERVICE                   'R'                 /* Real Server */
#define _TEST_SERVICE                   'T'                 /* Dev  Server */
#define __SERVICE                       _REAL_SERVICE

/******************************************************************************
 * Program Infomation
 ******************************************************************************/

#define PROGRAM_NAME					"MQTTPusher"

#define CONF_PATH						"conf"
#define DATA_PATH						"data"
#define LOG_PATH						"log"
#define LIB_PATH						"lib"

#define SERVER_CONFIG_FILE				"server.conf"
#define PROCESS_LIST_FILE				"process.dat"

/******************************************************************************
 * Key Define...
 ******************************************************************************/

#define COMMON_SHM_KEY					0x6801

/******************************************************************************
 * Max Define...
 ******************************************************************************/

#define MAX_BUFFER						4096
#define MAX_PROCESS						16
#define MAX_SERVER_USER                 64

/******************************************************************************
 * Process Define...
 ******************************************************************************/

#define _PROC_RECEIVE_PROCESS_          1
#define _PROC_PUSH_PROCESS_V1_          2
#define _PROC_PUSH_PROCESS_V2_          3

/******************************************************************************
 * Variable Types Define....
 ******************************************************************************/
#include <type_define.h>

#endif
