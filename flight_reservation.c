#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_NAME_LENGTH 50

typedef struct Passenger {
    char name[MAX_NAME_LENGTH];
    struct Passenger *next;
} Passenger;

typedef struct Flight {
   int flightNumber;
   int capacity;
   int bookedSeats;
   Passenger *reservations;
   Passenger *waitlist;
   struct Flight *next;
} Flight;

Flight *addFlight(Flight *head, int flightNumber, int capacity) {
    Flight *newFlight = (Flight *)malloc(sizeof(Flight));
    if (!newFlight) {
        printf("Memory allocation failed for new flight.\n");
        exit(1);  
    }

    // Initialize the new flight
    newFlight->flightNumber = flightNumber;
    newFlight->capacity = capacity;
    newFlight->bookedSeats = 0;  
    newFlight->reservations = NULL;  
    newFlight->waitlist = NULL;  
    newFlight->next = head;  

    return newFlight;  
}

Flight *findFlight(Flight *head, int flightNumber) {
    while (head) {
        if (head->flightNumber == flightNumber) {
            return head;
        }
        head = head->next;  
    }
    return NULL;  
}

void addReservation(Flight *head, char *name, int flightNumber) {
    Flight *flight = findFlight(head, flightNumber);
    if (!flight) {
        printf("Flight %d not found.\n", flightNumber);
        return;  
    }

    Passenger *newPassenger = (Passenger *)malloc(sizeof(Passenger));
    if (!newPassenger) {
        printf("Memory allocation failed for new passenger.\n");
        return;  
    }

    // Manually copy the name character by character
    int i = 0;
    while (name[i] != '\0' && i < MAX_NAME_LENGTH - 1) {
        newPassenger->name[i] = name[i];
        i++;
    }
    newPassenger->name[i] = '\0'; // Ensure the string is null-terminated
    newPassenger->next = NULL;

    if (flight->bookedSeats < flight->capacity) {
        // Add to confirmed reservations
        newPassenger->next = flight->reservations;
        flight->reservations = newPassenger;
        flight->bookedSeats++;
        printf("Reservation confirmed for %s on flight %d.\n", name, flightNumber);
    } else {
        // Add to waitlist
        newPassenger->next = flight->waitlist;
        flight->waitlist = newPassenger;
        printf("Flight %d is full. %s added to the waitlist.\n", flightNumber, name);
    }
}

void cancelReservation(Flight *head, char *name, int flightNumber) {
    Flight *flight = findFlight(head, flightNumber);
    if (!flight) {
        printf("Flight %d not found.\n", flightNumber);
        return;
    }

    Passenger *prev = NULL, *curr = flight->reservations;

    // Search in confirmed reservations
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            // Remove passenger
            if (prev) prev->next = curr->next;
            else flight->reservations = curr->next;

            free(curr);
            flight->bookedSeats--;
            printf("Reservation for %s on flight %d canceled.\n", name, flightNumber);

            // Promote the first person on the waitlist, if any
            if (flight->waitlist) {
                Passenger *promoted = flight->waitlist;
                flight->waitlist = promoted->next;
                promoted->next = flight->reservations;
                flight->reservations = promoted;
                flight->bookedSeats++;
                printf("%s has been moved from the waitlist to confirmed reservations.\n", promoted->name);
            }
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    // Search in the waitlist
    prev = NULL;
    curr = flight->waitlist;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            // Remove passenger
            if (prev) prev->next = curr->next;
            else flight->waitlist = curr->next;

            free(curr);
            printf("%s removed from the waitlist of flight %d.\n", name, flightNumber);
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    printf("%s not found in reservations or waitlist for flight %d.\n", name, flightNumber);
}

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
        printf("  Confirmed Reservations:\n");
        Passenger *curr = head->reservations;
        while (curr) {
            printf("    %s\n", curr->name);
            curr = curr->next;
        }

        printf("  Waitlist:\n");
        curr = head->waitlist;
        while (curr) {
            printf("    %s\n", curr->name);
            curr = curr->next;
        }
        head = head->next;
    }
}

void freeAll(Flight *head) {
    while (head) {
        Passenger *curr = head->reservations;
        while (curr) {
            Passenger *temp = curr;
            curr = curr->next;
            free(temp);
        }

        curr = head->waitlist;
        while (curr) {
            Passenger *temp = curr;
            curr = curr->next;
            free(temp);
        }

        Flight *temp = head;
        head = head->next;
        free(temp);
    }
}

int isValidName(char *name) {
    return (name && strlen(name) > 0 && strlen(name) < MAX_NAME_LENGTH);
}

int main() {
    Flight *head = NULL;
    int choice, flightNumber, capacity;
    char name[MAX_NAME_LENGTH];

    while (1) {
        printf("\nFlight Reservation System Menu:\n");
        printf("1. Add Flight\n");
        printf("2. Add Reservation\n");
        printf("3. Cancel Reservation\n");
        printf("4. View Available Flights\n");
        printf("5. View Reservation History\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  

        switch (choice) {
            case 1:
                // Adding a new flight
                printf("Enter flight number: ");
                scanf("%d", &flightNumber);
                printf("Enter flight capacity: ");
                scanf("%d", &capacity);

                if (capacity <= 0) {
                    printf("Capacity must be greater than 0.\n");
                    break;
                }

                head = addFlight(head, flightNumber, capacity);
                printf("Flight %d added with capacity %d.\n", flightNumber, capacity);
                break;

            case 2:
                printf("Enter flight number: ");
                scanf("%d", &flightNumber);
                getchar();  
                printf("Enter passenger name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0'; 

                if (!isValidName(name)) {
                    printf("Invalid name. It must be non-empty and less than %d characters.\n", MAX_NAME_LENGTH);
                    break;
                }

                addReservation(head, name, flightNumber);
                break;

            case 3:
                printf("Enter flight number: ");
                scanf("%d", &flightNumber);
                getchar();  
                printf("Enter passenger name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0'; 

                cancelReservation(head, name, flightNumber);
                break;

            case 4:
                searchAvailableFlights(head);
                break;

            case 5:
                viewReservationHistory(head);
                break;

            case 6:
                freeAll(head);
                printf("Exiting the program.\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
