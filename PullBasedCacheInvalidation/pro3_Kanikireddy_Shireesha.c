/* Project #3; Kanikireddy, Shireesha */
/* Pull Based Cache Invalidation
*/
//#include <stdlib.h>
#include <csim.h>
#include <stdio.h>


#define SIMULATION_TIME 100
#define NUMBER_OF_CLIENTS 2
#define DATABASE_SIZE 500
#define CACHE_SIZE 100
#define NUM_SERVER 1

#define BROADCAST_INTERVAL 20
#define MEAN_UPDATE_ITEM 10
#define MEAN_QUERY_GENERATION_TIME 10
#define HOT_DATA_ITEM_MAXIMUM 49
#define COLD_DATA_ITEM_MAXIMUM 450

typedef struct nde {
    MBOX mbox;
}node_t;

node_t node[NUMBER_OF_CLIENTS + NUM_SERVER];

typedef struct data_items {
    long dataID;
    TIME last_updated_time;

}dataItems_t;
dataItems_t  database[DATABASE_SIZE];
dataItems_t  iR;

typedef struct cache_items {
    long dataID;
    TIME last_updated_time;
    long accessed_item;
}cache_t;
cache_t cache_size[CACHE_SIZE];


void init();
void server();
void invalidationReport();
void updateDataItems();
void receiveMessage();

void client();
void generateQuery();
void receiveIRMessages();


void sim() {
    create("Simulation_Start");
    init();
    hold(SIMULATION_TIME);
}

void init() {
    long i=0;
    char str[24];
    
    long totalNodes = NUMBER_OF_CLIENTS + NUMBER_OF_CLIENTS;

    while(i<totalNodes){
        sprintf(str, "input.%d", i);
        node[i].mbox = mailbox(str);
        i++;
    }

    server(0);
    printf("Server Method is being called - server():\n");

    for (i = 0; i < NUMBER_OF_CLIENTS; i++) {
        client(i);
        printf("Client Method is being called - client(): %ld\n", i);
    }    
}

void server(long n) {
    create("server");
    printf("Server is Created/Generated\n");

    long i;
    for (i = 0; i < DATABASE_SIZE; i++) {
        database[i].dataID = i;
        database[i].last_updated_time = clock;
    }
    printf("Calling Invalidation Report Method - invalidation_report(): \n");
    invalidationReport();
    updateDataItems();
    // receiveMessage();
    while(clock < SIMULATION_TIME){
        hold(1);
    }   
}

void invalidationReport() {
    create("InvalidationReport"); 
    printf("InvalidationReport is generated\n");
    while(clock < SIMULATION_TIME){
        hold(5.0);    
        long i=0;
        iR = (struct data_item *)malloc(sizeof(struct data_items) * DATABASE_SIZE);
        printf("Creating an InvalidationReport...\n");
        while(i<DATABASE_SIZE){
            iR[i].dataID = i;
            iR[i].last_updated_time = clock;
            i++;
        }
        while( i < NUMBER_OF_CLIENTS) {   
            send(node[i].mbox, iR);
            printf("Broadcasting the message to node %ld \n", i);
            i++;
        }
    } 
}

void updateDataItems() {
    create("updateDataItems");
    printf("Updating data items\n");
    while(clock < SIMULATION_TIME){
        hold(10);
        long i;
        for (i = 0; i < DATABASE_SIZE; i++) {
            database[i].last_updated_time = clock;
            printf("Updating database at index %ld with time %6.3f\n", i, database[i].last_updated_time);
        }  
    }   
}

void receiveMessage() {
    create("receiveMessage");
    while(clock < SIMULATION_TIME){
        hold(5.0);

    }
}


void client(long n) {
    create("client");
    printf("Client %ld is generated\n", n);

    long i;
    for (i = 0; i < CACHE_SIZE; i++) {
        cache_size[i].dataID = i;
        cache_size[i].last_updated_time = i;
        cache_size[i].accessed_item = i;
    }

    receiveIRMessages(n);
    while(clock < SIMULATION_TIME){
        hold(1);  
    }
    
}

void generateQuery() {
    create("query");
    while(clock < SIMULATION_TIME) {
        hold(MEAN_QUERY_GENERATION_TIME);

    }  
}

void receiveIRMessages(long n, long* msg) {
    create("receiveIRMessages");
    printf("receiving messages\n");
    while(clock < SIMULATION_TIME){
        hold(20);
        receive(node[n].mbox, iR);
        long i;
        for(i = 0; i < 10; i++) {
            printf("Node %ld received message %6.3f\n", n, iR[i].last_updated_time);
        }
        
        for (i = 0; i < 5; i++) {
            printf("cache of node %ld at index %ld\n", n, cache_size[i].dataID);
        }   
    }
    
}

