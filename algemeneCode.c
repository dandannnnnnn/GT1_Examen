//============================================================================================================
//============================================================================================================
//Leerling: Mikhaela Balaga
//Klas: 612ICW
//Klasnummer: 3
//Datum: 06/12/2024
//============================================================================================================
//============================================================================================================

//=============================================================================================================
//=============================================================================================================
//Dit is de algemene code voor de GT1 Datacommunicatie van Mikhaela Balaga 612ICW
//=============================================================================================================
//=============================================================================================================

#include <stdio.h>
#include <string.h>
#include <MQTTClient.h> //library for MQTT functions

#define IP_ADDRESS "tcp://192.168.0.5:1883" //IP-ADDRESS:port
#define CLIENTID " "
#define PAYLOAD "Hello from RPi 101!"
#define topic "MB_subscribe" //subscribing to MQTT to receive message
#define TIMEOUT 10000L

//When message arrived, added to txt file
void messageHandler(void *context, char *topicName, int topicLEN, MQTTClient_message *message) {
    FILE *file;

    //opening file and adding the incoming MSG to the txt file: receivedMSGs.txt
    file = fopen("receivedMSGs.txt", "a");
    if (file != NULL) {
        fprintf(file, "Topic: %s\tMessage: %.*s\n", topicName, message -> payloadlen, (char *)message->payload);
        fclose(file);
    } else {
        printf("Error: cannot open file");
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

}

int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    //init MQTT CLIENT
    MQTTClient_create(&client, IP_ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    //Connection options
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    //Connecting to MB_subscribe topic/MQTT
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return -1;
    }

    MQTTClient_setCallbacks(client, NULL, NULL, messageHandler, NULL);

    //Subscribing to MB_subscribe
    if ((rc = MQTTClient_subscribe(client, topic, 0)) != MQTTCLIENT_SUCCESS) {
        printf("Cannot subscribe. Check if topic name is correct! Return code: %d\n", rc);
        return -1;
    }

    //Program keeps running to receive and publish messages
    for(;;) {
        ;
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return 0;
}