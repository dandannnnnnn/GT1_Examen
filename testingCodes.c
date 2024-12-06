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
//Hier worden enkele functies apart getest voor de GT1 Datacommunicatie van Mikhaela Balaga 612ICW
//=============================================================================================================
//=============================================================================================================
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <MQTTClient.h>

#define ADDRESS     "tcp://192.168.0.101:1883"       // MQTT broker URL
#define CLIENTID    "RaspberryPiClient"             // Unique client ID
#define TOPIC       "P1/DM3_test"                    // MQTT topic
#define PAYLOAD     "Hello from Raspberry Pi!"      // Message to send
#define QOS         1                               // Quality of Service level
#define TIMEOUT     10000L                          // Timeout in milliseconds

#define topicSUB "P1/DM3" //Receiving messages from this topic
#define topicPUB "P1/DM3_out" //Publishing message onto RPi terminal

#define topicLEN 150
#define outputLEN 2048
#define DATE_TIME_LEN 120

#define appendFile 100

volatile MQTTClient_deliveryToken deliveredtoken;
volatile int counter = 0;
/*volatile float totVerbruik_DAG = 0;
volatile float totOpbrengst_DAG = ;
volatile float totVerbruik_NACHT =;
volatile float totOpbrengst_NACHT = ;
volatile float totVerbruik_GAS = ; */

struct meter_data {
    char dateTime[DATE_TIME_LEN]; //only 1 dateTime necessarry
    float totaal_dagverbruik;
    float totaal_nachtverbruik;
    float totaal_dagopbrengst;
    float totaal_nachtopbrengst;

    float totaal_gasverbruik;
};

void addTo_FILE(const char *message);
void dateTime(char *timestamp);

void delivered(void *context, MQTTClient_deliveryToken dt) {
    printf("Message with token value %d delivery confirmed\n", dt);
    printf("-----------------------------------------------\n");
    deliveredtoken = dt;
}

//dateTime ; totaal_dagverbruik ; totaal_nachtverbruik ; totaal_dagopbrengst ; totaal_nachtopbrengst ; totaal_gasverbruik

//Extracts msg and splits into fields
int arrivedMSG(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char *payload = message->payload;
    char *token_str;
    char timeBUFFER[20];
    float data;

    token_str = strtok(payload, ";");
    char *device = token_str;
    token_str = strtok(NULL, ";");
    char *code = token_str;
    token_str = strtok(NULL, ";");
    data = strtold(token_str, NULL);
    
    counter++;




}

void dateTime(char *timestamp) {
        time_t t ;
    struct tm *tmp ;
    
    time( &t );
    tmp = localtime( &t );
     
    //formatting dateTime string
    sprintf( timestamp, "%02d.%02d.%02d-%02d:%02d:%02d", 
        tmp->tm_year-100, tmp->tm_mon+1, tmp->tm_mday, // YEAR-MONTH_DAY
        tmp->tm_hour, tmp->tm_min, tmp->tm_sec ); // HOUR:MIN:SEC
     
    return( 0 );
}

void connlost(void *context, char *cause) {
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main() {
    // Open MQTT client for listening
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    // Define the correct call back functions when messages arrive
    MQTTClient_setCallbacks(client, client, connlost, arrivedMSG, delivered);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    printf("Subscribing to topic %s for client %s using QoS%d\n\n", topicSUB, CLIENTID, QOS);
    MQTTClient_subscribe(client, topicSUB, QOS);

    // Keep the program running to continue receiving and publishing messages
    for(;;) {
        ;
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}