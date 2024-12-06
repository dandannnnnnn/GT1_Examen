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
#include <stdlib.h>
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

#define appendFILE 100 //after 100 messages it will be added to the file

volatile MQTTClient_deliveryToken deliveredtoken;
volatile float totaal_dagverbruik, totaal_nachtverbruik, totaal_dagopbrengst, totaal_nachtopbrengst;
volatile float totaal_gasverbruik;
volatile float totale_stroomverbruik, totale_stroomopbrengst, totale_gasverbruik;

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
float calculations(float *startCalculations);
float startCalculations();

void delivered(void *context, MQTTClient_deliveryToken dt) {
    printf("Message with token value %d delivery confirmed\n", dt);
    printf("-----------------------------------------------\n");
    deliveredtoken = dt;
}

//dateTime ; totaal_dagverbruik ; totaal_nachtverbruik ; totaal_dagopbrengst ; totaal_nachtopbrengst ; totaal_gasverbruik

//Extracts msg and split into fields
int arrivedMSG(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char *payload = message->payload;
    char *token_str;
    char timeBUFFER[20];

    //start: dateTime
    token_str = strtok(payload, ";");
    char *totaal_dagverbruik = token_str;
    token_str = strtok(NULL, ";");
    char *totaal_nachtverbruik = token_str;
    token_str = strtok(NULL, ";");
    char *totaal_dagopbrengst = token_str;
    token_str = strtok(NULL, ";");
    char *totaal_nachtopbrengst = token_str;
    token_str = strtok(NULL, ";");
    char *totaal_gasverbruik = token_str;
    token_str = strtok(token_str, NULL); //end splitting in fields
    
        // Publish to outgoingMSG (topicPUB)
    MQTTClient client = (MQTTClient)context;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    pubmsg.payload = outputLEN;
    pubmsg.payloadlen = strlen(outputLEN);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    MQTTClient_publishMessage(client, topicPUB, &pubmsg, &token);
    printf("Publishing to topic %s\n",topicPUB);
}
float startCalculations() {
    float start_dagverbruik = 6340.33594;
    float start_dagopbrengst = 298.30499;
    float start_nachtverbruik = 6664.99414;
    float start_nachtopbrengst = 146.75200;

    float start_gasverbruik = 6184.92480;
} 
//Calculating the usage of electricity
float calculations(float *startCalculations) {
    float totale_stroomverbruik = totaal_dagverbruik + totaal_nachtverbruik;
    float totale_stroomopbrengst = totaal_dagopbrengst + totaal_nachtopbrengst;

    float totale_gasverbruik = totale_gasverbruik * 11.55;
}

/*void calculationsDay() { //Calculations per day
}*/
void dateTime(char *timestamp) {
        time_t t ;
    struct tm *tmp ;
    
    time( &t );
    tmp = localtime( &t );
     
    //formatting dateTime string
    sprintf( timestamp, "%02d.%02d.%02d-%02d:%02d:%02d", 
        tmp->tm_year-100, tmp->tm_mon+1, tmp->tm_mday, // YEAR-MONTH_DAY
        tmp->tm_hour, tmp->tm_min, tmp->tm_sec ); // HOUR:MIN:SEC
     
}

void addTo_FILE(const char *message) {
    FILE *file = fopen("receivedMSGs.txt", "a");
    char line[2048];
    if (file == NULL) {
        perror("Error: cannot open file");
        return;
    }
    fprintf(file, "%s\n", message);
    fclose(file);
    
    for(int i = 0; i<=5 ;i++) {
    printf(file,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf(file,"Elektriciteit- en gas verbruik - totalen per dag\n");
    printf(file,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
    printf(file,"STARTWAARDEN\n\n");
    printf(file,"DATE - TIME: %s\n",dateTime);
    printf(file,"DAG\tTotaal verbruik\t= %f kWh\n",totaal_dagverbruik);
    printf(file,"DAG\tTotaal opbrengst\t= %f kWh\n",totaal_dagopbrengst);
    printf(file,"NACHT\tTotaal verbruik\t= %f kWh\n", totaal_nachtverbruik);
    printf(file,"NACHT\tTotaal opbrengst\t= %f kWh\n", totaal_nachtopbrengst);
    printf(file,"GAS\tTotaal verbruik\t= %f m³\n", totaal_gasverbruik);
    printf(file,"---------------------------------------------------------------\n");
    printf(file,"TOTALEN:\n");
    printf(file,"---------------------------------------------------------------\n\n");

    while(fgets(line, sizeof(line), file != NULL)) {
        printf("Datum: %s\n", dateTime);
        printf("---------------------\n");
        printf("STROOM:\n");
        printf("\tTotaal verbruik\t=\t%f kWh\n", totale_stroomverbruik);
        printf("\tTotaal opbrengst\t=\t%f kWh\n", totale_stroomopbrengst);
        printf("GAS:\n");
        printf("\tTotaal verbruik\t=\t%f kWh\n", totale_gasverbruik);
        printf("*");
    }
    }
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

    MQTTClient_setCallbacks(client, client, connlost, arrivedMSG, delivered);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(1);
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