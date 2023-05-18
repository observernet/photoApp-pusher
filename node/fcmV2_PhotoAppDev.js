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
function buildOverrideMessage(topic, message, token)
{
    const fcmMessage = buildCommonMessage(topic, message, token);

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
    
    return fcmMessage;
}
 
/**
 * Construct a JSON object that will be used to define the
 * common parts of a notification message that will be sent
 * to any app instance subscribed to the news topic.
 */
function buildCommonMessage(topic, message, token)
{
    //var topics = topic.split('-');

    if ( token.length > 10 )
    {
        return {
            'message': {
                'token': token,
                'notification': {
                    'title': message.notification.title,
                    'body': message.notification.body
                },
                'data': {
                    'topic': topic,
                    'message': JSON.stringify(message.message),
                },
            }
        };
    }
    else
    {
        return {
            'message': {
                'topic': topic,
                'notification': {
                    'title': message.notification.title,
                    'body': message.notification.body,
                },
                'data': {
                    'topic': topic,
                    'message': JSON.stringify(message.message),
                },
            }
        };
    }
}

if ( process.argv.length == 5 )
{
    var topic = process.argv[2];
    var message = JSON.parse(process.argv[3]);
    var token = process.argv[4];

    topic = topic.replace(/\//g, '-');

    try
    {
        const fcmMessage = buildOverrideMessage(topic, message, token);
        logger.info(JSON.stringify(fcmMessage, null, 2));
        sendFcmMessage(fcmMessage);
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
