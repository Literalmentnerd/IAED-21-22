/*Bernardo Luis Almeida Augusto 102820

This program manages flights between airports:
Depending of the input it can many things:
if the a command is inserted with an airport it will add that airport to the systems list of airports;
if the l command is inserted it will print out the current system list of airports and their information;
while if its inserted alongside a lists of airportIDs it will print out all the airports that are in the systems list and in the inserted list;
by the order in the latter one;
if the v command is inserted it will print out the systems flights list;
while if it also has a flight in the input it will add it to the systems flights list if its not already in it;
if the p command is inserted alongside an airportID it will print every flight that departs from that airport;
if the c command is inserted alondside an aiportID it will do the same as pbut with every flight that arrives at that airport;
Finally when t is inserted with a date it will update the systems date to the one inserted*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 150
#define ID 4
#define PAIS 31
#define CID 51
#define MAX_FLIGHTS 30001

/*-------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------Structs--------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------*/

typedef struct{
    char id[ID]; /*airport ID*/
    char pais[PAIS]; /*aiport country*/
    char cidade[CID]; /*airport city*/
    int voos; /*number of flights*/
} Aeroporto;

typedef struct{
    char code[8]; /*flight code*/
    char par[ID]; /*flight departure*/
    char che[ID]; /*flight arrival*/
    char dat[12]; /*flight date*/
    char hour[6]; /*flight hour*/
    char duration[6]; /*flight duration*/
    int capacity; /*flight capacity*/
} Voos;

/*-------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------Global variables-------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------*/

/*Array with every airport*/
Aeroporto aeroportos[41];
/*Counter for the number of airports on the list*/
int counter = 0;
/*Array with every flight*/
Voos flights[MAX_FLIGHTS];
/*Counter for the number of flights*/
int vcounter = 0;

/*-------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------Functions---------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------*/

void loop();
void commanda(char input[]);
void commandl(char input[]);
void sort();
int split(char input[], char* idlist[]);
void commandv(char input[], int date[]);
int checkdate(Voos flight, int date[]);
/*void commandp(char input[]);*/
/*void commandc(char input[]);*/
void commandt(int date[],char input[]);

int main(){
    loop();
    return 0;
}

/*loop that makes the whole program*/
void loop(){
    /*This will keep looping until the user uses the command q to terminate the program*/
    int a=1;
    /*Starting date*/
    int date[8] = {0,1,0,1,2,0,2,2};
    /*User input is saved in this array*/
    char input[MAX];
    while(a==1){
        scanf(" %[^\n]",input);
        switch(input[0]){
            case 'q':
                a = 0;
                break;
            case 'a':
                commanda(input);
                break;
            case 'l':
                commandl(input);
                break;
            case 'v':
                commandv(input,date);
                break;
            case 'p':
                /*commandp(input);*/
                break;
            case 'c':
                /*commandc(input);*/
                break;
            case 't':
                commandt(date,input);
                break;
        }
    }
}

/*Executes command a*/
void commanda(char input[]){
    Aeroporto airport;
    int i,n;
    /*Dividing the input into command ID country city and saving ID in x country in y and city in z*/
    char *x = strtok(input," \t"),*y,*z;
    x = strtok(NULL," \t");
    y = strtok(NULL," \t");
    z = strtok(NULL,"");
    /*Checking if the ID is valid*/
    n = strlen(x);
    if(n!=3){
        printf("invalid airport ID\n");
        return;
    }
    else{
        for(i=0; i<n;i++){
            if(islower(x[i])){
                printf("invalid airport ID\n");
                return;
            }
        }
    }
    /*Checking if there arent more than 40 airports in the aeroporto array*/
    if(counter == 40){
        printf("too many airports\n");
        return;
    }
    /*Checking if user isnt inserting a duplicate airport*/
    for(i=0; i<counter; i++){
        if(strcmp(aeroportos[i].id,x)==0){
            printf("duplicate airport\n");
            return;
        }
    }
    /*Giving esch struct parameter its respective info*/
    strcpy(airport.id,x);
    strcpy(airport.pais,y);
    strcpy(airport.cidade,z);
    airport.voos = 0;
    /*inserting the airport in aeroportos array */
    aeroportos[counter++] = airport;
    /*print airport ID*/
    printf("airport %s\n",airport.id); 
}

/*Executes command l*/
void commandl(char input[]){
    char* idlist[41];
    int size;
    int i,h,l,good;
    /*If the input was only l*/
    if(strlen(input)==1){
        /*Sort the struct array by the airports ID*/
        sort();
        /*Prints every airport on the aeroportos array*/
        for(i=0; i<counter;i++){
            printf("%s %s %s %d\n",aeroportos[i].id,aeroportos[i].cidade,aeroportos[i].pais,aeroportos[i].voos);
        }
    }
    /*If input was l plus a list of airport IDs*/
    else{
        size = split(input,idlist);
        for(h=0;h<size;h++){
            good = 0;
            for(l=0;l<counter;l++){
                /*If the ID is in the aeroportos array prints it and then good = 1*/
                if(strcmp(aeroportos[l].id,idlist[h])==0){
                    printf("%s %s %s %d\n",aeroportos[l].id,aeroportos[l].cidade,aeroportos[l].pais,aeroportos[l].voos);
                    good = 1;
                    break;
                }
            }
            /*If good is 0 then it means the id in idlist[h] isnt in the aeroportos array*/
            if(good == 0){
                printf("%s: no such airport ID\n", idlist[h]);
            }
        }
    }
}

/*Function that sorts aeroportos by the alphabethical oreder*/
void sort(){
    Aeroporto switcher[41];
    int i,j;
    for(i=0; i<counter-1; i++){
        for(j=0; j<counter-1-i; j++){
            /*Switches the order of the airports in the aeroportos array by the alphabetical order*/
            if(strcmp(aeroportos[j].id,aeroportos[j+1].id)>0){
                switcher[j] = aeroportos[j];
                aeroportos[j] = aeroportos[j+1];
                aeroportos[j+1] = switcher[j];
            }
        }
    }
}

/*Splits the input and returns the size of the idlist*/
int split(char input[], char* idlist[]){
    int i = 0;
    /*The line bellow removes the l in the input so we can get all the IDs and the number of IDs*/
    char* cmd = strtok(input," \t\n");
    cmd = strtok(NULL, " \t\n");
    /*While loop that saves the cmd that holds the id in the idlist*/
    while (cmd != NULL) {
        idlist[i] = malloc(sizeof(cmd)+1);
        strcpy(idlist[i++], cmd);
        cmd = strtok(NULL, " \t\n");
    } 
    return i;
}

/*Executes command v*/
void commandv(char input[], int date[]){
    Voos flight;
    /*Array that will hold everything from the input except the v command*/
    char* list[8];
    int i,h,k,codel,p;
    /*If input is only v, print every flight in the flights struct array*/
    if(strlen(input)==1){
        for(i=0; i<vcounter;i++){
            printf("%s %s %s %s %s\n",flights[i].code,flights[i].par,flights[i].che,flights[i].dat,flights[i].hour);
        }
    }
    else{
        /*Splitting the input to then give it to the respective part of the struct Voos*/
        split(input,list);
        strcpy(flight.code,list[0]);
        strcpy(flight.par,list[1]);
        strcpy(flight.che,list[2]);
        strcpy(flight.dat,list[3]);
        strcpy(flight.hour,list[4]);
        strcpy(flight.duration,list[5]);
        flight.capacity = atoi(list[6]);
        /*Invalid code error confirmation*/
        codel = strlen(flight.code);
        for(h=0;h<codel;h++){
            if(h<2){
                if(islower(flight.code[h])){
                    printf("invalid flight code\n");
                    return;
                }
            }
            else{
                if(!isdigit(flight.code[h])){
                    printf("invalid flight code\n");
                    return;
                }
            }
        }
        /*Checking if the flight isnt a duplicate*/
        for(k=0;k<vcounter;k++){
            if(strcmp(flights[k].code,flight.code)==0 && strcmp(flights[k].dat,flight.dat)==0){
                printf("flight already exists\n");
                return;
            }
        }
        /*Too many flights*/
        if(vcounter==30000){
            printf("too many flights");
            return;
        }
        /*Checking if the date is in the past or more than one year in the future*/
        p = checkdate(flight,date);
        if(p==0){
            printf("invalid date\n");
            return;
        }
        /*If the duration of the flight is invalid > 12h*/
        if(flight.duration[0]>='1' && flight.duration[1]>='2' &&
        flight.duration[3]>='0' && flight.duration[4]>'0'){
            printf("invalid duration\n");
            return;
        }
        /*If the flight capacity is invalid*/
        if(flight.capacity<10 || flight.capacity>100){
            printf("invalid capacity\n");
            return;
        }
        /*Adding the flight to the list of flights*/
        flights[vcounter++] = flight;
    }
}

int checkdate(Voos flight,int date[]){
    /*If statement to print invalid case if the date inserted was from  the past, from after 2023 or if the date inserted is more than 1 year ahead of the current one*/
    if((flight.dat[9]-48 < date[7]) || (flight.dat[9]-48 > 3) || 
        (flight.dat[9]-48 == date[7] && flight.dat[3]-48 < date[2]) || 
        (flight.dat[9]-48 == date[7] && flight.dat[3]-48 == date[2] && flight.dat[4]-48 < date[3]) || 
        (flight.dat[9]-48 == date[7] && flight.dat[3]-48 == date[2] && flight.dat[4]-48 == date[3] && flight.dat[0]-48 < date[0]) || 
        (flight.dat[9]-48 == date[7] && flight.dat[3]-48 == date[2] && flight.dat[4]-48 == date[3] && flight.dat[0]-48 == date[0] && flight.dat[1]-48 < date[1]) || 
        (flight.dat[9]-48 > date[7] && flight.dat[4]-48 > date[3]) || (flight.dat[9]-48 > date[7] && flight.dat[4]-48 == date[3] && flight.dat[3]-48 > date[2]) || 
        (flight.dat[9]-48 > date[7] && flight.dat[4]-48 == date[3] && flight.dat[3]-48 == date[2] && flight.dat[1]-48 > date[1]) || 
        (flight.dat[9]-48 > date[7] && flight.dat[4]-48 == date[3] && flight.dat[3]-48 == date[2] && flight.dat[1]-48 == date[1] && flight.dat[0]-48 > date[0])){
        return 0;
    }
    return 1;
}

/*Executes command p*/
/*void commandp(char input[]){
}*/

/*Execute command t*/
void commandt(int date[],char input[]){
    /*If statement to print invalid case if the date inserted was from  the past, from after 2023 or if the date inserted is more than 1 year ahead of the current one*/
    if((input[11]-48 < date[7]) || (input[11]-48 > 3) || 
        (input[11]-48 == date[7] && input[5]-48 < date[2]) || 
        (input[11]-48 == date[7] && input[5]-48 == date[2] && input[6]-48 < date[3]) || 
        (input[11]-48 == date[7] && input[5]-48 == date[2] && input[6]-48 == date[3] && input[2]-48 < date[0]) || 
        (input[11]-48 == date[7] && input[5]-48 == date[2] && input[6]-48 == date[3] && input[2]-48 == date[0] && input[3]-48 < date[1]) || 
        (input[11]-48 > date[7] && input[6]-48 > date[3]) || (input[11]-48 > date[7] && input[6]-48 == date[3] && input[5]-48 > date[2]) || 
        (input[11]-48 > date[7] && input[6]-48 == date[3] && input[5]-48 == date[2] && input[3]-48 > date[1]) || 
        (input[11]-48 > date[7] && input[6]-48 == date[3] && input[5]-48 == date[2] && input[3]-48 == date[1] && input[2]-48 > date[0])){
        printf("invalid date\n");
        return;
    }
    /*prints the updated date if the input is valid*/
    else{
        date[0]=input[2]-48;
        date[1]=input[3]-48;
        date[2]=input[5]-48;
        date[3]=input[6]-48;
        date[4]=input[8]-48;
        date[5]=input[9]-48;
        date[6]=input[10]-48;
        date[7]=input[11]-48;
        printf("%d%d-%d%d-%d%d%d%d\n",date[0],date[1],date[2],date[3],date[4],date[5],date[6],date[7]);
    }
}