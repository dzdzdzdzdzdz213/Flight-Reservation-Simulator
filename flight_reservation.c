#include<stdio.h>
#include<stdlib.h>
#include<string.h>
 
#define MAX_NAME_LENGHT 50

typedef struct Passenger {
    char name[MAX_NAME_LENGHT];
    struct Passenger *next;
} Passenger;

typedef struct Flight {
   int FlightNumber;
   int capacity;
   int bookedseats;
   Passenger *reservations;
   Passenger *waitlist;
   struct Flight *next;
} Flight;
