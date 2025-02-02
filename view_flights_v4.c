#include<stdio.h>
#include<stdlib.h>

typedef struct Flight {
   int flightNumber;
   int capacity;
   int bookedSeats;
   struct Flight *next;
} Flight;

void searchAvailableFlights(Flight *head) {
    printf("Available flights:\n");
    while (head) {
        printf("Flight %d: %d/%d seats booked\n", head->flightNumber, head->bookedSeats, head->capacity);
        head = head->next;
    }
}

void viewReservationHistory(Flight *head) {
    while (head) {
        printf("Flight %d:\n", head->flightNumber);
        printf("  Confirmed Reservations: %d/%d\n", head->bookedSeats, head->capacity);
        head = head->next;
    }
}