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
int messageHandler(void *context, char *topicName, int topicLEN, MQTTClient_message *message) {
    printf("Message received on topic: %s\n", topicName);
    printf("Message payload: %.*s\n", message->payloadlen, (char *)message->payload);
    
    FILE *file;

    //opening file and adding the incoming MSG to the txt file: receivedMSGs.txt
    file = fopen("receivedMSGs.txt", "a");
    if (file != NULL) {
        printf("Opening file\n");
    } else {
        perror("Error: cannot open file\n");
    }

    fprintf(file, "%.*s\n"), message->payloadlen, (char *)message->payload;
    fclose(file);
}

int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    // Initialize the MQTT client
    MQTTClient_create(&client, IP_ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    // Set connection options
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    // Connect to the broker
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return -1;
    }

    // Set the callback function for message arrival
    MQTTClient_setCallbacks(client, NULL, NULL, messageHandler, NULL);

    // Subscribe to MB_subscribe
    if ((rc = MQTTClient_subscribe(client, topic, 0)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to subscribe, return code %d\n", rc);
        return -1;
    }

    printf("Listening for messages on topic: %s\n", topic);

    // Keep the program running to receive messages and append them in the txt file
    while (1) {
    }

    // Disconnect and clean up (not reachable in this example due to infinite loop)
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return 0;
}
