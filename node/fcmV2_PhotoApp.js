/**
 * Firebase Cloud Messaging (FCM) can be used to send messages to clients on iOS, Android and Web.
 *
 * This sample uses FCM to send two types of messages to clients that are subscribed to the `news`
 * topic. One type of message is a simple notification message (display message). The other is
 * a notification message (display notification) with platform specific customizations. For example,
 * a badge is added to messages that are sent to iOS devices.
 */
const logger = require('./config/winston');
const child_process = require('child_process');
const https = require('https');
const { google } = require('googleapis');
 
const PROJECT_ID = 'observer-photo';
const HOST = 'fcm.googleapis.com';
const PATH = '/v1/projects/' + PROJECT_ID + '/messages:send';
const MESSAGING_SCOPE = 'https://www.googleapis.com/auth/firebase.messaging';
const SCOPES = [MESSAGING_SCOPE];
 
/**
 * Get a valid access token.
 */
function getAccessToken()
{
    return new Promise(function(resolve, reject)
    {
        const key = require('./service-account.json');
        const jwtClient = new google.auth.JWT(
            key.client_email,
            null,
            key.private_key,
            SCOPES,
            null
        );
        jwtClient.authorize(function(err, tokens)
        {
            if (err)
            {
                reject(err);
                return;
            }
            resolve(tokens.access_token);
        });
    });
}
 
/**
 * Send HTTP request to FCM with given message.
 */
function sendFcmMessage(fcmMessage)
{
    getAccessToken().then(function(accessToken)
    {
        const options = {
            hostname: HOST,
            path: PATH,
            method: 'POST',
            headers: {
                'Authorization': 'Bearer ' + accessToken
            }
        };
 
        const request = https.request(options, function(resp)
        {
            resp.setEncoding('utf8');
            resp.on('data', function(data)
            {
                logger.info('Message sent to Firebase for delivery, response:');
                logger.info(data);
            });
        });
 
        request.on('error', function(err)
        {
            logger.info('Unable to send message to Firebase');
            logger.info(err);
        });
 
        request.write(JSON.stringify(fcmMessage));
        request.end();
    });
}
 
/**
 * Construct a JSON object that will be used to customize
 * the messages sent to iOS and Android devices.
 */
function buildOverrideMessage(topic, message, callback)
{
    buildCommonMessage(topic, message, (fcmMessage) =>
    {
        const apnsOverride = {
            payload: {
                'aps': {
                    'badge': 1,
                    'sound': 'default',
                    'content_available':true
                }
            },
            headers: {
                // "apns-push-type": "background",
                "apns-priority": "10",
            },
        };
    
        const androidOverride = {
            "priority": "high",
            // 'notification': {
            //     "android_channel_id" : topic,
        //         'click_action': 'android.intent.action.MAIN'
            // }
        };
    
        fcmMessage['message']['android'] = androidOverride;
        fcmMessage['message']['apns'] = apnsOverride;
        
        callback(fcmMessage);
    });
}
 
/**
 * Construct a JSON object that will be used to define the
 * common parts of a notification message that will be sent
 * to any app instance subscribed to the news topic.
 */
function buildCommonMessage(topic, message, callback)
{
    var topics = topic.split('-');

    if ( topics.length == 4 && (topics[1] == 'PhotoApp' || topics[1] == 'PhotoAppDev') && (topics[2] == 'inout' || topics[2] == 'like') )
    {
        child_process.exec('/usr/local/php8/bin/php /home/www/MQTTPusher/php/get_fcm_token.php ' + topics[3], (err, sout, serr) =>
        {
            var token = sout;

            callback({
                'message': {
                    'token': token,
                    'notification': {
                        'title': message.title,
                        'body': message.body
                    },
                    'data': {
                        'topic': topic,
                        'message': JSON.stringify(message),
                    },
                }
            });
        });
    }
    else
    {
        if ( topics[1] == 'PhotoAppDev' )
        {
            callback({
                'message': {
                    'topic': topic,
                    'notification': {
                        'title': message.title,
                        'body': message.body,
                    },
                    'data': {
                        'topic': topic,
                        'message': JSON.stringify(message),
                    },
                }
            });
        }
        else
        {
            if ( topics.length == 4 && topics[1] == 'PhotoApp' && topics[2] == 'team' && topics[3] == 'observer' )
            {
                callback({
                    'message': {
                        'topic': topic,
                        'notification': {
                            'title': message.title,
                            'body': message.body,
                        },
                        'data': {
                            'topic': topic,
                            'message': JSON.stringify(message),
                        },
                    }
                });
            }
            else
            {
                callback({
                    'message': {
                        'topic': topic,
                        'notification': {
                            'title': 'OBSERVER',
                            'body': message.title
                        },
                        'data': {
                            'topic': topic,
                            'message': JSON.stringify({
                                'title': 'OBSERVER',
                                'body': message.title
                            }),
                        },
                    }
                });
            }
        }
    }
}

if ( process.argv.length == 4 )
{
    var topic = process.argv[2];
    var message = JSON.parse(process.argv[3]);

    topic = topic.replace(/\//g, '-');

    try
    {
        buildOverrideMessage(topic, message, (fcmMessage) =>
        {
            sendFcmMessage(fcmMessage);
        });
    }
    catch (err)
    {
        logger.info(err);
    }
}
else
{
    logger.info('Argument Error!!');
}
