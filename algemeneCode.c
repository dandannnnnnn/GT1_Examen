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
#define CLIENT ""
#define PAYLOAD "Hello from RPi 101!"
#define topic "MB_subscribe" //subscribing to MQTT to receive message
#define TIMEOUT 10000L

void messageHandler(void *context, char *topicName, int topicLEN, MQTTClient_message *message) {
    FILE *file;

    file = fopen("receivedMSGs.txt", "a");
    if (file != NULL) {
        fprintf(file, "Topic: %s\tMessage: %.*s\n", topicName, message -> payloadlen, (char *)message->payload);
        fclose(file;)
    } else {
        printf("Error: cannot open file");
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

}

int main() {
    MQTTClient CLIENT;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    
}