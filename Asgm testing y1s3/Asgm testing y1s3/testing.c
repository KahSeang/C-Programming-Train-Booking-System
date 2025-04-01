#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#pragma warning(disable:4996) 
#define MAX_STAFF 10000
#define MAX_MEMBER 10000
#define MAX_SCHEDULE 10000
#define RED "\033[1;31m"    // ANSI escape code for red color
#define GREEN "\033[1;32m"  // ANSI escape code for green color
#define BLUE "\033[1;34m"   // ANSI escape code for blue color
#define RESET "\033[0m"     // ANSI escape code to reset color
#define ROWS 5
#define COLS 5
#define MAX_MEMBERS 10000
#define ID_LENGTH 100

struct Staff {
    char sId[100];
    char sName[100];
    char spass[100];
    char spassR[100];
    char sposition[100];
    char phoneNumber[15];
    char ic[20];
    char gender;
    char semail[100];
};


struct Member {
    char mId[100], mName[100], mpass[100], mpassr[100], gender, mIC[100], mContact[100], mBookId[100], memail[100];
};

struct Schedule {
    char tId[100], tDs[100], tAs[100];
    int tDtime, tAtime, tAseat, totalSeat;
    float eachPrice;
};

struct Booking {
    char bId[100], bMId[100], bTid[100], bdateId[100], bdepartId[100], bSeatN[100], bCoach, bPayment, bStatus;
    int quantity;
    double amount;
};

struct Seat {
    char trainID[5];
    char seat[200];
};

struct Staff staff[MAX_STAFF];
struct Member member[MAX_MEMBER];
struct Schedule schedule[MAX_SCHEDULE];
struct Booking booking[MAX_SCHEDULE];
struct Member loginM;
struct Staff displayS;
struct Staff staffId;
struct Staff newStaff;
struct Schedule s;


int selection = 0;

void staffM();
void memberM();
int checkMemberIDExists(char* memberId);
int checkStaffIDExists(char* staffId);
void displayTrainSchedule();
void ticket();
void registerStaff();
void registerMember();
void loginStaff();
void loginMember();
void staffmenu();
void membermenu();
void showTime();
void addSchedule();
void memberDetails();
void editSchedule();
void searchScehdule();
void deleteSchedule();
void staffProfile();
void memberProfile();
void book();
void viewbook();
void cancelBooking();
void ticketManagement();
void profilemanagement();
void modifyprofilemember();
void displayprofilemember();
void viewlist();
void stafflist();
void ticketManagement2();
void profilemanagement2();
void addstaff();
void modifyProfileStaff();
int isValidTrainID(const char* id);
int isValidStationName(const char* name);
int isValidTime(int time);
int isValidSeats(int seats);
void initializeSeatsFromFile(char seats[][COLS], const char* tId);
void displaySeats(const char* tId);
void viewAvailableSeats(char seats[][COLS]);
void menuSeat(const char* tId);
void makePayment();
void resetPassword();
void resetStaffPassword();
void displayprofilestaff();
void report();
void reportM();
void report2();
void logPayment(const char* memberId, const char* bookingId, double price, int quantity);
void generateTotalSalesReport();
//avaialable seat in seat.txt
//global variable loginM, to see who is login now
//The train schedule added total Seat.
//Modified function :  menuSeat, all the train schedule module, book, viewbook


void initializeSeatsFromFile(char seats[][COLS], const char* tId) {
    FILE* file = fopen("seat.txt", "r");
    if (file == NULL) {
        printf("Error: Unable to open seat file.\n");
        exit(EXIT_FAILURE);
    }

    char trainId[100];
    int found = 0;

    while (fscanf(file, "%s", trainId) != EOF) {
        if (strcmp(trainId, tId) == 0) {
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    if (fscanf(file, " %c", &seats[i][j]) != 1) {
                        printf("Error reading seat label.\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Error: Train ID %s not found.\n", tId);
        exit(EXIT_FAILURE);
    }
}

void viewAvailableSeats(char seats[][COLS]) {
    printf("\n--- Available Seats ---\n");
    int availableCount = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (seats[i][j] == 'O') {
                printf("(%d,%d) ", i + 1, j + 1);
                availableCount++;
            }
        }
    }
    if (availableCount == 0) {
        printf("No available seats.\n");
    }
    printf("\n-----------------------\n");

}void displaySeats(const char* tId) {
    FILE* file = fopen("seat.txt", "r");
    if (file == NULL) {
        printf("Error: Unable to open seat file.\n");
        exit(EXIT_FAILURE);
    }

    char trainId[100];
    int found = 0;

    while (fscanf(file, "%s", trainId) != EOF) {
        if (strcmp(trainId, tId) == 0) {
            printf("\n--- All Seats for Train ID %s ---\n", tId);
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    char seatLabel[5];
                    if (fscanf(file, " %s", seatLabel) != 1) {
                        printf("Error reading seat label.\n");
                        exit(EXIT_FAILURE);
                    }
                    if (strcmp(seatLabel, "|") == 0) {
                        exit(EXIT_FAILURE);
                    }
                    printf("%s ", seatLabel);

                }
                printf("\n");
            }
            printf("----------------------\n");
            found = 1;
            break;
        }
        else {
            // Skip reading the entire line for other train IDs
            int c;
            while ((c = fgetc(file)) != '\n' && c != EOF); // Skip to the end of the line
        }
    }

    fclose(file);

    if (!found) {
        printf("Error: Train ID %s not found.\n", tId);
        exit(EXIT_FAILURE);
    }
}

//validation of name
int isValidName(char* name) {
    int length = strlen(name);

    for (int i = 0; i < length; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') {
            return 0; // Name contains a non-alphabetic character (excluding space)
        }
    }

    return 1;
}

//validation of password
int isValidPassword(char* password) {
    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
    int length = strlen(password);

    if (length < 8) {
        return 0; // Password is too short
    }

    for (int i = 0; i < length; i++) {
        if (isupper(password[i])) {
            hasUpper = 1;
        }
        else if (islower(password[i])) {
            hasLower = 1;
        }
        else if (isdigit(password[i])) {
            hasDigit = 1;
        }
        else {
            hasSpecial = 1;
        }
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;
}

//validation IC
int isValidIC(char* ic) {
    int length = strlen(ic);
    if (length != 12) {
        return 0; // IC number must be 12 digits long
    }

    for (int i = 0; i < length; i++) {
        if (!isdigit(ic[i])) {
            return 0; // IC number contains non-digit characters
        }
    }

    return 1;
}

//validation phoneNum
int isValidPhoneNumber(char* phone) {
    int length = strlen(phone);

    if (length != 12 && length != 13) {
        return 0; // Phone number must be either 12 or 13 characters long
    }

    if (strncmp(phone, "60", 2) != 0) {
        return 0; // Phone number must start with "60"
    }

    if (phone[2] != '1') {
        return 0; // For 13-digit phone numbers, the third digit must be '1'
    }

    if (phone[4] != '-') {
        return 0; // Phone number must have a dash "-" at the 6th position
    }


    return 1;
}







int isValidPosition(const char* position) {
    // Basic check to ensure position is not empty and contains only alphabetic and space characters
    if (strlen(position) == 0) {
        return 0; // Empty string
    }
    for (int i = 0; i < strlen(position); ++i) {
        if (!isalpha(position[i]) && position[i] != ' ') {
            return 0; // Contains invalid characters
        }
    }
    return 1; // Valid position
}

//validation ID
int isValidMemberID(char* memberId) {
    int length = strlen(memberId);
    if (length != 4) {
        return 0; // Member ID must be 4 digits long
    }

    for (int i = 0; i < length; i++) {
        if (!isdigit(memberId[i])) {
            return 0; // Member ID contains non-digit characters
        }
    }

    return 1;
}

//validation psRecovery
int isValidPasswordRecovery(char* passwordRecovery) {
    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
    int length = strlen(passwordRecovery);

    if (length < 8) {
        return 0; // Password is too short
    }

    for (int i = 0; i < length; i++) {
        if (isupper(passwordRecovery[i])) {
            hasUpper = 1;
        }
        else if (islower(passwordRecovery[i])) {
            hasLower = 1;
        }
        else if (isdigit(passwordRecovery[i])) {
            hasDigit = 1;
        }
        else {
            hasSpecial = 1;
        }
    }
    return isValidPassword(passwordRecovery);
}

//validation gender
int isValidGender(char gender) {
    return gender == 'M' || gender == 'F';
}

int isValidPrice(float price) {
    if (price < 0) {
        return 0;
    }

    return 1;
}
void menuSeat(const char* tId) {

    FILE* fptrSt;
    struct Seat bookSeat;
    int found, num;

    fptrSt = fopen("seat.txt", "r");
    found = 0;

    num = 0;

    if (fptrSt == NULL) {
        printf("Error opening seat file.\n");
        return;
    }

    while (fscanf(fptrSt, "%4[^|]|%199[^\n]\n", bookSeat.trainID, bookSeat.seat) == 2) {

        if (strcmp(bookSeat.trainID, tId) == 0) {
            found = 1;
            printf("\n\n\n");
            printf("Train ID: %s\n", tId);
            printf("Available Seat\n");
            printf("=================");
            printf("\n%s\n\n", bookSeat.seat);
            break;

        }
        num++;
    }

    if (!found) {
        printf("Train with ID %s not found.\n", book);
    }


    system("pause");
}
int main() {
    int selection;
    struct Schedule schedule;

    printf("\n");
    showTime();
    printf("\n----------------------------------------------------------------------------\n");
    printf("                  Welcome to Train Ticket Reservation System");
    printf("\n----------------------------------------------------------------------------\n");

    do {
        printf("Please select the number:\n");
        printf("[1] Member\n[2] Staff\n[3] Train Schedule\n[4] Menu Seat \n[5] Exit\n");
        printf("Enter your choice: ");

        // Input validation loop
        while (scanf("%d", &selection) != 1) {
            // Clear the input buffer
            while (getchar() != '\n');
            printf(RED "Invalid input." RESET);
            printf("\nEnter your choice:");
        }

        switch (selection) {
        case 1:
            memberM();
            break;
        case 2:
            staffM();
            break;
        case 3:
            displayTrainSchedule();
            break;
        case 4:
            displayTrainSchedule();
            printf("Enter the Train ID: ");
            char tId[100];
            scanf("%s", tId);
            menuSeat(tId);
            break;
        case 5:
            printf("Thank you for using this system! Have a nice day!\n");
            break;

        default:
            printf(RED "Invalid choice. Please try again.\n" RESET);
            break;
        }
    } while (selection != 5);

    return 0;
}




void member1() {
    //  profile management - crud create read update delete
     // ticket management
    int choice = 0;

    do {
        printf("ENTER \n[1]PROFILE MANAGEMENT \n[2]TICKET MANAGEMENT\n[3]EXIT\n");
        scanf("%d", &choice);


    } while (choice != 3);



}




void staffM() {
    int selection = 0;


    do {
        printf("\n----------------------------------------------------------------------------\n");
        printf("                     Login/Register(Admin)");
        printf("\n----------------------------------------------------------------------------\n");

        printf("[1] LOGIN\n[2] REGISTER\n[3] Exit\n");
        printf("Please enter your choice: ");
        if (scanf("%d", &selection) != 1) {
            // If input is not an integer, clear input buffer
            while (getchar() != '\n');
            printf(RED "Invalid input. Please enter a number.\n" RESET);
            continue;
        }

        switch (selection) {
        case 1:
            loginStaff();
            break;
        case 2:
            registerStaff();
            break;
        case 3:
            main();

            break;
        default:
            printf(RED "Invalid selection. Please try again.\n" RESET);
            break;
        }
    } while (selection != 3);
}


void memberM() {
    int selection = 0;


    do {
        printf("\n---------------------------------------------\n");
        printf("                 USER Menu                      ");
        printf("\n---------------------------------------------\n");

        printf("[1] LOGIN\n[2] REGISTER\n[3] Exit\n");
        printf("Please enter your choice: ");
        if (scanf("%d", &selection) != 1) {
            // If input is not an integer, clear input buffer
            while (getchar() != '\n');
            printf(RED "Invalid input. Please enter a number.\n" RESET);
            continue;
        }


        switch (selection) {
        case 1:
            loginMember();
            break;
        case 2:
            registerMember();
            break;
        case 3:
            main();
            break;
        default:
            printf(RED "Invalid selection. Please try again.\n" RESET);
            break;
        }
    } while (selection != 3);
}



void addSchedule() {
    FILE* fptrSc, * fptrSt, * fptrStr;
    struct Schedule newSchedule;
    struct Seat addSeat[100];
    int numRecords = 0;
    int idExists = 0;

    fptrSc = fopen("scheduleTrain.txt", "a");
    fptrSt = fopen("seat.txt", "a");
    fptrStr = fopen("seat.txt", "r");

    if (fptrSc == NULL || fptrSt == NULL || fptrStr == NULL) {
        printf(RED"ERROR OPENING SCHEDULE FILE\n"RESET);
        exit(EXIT_FAILURE);
    }

    while (fscanf(fptrStr, "%4[^|]|%99[^\n]\n", addSeat[numRecords].trainID, addSeat[numRecords].seat) == 2) {
        numRecords++;
    }

    fclose(fptrStr);

    do {
        printf("Enter Train ID (4 digits): ");
        rewind(stdin);
        scanf("%s", newSchedule.tId);
        idExists = checkTrainIDExists(newSchedule.tId, addSeat, numRecords);
        if (!isValidTrainID(newSchedule.tId) || idExists) {
            printf(RED "Invalid Train ID or already exists. Please ensure it is exactly 4 digits and unique.\n"RESET);
        }
    } while (!isValidTrainID(newSchedule.tId) || idExists);

    do {
        printf("Enter Departure Station (3 uppercase letters, e.g., PNG): ");
        rewind(stdin);
        scanf("%s", newSchedule.tDs);
        if (!isValidStationName(newSchedule.tDs)) {
            printf(RED"Invalid station name. It must be exactly 3 uppercase letters.\n"RESET);
        }
    } while (!isValidStationName(newSchedule.tDs));

    do {
        printf("Enter Arrival Station (3 uppercase letters, e.g., PNG): ");
        rewind(stdin);
        scanf("%s", newSchedule.tAs);
        if (!isValidStationName(newSchedule.tAs)) {
            printf(RED"Invalid station name. It must be exactly 3 uppercase letters.\n"RESET);
        }
    } while (!isValidStationName(newSchedule.tAs));

    do {
        printf("Enter Departure Time (HHMM format): ");
        rewind(stdin);
        scanf("%d", &newSchedule.tDtime);
        if (!isValidTime(newSchedule.tDtime)) {
            printf(RED"Invalid time. Please enter time in HHMM format between 0000 and 2359.\n"RESET);
        }
    } while (!isValidTime(newSchedule.tDtime));

    do {
        printf("Enter Arrival Time (HHMM format): ");
        rewind(stdin);
        scanf("%d", &newSchedule.tAtime);
        if (!isValidTime(newSchedule.tAtime)) {
            printf(RED"Invalid time. Please enter time in HHMM format between 0000 and 2359.\n"RESET);
        }
    } while (!isValidTime(newSchedule.tAtime));

    do {
        printf("Enter Available Seats: ");
        rewind(stdin);
        scanf("%d", &newSchedule.tAseat);
        if (!isValidSeats(newSchedule.tAseat)) {
            printf(RED"Invalid number of seats. It must be a positive integer up to 25.\n"RESET);
        }
    } while (!isValidSeats(newSchedule.tAseat));


    do {
        printf("Enter Price : ");
        rewind(stdin);
        scanf("%f", &newSchedule.eachPrice);
        if (!isValidPrice(newSchedule.tAseat)) {
            printf(RED"Invalid number of seats. It must be a positive integer.\n"RESET);
        }
    } while (!isValidPrice(newSchedule.tAseat));


    newSchedule.totalSeat = newSchedule.tAseat;

    char letter = 'A';
    int seatCounter = 1;

    strcpy(addSeat[numRecords].trainID, newSchedule.tId);
    addSeat[numRecords].seat[0] = '\0';
    for (int i = 0; i < newSchedule.totalSeat; i++) {
        sprintf(addSeat[numRecords].seat + strlen(addSeat[numRecords].seat), "%c%02d ", letter, seatCounter);
        seatCounter++;

        if (seatCounter > 10) {
            seatCounter = 1;
            letter++;
        }
    }

    fprintf(fptrSt, "%s|%s\n", addSeat[numRecords].trainID, addSeat[numRecords].seat);



    fprintf(fptrSc, "%s|%s|%s|%d|%d|%d|%d|%.2f|\n",
        newSchedule.tId, newSchedule.tDs, newSchedule.tAs,
        newSchedule.tDtime, newSchedule.tAtime, newSchedule.tAseat, newSchedule.totalSeat, newSchedule.eachPrice);

    fclose(fptrSc);
    fclose(fptrSt);
    printf(GREEN "Schedule added successfully!\n"RESET);

    ticketManagement2();
}

int checkTrainIDExists(char* trainID, struct Seat addSeat[], int numRecords) {
    for (int i = 0; i < numRecords; i++) {
        if (strcmp(trainID, addSeat[i].trainID) == 0) {
            return 1; // Train ID already exists
        }
    }
    return 0; // Train ID does not exist
}

void editSchedule() {

    FILE* fptrSc, * fttrSc, * fptrSt, * tempSt;
    struct Schedule currentSchedule[1000];
    struct Seat aSeat;
    char scheduleId[100];
    int found = 0, i = 0, count = 0, choice = 0, edit = 0;

    fptrSc = fopen("scheduleTrain.txt", "r");
    fptrSt = fopen("seat.txt", "r");

    if (fptrSc == NULL || fptrSt == NULL) {
        printf(RED "Error opening schedule file. Please make sure the file exists and is not being accessed by another program.\n" RESET);
        return;
    }


    tempSt = fopen("temp.txt", "w");
    if (tempSt == NULL) {
        printf(RED"Error creating temporary file!\n" RESET);
        return;
    }

    displayTrainSchedule();

    printf("Enter the ID of the schedule to edit: ");
    scanf("%s", scheduleId);

    // Load schedules into array
    while (fscanf(fptrSc, "%[^|]|%[^|]|%[^|]|%d|%d|%d|%d|%f|\n",
        currentSchedule[i].tId, currentSchedule[i].tDs, currentSchedule[i].tAs,
        &currentSchedule[i].tDtime, &currentSchedule[i].tAtime, &currentSchedule[i].tAseat, &currentSchedule[i].totalSeat, &currentSchedule[i].eachPrice) != EOF) {

        if (strcmp(currentSchedule[i].tId, scheduleId) == 0) {
            found = 1;
            printf("Current Details of Schedule with ID %s:\n", currentSchedule[i].tId);
            printf("1. Departure Station: %s\n", currentSchedule[i].tDs);
            printf("2. Arrival Station: %s\n", currentSchedule[i].tAs);
            printf("3. Departure Time: %d\n", currentSchedule[i].tDtime);
            printf("4. Arrival Time: %d\n", currentSchedule[i].tAtime);
            printf("5. Available Seats: %d\n", currentSchedule[i].tAseat);
            printf("6. Price: %.2f\n", currentSchedule[i].eachPrice);
            printf("Enter the number of the field you want to edit: ");
            rewind(stdin);
            scanf("%d", &choice);

            switch (choice) {
            case 1:
                printf("Enter New Departure Station: ");
                rewind(stdin);
                scanf("%s", currentSchedule[i].tDs);
                break;
            case 2:
                printf("Enter New Arrival Station: ");
                rewind(stdin);
                scanf("%s", currentSchedule[i].tAs);
                break;
            case 3:
                printf("Enter New Departure Time (HHMM): ");
                rewind(stdin);
                scanf("%d", &currentSchedule[i].tDtime);
                break;
            case 4:
                printf("Enter New Arrival Time (HHMM): ");
                rewind(stdin);
                scanf("%d", &currentSchedule[i].tAtime);
                break;
            case 5:

                if (currentSchedule[i].tAseat < currentSchedule[i].totalSeat) {
                    printf(RED"Cannot be edit, someone has book this train\n" RESET);
                }
                else {
                    printf("Enter New Available Seats: ");
                    rewind(stdin);
                    scanf("%d", &currentSchedule[i].tAseat);

                    edit = 1;

                    while (fscanf(fptrSt, "%4[^|]|%199[^\n]\n", aSeat.trainID, aSeat.seat) == 2) {
                        if (strcmp(currentSchedule[i].tId, aSeat.trainID) == 0) {
                            char letter = 'A';
                            int seatCounter = 1;

                            aSeat.seat[0] = '\0';
                            for (int k = 0; k < currentSchedule[i].tAseat; k++) {
                                sprintf(aSeat.seat + strlen(aSeat.seat), "%c%02d ", letter, seatCounter);
                                seatCounter++;

                                if (seatCounter > 10) {
                                    seatCounter = 1;
                                    letter++;
                                }
                            }

                            fprintf(tempSt, "%s|%s\n", aSeat.trainID, aSeat.seat);
                        }
                        else {

                            fprintf(tempSt, "%s|%s\n", aSeat.trainID, aSeat.seat);
                        }
                    }

                    currentSchedule[i].totalSeat = currentSchedule[i].tAseat;
                }

                break;
            case 6:
                printf("Enter Price: ");
                rewind(stdin);
                scanf("%f", &currentSchedule[i].eachPrice);

                break;
            default:
                printf(RED "Invalid choice. No changes made.\n" RESET);
            }

            printf(GREEN"Schedule updated successfully!\n" RESET);
        }
        currentSchedule[i].totalSeat = currentSchedule[i].tAseat;

        i++;
    }
    count = i;

    fttrSc = fopen("scheduleTrain.txt", "w");
    for (i = 0; i < count; i++) {

        fprintf(fttrSc, "%s|%s|%s|%d|%d|%d|%d|%.2f|\n",
            currentSchedule[i].tId, currentSchedule[i].tDs, currentSchedule[i].tAs,
            currentSchedule[i].tDtime, currentSchedule[i].tAtime, currentSchedule[i].tAseat, currentSchedule[i].totalSeat, currentSchedule[i].eachPrice);

    }


    if (!found) {
        printf(RED "Schedule with ID %s not found.\n" RESET, scheduleId);
    }

    fclose(fptrSt);
    fclose(fttrSc);
    fclose(fptrSc);
    fclose(tempSt);
    remove("seat.txt");
    if (rename("temp.txt", "seat.txt") == 0) {
        printf(GREEN"File renamed successfully.\n" RESET);
    }
    else {
        printf(RED"Error renaming file.\n"  RESET);
    }

}

void searchScehdule() {
    int i = 0;
    char searchId[100];
    FILE* ptr;
    struct Schedule currentSchedule;

    ptr = fopen("scheduleTrain.txt", "r");
    if (ptr == NULL) {
        printf(RED "Error opening schedule file.\n" RESET);
        return;
    }

    printf("Enter the ID that you want to search for: ");
    scanf("%s", searchId);

    int found = 0;

    // Loop through each schedule in the file
    while (fscanf(ptr, "%[^|]|%[^|]|%[^|]|%d|%d|%d|%d|%f|\n",
        currentSchedule.tId, currentSchedule.tDs, currentSchedule.tAs,
        &currentSchedule.tDtime, &currentSchedule.tAtime, &currentSchedule.tAseat, &currentSchedule.totalSeat, &currentSchedule.eachPrice) != EOF) {


        if (strcmp(searchId, currentSchedule.tId) == 0) {
            printf("Schedule found:\n");
            printf("Train ID: %s\n", currentSchedule.tId);
            printf("Departure Station: %s\n", currentSchedule.tDs);
            printf("Arrival Station: %s\n", currentSchedule.tAs);
            printf("Departure Time: %d\n", currentSchedule.tDtime);
            printf("Arrival Time: %d\n", currentSchedule.tAtime);
            printf("Available Seats: %d\n", currentSchedule.tAseat);
            printf("Total Seats: %d\n", currentSchedule.totalSeat);
            printf("Price: %.2f\n", currentSchedule.eachPrice);
            found = 1;
            break; // No need to continue searching once found
        }
    }

    if (!found) {
        printf(RED "Schedule with ID %s not found.\n" RESET, searchId);
    }

    // Close the file
    fclose(ptr);
}

void deleteSchedule() {

    FILE* fptr;
    struct Schedule currentSchedule[1000];
    char delete[100], ans;
    int i = 0, a, found = 0;

    fptr = fopen("scheduleTrain.txt", "r");
    if (fptr == NULL) {
        printf(RED "Error opening schedule file.\n" RESET);
        return;
    }

    while (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%d|%d|%d|%d|%f|\n",
        currentSchedule[i].tId, currentSchedule[i].tDs, currentSchedule[i].tAs,
        &currentSchedule[i].tDtime, &currentSchedule[i].tAtime, &currentSchedule[i].tAseat, &currentSchedule[i].totalSeat, &currentSchedule[i].eachPrice) != EOF) {

        i++;
    }
    fclose(fptr);

    displayTrainSchedule();

    printf("Enter the ID of the schedule to delete:  ");
    scanf("%s", delete);
    rewind(stdin);

    for (a = 0; a < i; a++) {
        if (strcmp(currentSchedule[a].tId, delete) == 0) {
            printf("Current Details of Schedule with ID %s:\n", delete);
            printf("Departure Station: %s\n", currentSchedule[a].tDs);
            printf("Arrival Station: %s\n", currentSchedule[a].tAs);
            printf("Departure Time: %d\n", currentSchedule[a].tDtime);
            printf("Arrival Time: %d\n", currentSchedule[a].tAtime);
            printf("Available Seats: %d\n", currentSchedule[a].tAseat);
            printf("Total Seats: %d\n", currentSchedule[a].totalSeat);
            printf("Price: %.2f\n", currentSchedule[a].eachPrice);
            printf("\n\n");
            found = 1;

            do {
                printf("\nComfirm to Delete? (Y = Yes)?> ");
                rewind(stdin);
                scanf("%c", &ans);

                if (toupper(ans) == 'Y')
                {

                    fptr = fopen("scheduleTrain.txt", "w");

                    for (a = 0; a < i; a++) {
                        if (strcmp(currentSchedule[a].tId, delete) != 0) {
                            fprintf(fptr, "%s|%s|%s|%d|%d|%d|%d|%.2f|\n",
                                currentSchedule[a].tId, currentSchedule[a].tDs, currentSchedule[a].tAs,
                                currentSchedule[a].tDtime, currentSchedule[a].tAtime, currentSchedule[a].tAseat, currentSchedule[a].totalSeat, currentSchedule[a].eachPrice);
                        }
                    }

                    printf(GREEN "Schedule with ID %s delete successfully!\n" RESET, delete);

                    fclose(fptr);

                }
                else if (toupper(ans) == 'N')
                {
                    printf(RED "No record deleted..\n" RESET);
                }
                else {
                    printf("Please type it again (Y/N) \n");
                }


            } while (toupper(ans) != 'Y' && toupper(ans) != 'N');


        }
    }
    if (!found) {
        printf(RED "Schedule with ID %s not found.\n" RESET, delete);
    }

}

//added Total Seat
void displayTrainSchedule() {
    int count = 0;
    FILE* fptrSc;
    struct Schedule currentSchedule;

    fptrSc = fopen("scheduleTrain.txt", "r");
    if (fptrSc == NULL) {
        printf(RED "Error opening schedule file.\n" RESET);
        return;
    }

    printf("------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| Train ID | Departure Station | Arrival Station | Departure Time | Arrival Time | Available Seats | Total Seat | Price      |\n");
    printf("------------------------------------------------------------------------------------------------------------------------------\n");

    while (fscanf(fptrSc, "%[^|]|%[^|]|%[^|]|%d|%d|%d|%d|%f|\n",
        currentSchedule.tId, currentSchedule.tDs, currentSchedule.tAs,
        &currentSchedule.tDtime, &currentSchedule.tAtime, &currentSchedule.tAseat, &currentSchedule.totalSeat, &currentSchedule.eachPrice) != EOF) {
        printf("| %-9s| %-18s| %-16s| %-15d| %-13d| %-15d | %-10d | %.2f      |\n",
            currentSchedule.tId, currentSchedule.tDs, currentSchedule.tAs,
            currentSchedule.tDtime, currentSchedule.tAtime, currentSchedule.tAseat, currentSchedule.totalSeat, currentSchedule.eachPrice);
        count++;
    }
    printf("------------------------------------------------------------------------------------------------------------------------------\n");

    printf(GREEN "\nTOTAL %d schedules have been displayed.\n" RESET, count);
    fclose(fptrSc);
}


// HAI MWI YOU ZUO
void ticket() {
    FILE* fptrt;
    fptrt = fopen("ticket.txt", "r");
    if (fptrt == NULL) {
        printf(RED "ERROR OPENING SCHEDULE\n" RESET);
        exit(-1);
    }

    fclose(fptrt);
}


void registerMember() {
    FILE* log;
    struct Member newMember;
    char password[100];
    int validPassword = 0;
    int validName = 0;
    int validIC = 0;
    int validPhoneNumber = 0;
    int validMemberID = 0;
    int validPasswordRecovery = 0;
    int validGender = 0;
    int idExists = 0;

    log = fopen("memberRegister.bin", "ab+");
    if (log == NULL) {
        printf(RED "Error opening file.\n" RESET);
        exit(-1);
    }

    do {
        printf("Enter your member ID: ");
        rewind(stdin);
        scanf("%s", newMember.mId);
        idExists = checkMemberIDExists(newMember.mId);
        if (!isValidMemberID(newMember.mId) || idExists) {
            printf(RED "Invalid member ID or already exists. Must be 4 digits and unique. Please try again.\n\n" RESET);
        }
        else {
            validMemberID = 1;
        }
    } while (!validMemberID || idExists);

    do {
        printf("Enter your name: ");
        rewind(stdin);
        scanf("%[^\n]", newMember.mName);
        if (!isValidName(newMember.mName)) {
            printf(RED "Name must contain only alphabetic characters.\n\n" RESET);
        }
        else {
            validName = 1;
        }
    } while (!validName);

    do {
        printf("Enter your password: ");

        scanf("%s", password);
        if (!isValidPassword(password)) {
            printf(RED "Password must be at least 8 characters long and contain at least one uppercase letter, one lowercase letter, one digit, and one special character.\n\n" RESET);
        }
        else {
            validPassword = 1;
            strcpy(newMember.mpass, password);
        }
    } while (!validPassword);


    do {
        printf("Enter your gender (M/F): ");
        scanf(" %c", &newMember.gender);

        newMember.gender = toupper(newMember.gender);

        if (!isValidGender(newMember.gender)) {
            printf(RED "Invalid gender. Please enter 'M' or 'F'.\n\n" RESET);
        }
        else {
            validGender = 1;
        }
    } while (!validGender);

    do {
        printf("Enter your IC number (MyKad): ");
        scanf("%s", newMember.mIC);
        if (!isValidIC(newMember.mIC)) {
            printf(RED "IC number must be 12 digits long.\n\n" RESET);
        }
        else {
            validIC = 1;
        }
    } while (!validIC);

    do {
        printf("Enter your contact number (format 601X-XXXXXXXXX): ");
        scanf("%s", newMember.mContact);
        if (!isValidPhoneNumber(newMember.mContact)) {
            printf(RED "Invalid phone number format. Please enter in the format '601X-XXXXXXXXX'.\n\n" RESET);
        }
        else {
            validPhoneNumber = 1;
        }
    } while (!validPhoneNumber);

    fwrite(&newMember, sizeof(struct Member), 1, log);
    fclose(log);
    printf(GREEN "Registration successful!\n\n" RESET);
}

int checkStaffIDExists(char* staffId) {
    FILE* log;
    struct Staff existingStaff;

    log = fopen("staffRegister.bin", "rb");
    if (log == NULL) {
        printf(RED "Error opening file.\n" RESET);
        exit(-1);
    }

    while (fread(&existingStaff, sizeof(struct Staff), 1, log)) {
        if (strcmp(staffId, existingStaff.sId) == 0) {
            fclose(log);
            return 1; // Staff ID already exists
        }
    }

    fclose(log);
    return 0; // Staff ID does not exist
}
int checkMemberIDExists(char* memberId) {
    FILE* log;
    struct Member existingMember;

    log = fopen("memberRegister.bin", "rb");
    if (log == NULL) {
        printf(RED "Error opening file.\n" RESET);
        exit(-1);
    }

    while (fread(&existingMember, sizeof(struct Member), 1, log)) {
        if (strcmp(memberId, existingMember.mId) == 0) {
            fclose(log);
            return 1; // Member ID already exists
        }
    }

    fclose(log);
    return 0; // Member ID does not exist
}
void registerStaff() {
    FILE* log;
    

    char password[100];
    int validPassword = 0;
    int validName = 0;
    int validIC = 0;
    int validPhoneNumber = 0;
    int validStaffID = 0;
    int validPasswordRecovery = 0;
    int validGender = 0;
    int validPosition = 0;

    log = fopen("staffRegister.bin", "ab");
    if (log == NULL) {
        printf(RED "Error opening file.\n" RESET);
        exit(-1);
    }

    do {
        printf("Enter your staff ID: ");
        scanf("%s", newStaff.sId);
        if (!isValidMemberID(newStaff.sId)) {
            printf(RED "Invalid staff ID. Must be in 4 digits. Please try again.\n\n" RESET);
        }
        else if (checkStaffIDExists(newStaff.sId)) {
            printf(RED "Staff ID already exists. Please choose a different one.\n\n" RESET);
        }
        else {
            validStaffID = 1;
        }
    } while (!validStaffID);

    do {
        printf("Enter your name: ");
        scanf(" %[^\n]", newStaff.sName); // Note: 99 is the maximum field width
        if (!isValidName(newStaff.sName)) {
            printf(RED "Name must contain only alphabetic characters.\n\n" RESET);
        }
        else {
            validName = 1;
        }
    } while (!validName);

    do {
        printf("Enter your password: ");
        scanf("%s", password);
        if (!isValidPassword(password)) {
            printf(RED "Password must be at least 8 characters long and contain at least one uppercase letter, one lowercase letter, one digit, and one special character.\n\n" RESET);
        }
        else {
            validPassword = 1;
            strcpy(newStaff.spass, password); // Copy valid password to struct
        }
    } while (!validPassword);

    do {
        printf("Enter your gender (M/F): ");
        scanf(" %c", &newStaff.gender);
        newStaff.gender = toupper(newStaff.gender);
        if (!isValidGender(newStaff.gender)) {
            printf(RED "Invalid gender. Please enter 'M' or 'F'.\n\n" RESET);
        }
        else {
            validGender = 1;
        }
    } while (!validGender);

    do {
        printf("Enter your IC number (MyKad): ");
        scanf("%s", newStaff.ic);
        if (!isValidIC(newStaff.ic)) {
            printf(RED "IC number must be 12 digits long.\n\n" RESET);
        }
        else {
            validIC = 1;
        }
    } while (!validIC);

    do {
        printf("Enter your contact number (format 601X-XXXXXXXXX): ");
        scanf("%s", newStaff.phoneNumber);
        if (!isValidPhoneNumber(newStaff.phoneNumber)) {
            printf(RED "Invalid phone number format. Please enter in the format '6011-XXXXXXXXX'.\n\n" RESET);
        }
        else {
            validPhoneNumber = 1;
        }
    } while (!validPhoneNumber);

    do {
        printf("Enter your position: ");
        scanf(" %[^\n]", newStaff.sposition);
        if (strlen(newStaff.sposition) > 0) { // Basic validation to check if something was entered
            validPosition = 1;
        }
        else {
            printf(RED "Position cannot be empty. Please enter your position.\n\n" RESET);
        }
    } while (!validPosition);

    fwrite(&newStaff, sizeof(struct Staff), 1, log);
    fclose(log);
    printf(GREEN "Registration successful!\n\n" RESET);
}

void loginMember() {
    FILE* log;
    struct Member currentMember;
    char memberId[100], password[100];

    log = fopen("memberRegister.bin", "rb");
    if (log == NULL) {
        printf(RED "Error opening file.\n" RESET);
        exit(EXIT_FAILURE);
    }

    printf("=======MEMBER LOGIN======\n");
    printf("Enter your user ID: ");
    scanf("%s", memberId);
    printf("Enter your password: ");
    scanf("%s", password);

    int loggedIn = 0;

    while (fread(&currentMember, sizeof(struct Member), 1, log)) {
        if (strcmp(memberId, currentMember.mId) == 0 && strcmp(password, currentMember.mpass) == 0) {
            printf(GREEN "SUCCESSFUL LOGIN\n" RESET);
            loggedIn = 1;
            loginM = currentMember;
            membermenu();
            break;
        }
    }

    if (!loggedIn) {
        printf(RED "LOGIN FAILED\n" RESET);
        char choice;
        printf("Forgot your password? (Y/N): ");
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y') {
            resetPassword();
        }
    }

    fclose(log);
}

void resetPassword() {
    FILE* log;
    struct Member newMember;
    char memberId[100], icNumber[100], newPassword[100];

    log = fopen("memberRegister.bin", "rb+");
    if (log == NULL) {
        printf(RED "Error opening file.\n" RESET);
        exit(EXIT_FAILURE);
    }

    printf("=======RESET PASSWORD======\n");
    printf("Enter your member ID: ");
    scanf("%s", memberId);

    printf("Enter your IC number: ");
    scanf("%s", icNumber);

    int found = 0;

    long position; // To store the position of the record in the file

    while (fread(&newMember, sizeof(struct Member), 1, log)) {

        if (strcmp(icNumber, newMember.mIC) == 0 && strcmp(memberId, newMember.mId) == 0) {
            found = 1;
            int validPassword = 0;
            do {
                printf("Enter your new password: ");
                scanf("%s", newPassword);

                // Validate the new password
                if (!isValidPassword(newPassword)) {
                    printf(RED "Invalid password. Password must be at least 8 characters long and contain at least one uppercase letter, one lowercase letter, one digit, and one special character.\n" RESET);
                }
                else {
                    validPassword = 1;
                }
            } while (!validPassword);

            // Store the position of the record in the file
            position = ftell(log) - sizeof(struct Member);
            // Update the password in the struct
            strcpy(newMember.mpass, newPassword);
            // Move file pointer back to the beginning of the record
            fseek(log, position, SEEK_SET);
            // Write the updated struct back to the file
            fwrite(&newMember, sizeof(struct Member), 1, log);
            printf(GREEN "Password reset successfully.\n" RESET);
            break;
        }
    }

    if (!found) {
        printf(RED "No matching record found for the provided member ID and IC number.\n" RESET);
    }

    fclose(log);
}




void loginStaff() {
    FILE* log;
    
    char staffId[100], staffPassword[100];

    log = fopen("staffRegister.bin", "rb+");
    if (log == NULL) {
        printf(RED "Error opening file.\n" RESET);
        exit(EXIT_FAILURE);
    }

    printf("Enter your staff ID: ");
    scanf("%s", staffId);
    printf("Enter your password: ");
    scanf("%s", staffPassword);

    int loggedIn = 0;

    while (fread(&newStaff, sizeof(struct Staff), 1, log)) {
        
        if (strcmp(staffId, newStaff.sId) == 0 && strcmp(staffPassword, newStaff.spass) == 0) {
            printf(GREEN "SUCCESSFUL LOGIN\n" RESET);
            loggedIn = 1;
            displayS = newStaff; // Assign the details of the logged-in staff to displayS
            staffmenu();
            break;
        }
    }

    if (!loggedIn) {
        printf(RED "LOGIN FAILED\n" RESET);
        char choice;
        printf("Forgot your password? (Y/N): ");
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y') {
            resetStaffPassword();
        }
    }

    fclose(log);
}



void resetStaffPassword() {
    FILE* log;
    struct Staff currentStaff;
    char staffId[100];

    log = fopen("staffRegister.bin", "r+");
    if (log == NULL) {
        printf(RED "Error opening file.\n" RESET);
        exit(EXIT_FAILURE);
    }

    printf("Enter your staff ID: ");
    scanf("%s", staffId);

    int reset = 0;

    while (fread(&currentStaff, sizeof(struct Staff), 1, log)) {
        if (strcmp(staffId, currentStaff.sId) == 0) {
            printf("Enter new password: ");
            scanf("%s", currentStaff.spass);

            fseek(log, sizeof(struct Staff), SEEK_CUR);
            fwrite(&currentStaff, sizeof(struct Staff), 1, log);

            reset = 1;
            break;
        }
    }

    if (reset) {
        printf(GREEN "Password reset successful\n" RESET);
    }
    else {
        printf(RED "Staff ID not found\n" RESET);
    }

    fclose(log);
}



void staffmenu() {
    int choice = 0;

    // Read the train schedule data


    do {
        printf("\n---------------------------------------------\n");
        printf("                STAFF Menu                      ");
        printf("\n---------------------------------------------\n");

        printf("[1] View List\n[2] Ticket Management\n[3] Profile Management\n[4] Report\n[5] Exit\n");
        printf("Please enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            // If input is not an integer, clear input buffer
            while (getchar() != '\n');
            printf(RED "Invalid input. Please enter a number.\n" RESET);
            continue;
        }


        switch (choice) {
        case 1:
            viewlist();
            break;
        case 2:
            // Modify Schedule //bugggggg
            ticketManagement2();
            break;
        case 3:
            // Search Schedule // bugggggg
            profilemanagement2();
            break;
        case 5:
            staffM();
            break;
        case 4:
            reportM();
            break;
        default:
            printf(RED "Invalid selection. Please try again.\n" RESET);
            break;
        }
    } while (choice != 5);
}
void reportM() {
    int choice;

    do {
        // Display the menu
        printf("====================================");
        printf("\n            Report Menu\n");
        printf("====================================\n");
        printf("[1] Generate Payment Report\n");
        printf("[2] Generate Pending Report\n");
        printf("[3] Generate Total Sales Report\n");
        printf("[4] Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            // If input is not an integer, clear input buffer
            while (getchar() != '\n');
            printf(RED "Invalid input. Please enter a number.\n" RESET);
            continue;
        }



        switch (choice) {
        case 1:
            report();
            break;
        case 2:
            report2();
            break;
        case 3:
            generateTotalSalesReport();
            break;
        case 4:
            staffmenu();
            break;
        default:
            printf("Invalid choice. Please enter a number between 1 and 4.\n");
        }
    } while (choice != 4);
}

void generateTotalSalesReport() {
    FILE* bookingFile, * scheduleFile;
    char memberId[100], bookingId[100], trainId[100], departure[100], arrival[100], seatNumber[5], tStatus[15];
    float price2 = 0, totalSales = 0; // Add totalSales variable to store the total amount
    int departureTime, arrivalTime;

    bookingFile = fopen("booking.txt", "r");
    scheduleFile = fopen("scheduleTrain.txt", "r");

    if (bookingFile == NULL || scheduleFile == NULL) {
        printf(RED "Error opening files.\n" RESET);
        return;
    }

    printf("\n---------------------------------------------------------------------------------------------------------------\n");
    printf("                                                 Total Sales Report                     ");
    printf("\n---------------------------------------------------------------------------------------------------------------\n");
    printf("| %-10s | %-10s | %-10s | %-15s | %-15s | %-8s | %-8s | %-10s |\n", "Member ID", "Booking ID", "Train ID", "Departure", "Arrival", "Dep Time", "Arr Time", "Price");
    printf("---------------------------------------------------------------------------------------------------------------\n");

    while (fscanf(bookingFile, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%[^|]|%s\n",
        memberId, bookingId, trainId, departure, arrival, &departureTime, &arrivalTime, seatNumber, tStatus) != EOF) {
        if (strcmp(tStatus, "Pending") != 0) { // Check if status is not "Pending"
            struct Schedule currentSchedule;
            int found = 0;
            // Search for the corresponding train schedule
            while (fscanf(scheduleFile, "%[^|]|%[^|]|%[^|]|%d|%d|%d|%d|%f|\n",
                currentSchedule.tId, currentSchedule.tDs, currentSchedule.tAs,
                &currentSchedule.tDtime, &currentSchedule.tAtime, &currentSchedule.tAseat, &currentSchedule.totalSeat, &currentSchedule.eachPrice) != EOF) {
                if (strcmp(currentSchedule.tId, trainId) == 0) {
                    found = 1;
                    break;
                }
            }
            if (found) {
                printf("| %-10s | %-10s | %-10s | %-15s | %-15s | %02d:%02d    | %02d:%02d    | %-10.2f |\n", memberId, bookingId, trainId, departure, arrival, departureTime / 100, departureTime % 100, arrivalTime / 100, arrivalTime % 100, currentSchedule.eachPrice);
                price2 += currentSchedule.eachPrice;
            }
            rewind(scheduleFile); // Reset file pointer to beginning for next search
        }
    }

    // Calculate total sales
    totalSales = price2;

    fclose(bookingFile);
    fclose(scheduleFile);
    printf("---------------------------------------------------------------------------------------------------------------\n");
    printf("                                                                                           Total Sales: %.2f\n", totalSales); // Print the total amount
    printf("---------------------------------------------------------------------------------------------------------------\n");
}


void logPayment(const char* memberId, const char* bookingId, double price, int quantity) {
    FILE* file = fopen("sales.log", "a");
    if (file == NULL) {
        perror("Failed to open sales record file");
        return;
    }
    double totalAmount = price * quantity;
    fprintf(file, "%s %s %.2f %d %.2f\n", memberId, bookingId, price, quantity, totalAmount);
    fclose(file);
}
void viewlist() {
    int choice = 0;

    do {
        printf("\n---------------------------------------------\n");
        printf("                View List                      ");
        printf("\n---------------------------------------------\n");
        printf("[1] Member List\n[2] Staff List\n[3] Exit\n");
        printf("Please enter your choice:");
        if (scanf("%d", &choice) != 1) {
            // If input is not an integer, clear input buffer
            while (getchar() != '\n');
            printf(RED "Invalid input. Please enter a number.\n" RESET);
            continue;
        }

        switch (choice)
        {
        case 1:
            memberDetails();
            break;
        case 2:
            stafflist();
        case 3:
            staffmenu();
        default:
            printf(RED "Invalid selection. Please try again.\n" RESET);
            break;
        }
    } while (choice != 3);
}

void stafflist() {

    FILE* log;
    struct Staff staff;

    log = fopen("staffRegister.bin", "rb"); // Open in binary mode
    if (log == NULL) {
        printf(RED "Error opening file.\n" RESET);
        exit(EXIT_FAILURE);
    }

    printf("\n\n=======================================\n\t\tStaff Details \n=======================================\n");

    while (fread(&staff, sizeof(struct Staff), 1, log)) {
        printf("Member ID: %s\n", staff.sId);
        printf("Name: %s\n", staff.sName);
        printf("Gender: %c\n", staff.gender);
        printf("IC: %s\n", staff.ic);
        printf("Contact Number: %s\n", staff.phoneNumber);
        printf("Position: %s\n\n", staff.sposition);
    }

    fclose(log);
}

void ticketManagement2() {

    int choice = 0;

    do {
        printf("\n---------------------------------------------\n");
        printf("                Ticket Management                      ");
        printf("\n---------------------------------------------\n");
        printf("[1] Add Schedule\n[2] Modify Schedule\n[3] Delete Schedule\n[4] Search Schedule\n[5] Exit\n");
        printf("Please enter your choice:");
        if (scanf("%d", &choice) != 1) {
            // If input is not an integer, clear input buffer
            while (getchar() != '\n');
            printf(RED "Invalid input. Please enter a number.\n" RESET);
            continue;
        }

        switch (choice)
        {
        case 1:
            addSchedule();
            break;
        case 2:
            editSchedule();
            break;

        case 3:
            deleteSchedule();
            break;

        case 4:
            searchScehdule();
            break;

        case 5:
            staffmenu();
            break;

        default:
            printf(RED "Invalid selection. Please try again.\n" RESET);
            break;
        }
    } while (choice != 5);
}

void profilemanagement2() {

    int choice = 0;

    do {
        printf("\n---------------------------------------------\n");
        printf("                Profile Management                      ");
        printf("\n---------------------------------------------\n");
        printf("[1] Add Staff\n[2] Modify Profile\n[3] Display Profile\n[4] Exit\n");
        printf("Please select your choice:");
        if (scanf("%d", &choice) != 1) {
            // If input is not an integer, clear input buffer
            while (getchar() != '\n');
            printf(RED "Invalid input. Please enter a number.\n" RESET);
            continue;
        }

        switch (choice)
        {
        case 1:
            addstaff();
            break;
        case 2:
            modifyProfileStaff();
            break;

        case 3:
            displayprofilestaff();
            break;

        case 4:
            staffmenu();
            break;

        default:
            printf(RED "Invalid Input.Please re-enter." RESET);
            break;
        }
    } while (choice != 4);
}

void addstaff() {
    printf("\n---------------------------------------------\n");
    printf("                Add Staff                      ");
    printf("\n---------------------------------------------\n");

    registerStaff();


}


void modifyProfileStaff() {
    FILE* file;
    struct Staff staff;
    char id[100];
    char newPassword[100], newPosition[100], newPhoneNumber[100];
    char newIC[100];
    char newGender;
    int choice;

    file = fopen("staffRegister.bin", "rb+");

    if (file == NULL) {
        printf(RED "Error: Unable to open file.\n" RESET);
        exit(-1);
    }

    // Prompt user for staff ID
    printf("Enter staff ID: ");
    scanf("%s", id);

    // Search for staff ID in the file
    while (fread(&staff, sizeof(struct Staff), 1, file)) {
        if (strcmp(staff.sId, id) == 0) {
            // Display menu for field selection
            printf("Select field to modify:\n");
            printf("1. Password\n");
            printf("2. Position\n");
            printf("3. Phone Number\n");
            printf("4. Gender\n");
            printf("5. IC (Identification Card) Number\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            // Process user choice
            switch (choice) {
            case 1:
                // Prompt user for new password
                printf("Enter new password: ");
                scanf("%s", newPassword);
                if (!isValidPassword(newPassword)) {
                    printf(RED "Invalid password format. Password must have at least 8 characters, including uppercase, lowercase, digit, and special character.\n" RESET);
                    fclose(file);
                    return;
                }
                strcpy(staff.spass, newPassword);
                break;
            case 2:
                // Prompt user for new position
                printf("Enter new position: ");
                scanf("%s", newPosition);
                if (!isValidPosition(newPosition)) {
                    printf(RED "Invalid position format. Please enter a valid position.\n" RESET);
                    fclose(file);
                    return;
                }
                strcpy(staff.sposition, newPosition);
                break;
            case 3:
                // Prompt user for new phone number
                printf("Enter new phone number: ");
                scanf("%s", newPhoneNumber);
                if (!isValidPhoneNumber(newPhoneNumber)) {
                    printf(RED "Invalid phone number format. Please enter a valid phone number.\n" RESET);
                    fclose(file);
                    return;
                }
                strcpy(staff.phoneNumber, newPhoneNumber);
                break;
            case 4:
                // Prompt user for new gender
                printf("Enter new gender (M/F): ");
                scanf(" %c", &newGender);
                newGender = toupper(newGender);
                if (!isValidGender(newGender)) {
                    printf(RED "Invalid gender format. Please enter 'M' for male or 'F' for female.\n" RESET);
                    fclose(file);
                    return;
                }
                staff.gender = newGender;
                break;
            case 5:
                // Prompt user for new IC number
                printf("Enter new IC number: ");
                scanf("%s", newIC);
                if (!isValidIC(newIC)) {
                    printf(RED "Invalid IC number format. IC number must be 12 digits long and contain only digits.\n" RESET);
                    fclose(file);
                    return;
                }
                strcpy(staff.ic, newIC);
                break;
            default:
                printf(RED "Invalid choice.\n" RESET);
                break;
            }

            // Move file pointer to the beginning of the record
            long pos = ftell(file) - sizeof(struct Staff);
            fseek(file, pos, SEEK_SET);

            // Write the modified record back to the file
            fwrite(&staff, sizeof(struct Staff), 1, file);

            displayS = staff;

            printf(GREEN "Profile updated successfully.\n" RESET);
            break;
        }
    }

    // Close the file
    fclose(file);
}



//bug
void displayprofilestaff() {



    // Print the profile details
    printf("\n---------------------------------------------\n");
    printf("                 Display Profile                    ");
    printf("\n---------------------------------------------\n");

    printf("Staff ID: %s\n", displayS.sId);
    printf("Name: %s\n", displayS.sName);
    printf("Gender: %c\n", displayS.gender);
    printf("IC: %s\n", displayS.ic);
    printf("Contact Number: %s\n", displayS.phoneNumber);
    printf("Position: %s\n\n", displayS.sposition);
}





int isValidTrainID(const char* id) {
    int length = strlen(id);
    if (length != 4) return 0;
    for (int i = 0; i < length; i++) {
        if (id[i] < '0' || id[i] > '9') return 0;
    }
    return 1;
}

int isValidStationName(const char* name) {
    if (strlen(name) != 3) return 0;  // Check for exact length of 3 characters
    for (int i = 0; i < 3; i++) {
        if (!isupper(name[i])) return 0;  // Check each character if it's uppercase
    }
    return 1;
}

int isValidTime(int time) {
    return (time >= 0 && time <= 2359 && time % 100 < 60);
}

int isValidSeats(int seats) {
    return (seats > 0 && seats <= 25); // Ensures the number of seats is positive and up to 25
}


void membermenu() {
    int choice = 0;


    do {
        printf("\n---------------------------------------------\n");
        printf("                 MEMBER Menu                    ");
        printf("\n---------------------------------------------\n");

        printf("[1] Ticket Management\n[2] Profile Management\n[3] Exit\n");
        printf("Please enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            // If input is not an integer, clear input buffer
            while (getchar() != '\n');
            printf(RED "Invalid input. Please enter a number.\n" RESET);
            continue;
        }


        switch (choice) {
        case 1:
            ticketManagement();
            break;
        case 2:
            profilemanagement();
            break;
        case 3:
            memberM();
            break;
        default:
            printf(RED "Invalid selection. Please try again.\n" RESET);
            break;
        }
    } while (choice != 3);
}

void ticketManagement() {
    int choice = 0;


    do {
        printf("\n---------------------------------------------\n");
        printf("                 Ticket Management                    ");
        printf("\n---------------------------------------------\n");

        printf("[1] Search and Add Booking\n[2] View Booking Cart\n[3] Cancel Booking\n[4] Payment\n[5] Exit\n");
        printf("Please enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            // If input is not an integer, clear input buffer
            while (getchar() != '\n');
            printf(RED "Invalid input. Please enter a number.\n" RESET);
            continue;
        }


        switch (choice) {
        case 1:
            book();
            break;
        case 2:
            viewbook();
            break;
        case 3:
            cancelBooking();
            break;
        case 4:
            makePayment();
            break;
        case 5:
            membermenu();
            break;
        default:
            printf(RED "Invalid selection. Please try again.\n" RESET);
            break;
        }
    } while (choice != 4);
}


void makePayment() {

    FILE* ptr, * temp;
    char bookingId[100];
    char memberId[100], trainId[100], departure[100], arrival[100], seatNumber[5], tStatus[15];
    char id[100], choice;
    int departureTime, arrivalTime, numTickets, found = 0;
    float price[20];
    struct Schedule currentSchedule;

    ptr = fopen("booking.txt", "r");
    temp = fopen("temp.txt", "w");
    if (ptr == NULL) {
        printf(RED "Error opening booking file.\n" RESET);
        return;
    }


    printf("Enter which ticket your want to make payment(Booking ID): ");
    rewind(stdin);
    scanf("%s", id);

    while (fscanf(ptr, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%[^|]|%s\n",
        memberId, bookingId, trainId, departure, arrival, &departureTime, &arrivalTime, seatNumber, tStatus) != EOF) {
        if (strcmp(loginM.mId, memberId) == 0) {
            if (strcmp(bookingId, id) == 0) {
                if (strcmp(tStatus, "Pending") == 0) {
                    found = 1;

                    printf("Booked Tickets:\n");
                    printf("---------------------------------------------------------------------------------------------------\n");
                    printf("| Train ID | Booking ID | Departure | Arrival | Departure | Arrival | Seat Number | Ticket Status |\n");
                    printf("---------------------------------------------------------------------------------------------------\n");
                    printf("| %-9s| %-11s| %-9s| %-9s| %-10d| %-8d| %-12s| %-13s |\n", trainId, bookingId, departure, arrival, departureTime, arrivalTime, seatNumber, tStatus);
                    printf("---------------------------------------------------------------------------------------------------\n");

                    do {

                        printf("Are you sure to make payment ? (Y/N): ");
                        rewind(stdin);
                        scanf("%c", &choice);
                        if (toupper(choice) != 'Y' && toupper(choice) != 'N') {
                            printf(RED "Invalid, please try again (Y/N)\n" RESET);
                        }

                    } while (toupper(choice) != 'Y' && toupper(choice) != 'N');

                    if (toupper(choice) == 'Y') {
                        printf("========================================================================\n");
                        printf("|                             Ticket Receipt                            |\n");
                        printf("========================================================================\n");
                        printf("|Booking ID        : %-35s                                          |\n", bookingId);
                        printf("|Train ID          : %-35s                                          |\n", trainId);
                        printf("|Departure Station : %-35s                                          |\n", departure);
                        printf("|Arrival Station   : %-35s                                          |\n", arrival);
                        printf("|Departure Time    : %-35d                                          |\n", departureTime);
                        printf("|Arrival Time      : %-35d                                          |\n", arrivalTime);
                        printf("|Seat Number       : %-35s                                          |\n", seatNumber);
                        printf("=======================================================================\n");

                        strcpy(tStatus, "Booked");
                        system("pause");

                        fprintf(temp, "%s|%s|%s|%s|%s|%d|%d|%s|%s\n", memberId, bookingId, trainId, departure, arrival, departureTime, arrivalTime, seatNumber, tStatus);


                    }
                    else {
                        fprintf(temp, "%s|%s|%s|%s|%s|%d|%d|%s|%s\n", memberId, bookingId, trainId, departure, arrival, departureTime, arrivalTime, seatNumber, tStatus);

                    }

                }
                else {
                    fprintf(temp, "%s|%s|%s|%s|%s|%d|%d|%s|%s\n", memberId, bookingId, trainId, departure, arrival, departureTime, arrivalTime, seatNumber, tStatus);
                }

            }
            else {
                fprintf(temp, "%s|%s|%s|%s|%s|%d|%d|%s|%s\n", memberId, bookingId, trainId, departure, arrival, departureTime, arrivalTime, seatNumber, tStatus);

            }


        }
        else {
            fprintf(temp, "%s|%s|%s|%s|%s|%d|%d|%s|%s\n", memberId, bookingId, trainId, departure, arrival, departureTime, arrivalTime, seatNumber, tStatus);


        }
    }

    if (!found) {
        printf("No any booking need to make payment\n");
    }

    fclose(ptr);
    fclose(temp);
    remove("booking.txt");

    if (rename("temp.txt", "booking.txt") == 0) {
        printf(RED "File renamed successfully.\n" RESET);
    }
    else {
        printf(RED "Error renaming file.\n" RESET);
    }

}

void profilemanagement() {
    int choice = 0;


    do {
        printf("\n---------------------------------------------\n");
        printf("                 Profile Management                    ");
        printf("\n---------------------------------------------\n");

        printf("[1] Modify Profile\n[2] Display Profile\n[3] Exit\n");
        printf("Please enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            // If input is not an integer, clear input buffer
            while (getchar() != '\n');
            printf(RED "Invalid input. Please enter a number.\n" RESET);
            continue;
        }


        switch (choice) {
        case 1:
            modifyprofilemember();
            break;
        case 2:
            displayprofilemember();
            break;
        case 3:
            membermenu();
            break;
        default:
            printf("INVALID\n");
            break;
        }
    } while (choice != 3);
}

void modifyprofilemember() {
    FILE* file;
    struct Member tempMember;
    int choice;
    char newPassword[100], newIC[100], newContact[100];
    char newGender;

    file = fopen("memberRegister.bin", "rb+");

    if (file == NULL) {
        printf("Error: Unable to open file.\n");
        exit(-1);
    }

    int found = 0;

    // Display the current details before modification
    printf("Current Details:\n");
    printf("Password: %s\n", loginM.mpass);
    printf("Gender: %c\n", loginM.gender);
    printf("IC Number: %s\n", loginM.mIC);
    printf("Contact Number: %s\n", loginM.mContact);

    // Display menu for field selection
    printf("\nSelect field to modify:\n");
    printf("1. Password\n");
    printf("2. Gender\n");
    printf("3. IC (Identification Card) Number\n");
    printf("4. Contact Number\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    // Read through the file to find and update the member
    while (fread(&tempMember, sizeof(struct Member), 1, file)) {
        if (strcmp(tempMember.mId, loginM.mId) == 0) {
            found = 1;
            // Process user choice
            switch (choice) {
            case 1:
                printf("Enter new password: ");
                scanf("%s", newPassword);
                if (!isValidPassword(newPassword)) {
                    printf(RED "Invalid password format. Password must have at least 8 characters, including uppercase, lowercase, digit, and special character.\n");
                    fclose(file);
                    return;
                }
                strcpy(tempMember.mpass, newPassword);
                break;
            case 2:
                printf("Enter new gender (M/F): ");
                scanf(" %c", &newGender);
                if (!isValidGender(newGender)) {
                    printf(RED "Invalid gender. Please enter 'M' for male or 'F' for female.\n");
                    fclose(file);
                    return;
                }
                tempMember.gender = newGender;
                break;
            case 3:
                printf("Enter new IC number: ");
                scanf("%s", newIC);
                if (!isValidIC(newIC)) {
                    printf(RED "Invalid IC number format. IC number must be 12 digits long and contain only digits.\n");
                    fclose(file);
                    return;
                }
                strcpy(tempMember.mIC, newIC);
                break;
            case 4:
                printf("Enter new contact number: ");
                scanf("%s", newContact);
                if (!isValidPhoneNumber(newContact)) {
                    printf(RED "Invalid phone number format. Please enter in the format '601X-XXXXXXXXX'.\n\n" RESET);
                    fclose(file);
                    return;
                }

                strcpy(tempMember.mContact, newContact);
                break;
            default:
                printf("Invalid choice.\n");
                continue;
            }

            // Move file pointer to the beginning of the record
            long pos = ftell(file) - sizeof(struct Member);
            fseek(file, pos, SEEK_SET);

            // Write the modified record back to the file
            fwrite(&tempMember, sizeof(struct Member), 1, file);

            // Update global loginM to reflect changes
            loginM = tempMember;

            // Display updated details after modification
            printf("\nUpdated Details:\n");
            printf("Password: %s\n", tempMember.mpass);
            printf("Gender: %c\n", tempMember.gender);
            printf("IC Number: %s\n", tempMember.mIC);
            printf("Contact Number: %s\n", tempMember.mContact);
            printf("Profile updated successfully.\n");

            break;
        }
    }

    if (!found) {
        printf("Member ID not found in the record.\n");
    }

    // Close the file
    fclose(file);
}


void displayprofilemember() {
    int found = 0;

    printf("\n---------------------------------------------\n");
    printf("                 Display Profile                    ");
    printf("\n---------------------------------------------\n");

    printf("Member ID: %s\n", loginM.mId);
    printf("Name: %s\n", loginM.mName);
    printf("Gender: %c\n", loginM.gender);
    printf("IC: %s\n", loginM.mIC);
    printf("Contact Number: %s\n\n", loginM.mContact);
}




void memberDetails() {
    FILE* log;
    struct Member currentMember;

    log = fopen("memberRegister.bin", "rb"); // Open in binary mode
    if (log == NULL) {
        printf(RED "Error opening file.\n" RESET);
        exit(EXIT_FAILURE);
    }

    printf("\n\n=======================================\n\tMember Details \n=======================================\n");

    while (fread(&currentMember, sizeof(struct Member), 1, log)) {
        printf("Member ID: %s\n", currentMember.mId);
        printf("Name: %s\n", currentMember.mName);
        printf("Gender: %c\n", currentMember.gender);
        printf("IC: %s\n", currentMember.mIC);
        printf("Contact Number: %s\n\n", currentMember.mContact);
    }

    fclose(log);
}


void book() {
    FILE* ptr, * fp;
    FILE* fptr, * fptra;
    FILE* fptrSt, * tempSt;

    char book[100], seatNumber[5], bookingID[100];
    struct Schedule currentSchedule[100];
    struct Seat bookSeat;
    int found, fseat, num, numSt;
    int choice = 0;

    do {

        ptr = fopen("booking.txt", "a");
        fptr = fopen("scheduleTrain.txt", "r");

        fptrSt = fopen("seat.txt", "r");
        found = 0;
        fseat = 0;
        num = 0;
        numSt = 0;
        if (fptr == NULL) {
            printf(RED "Error opening schedule file.\n" RESET);
            return;
        }

        //temp seat file for update the available seat
        tempSt = fopen("temp.txt", "w");
        if (tempSt == NULL) {
            printf(RED "Error creating temporary file!\n" RESET);
            return;
        }

        rewind(fptr);
        rewind(fptrSt);
        displayTrainSchedule();
        printf("Enter the train ID you want to book: ");
        scanf("%s", book);


        //read number of trainSchedule
        while (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%d|%d|%d|%d|%f|\n",
            currentSchedule[num].tId, currentSchedule[num].tDs, currentSchedule[num].tAs,
            &currentSchedule[num].tDtime, &currentSchedule[num].tAtime, &currentSchedule[num].tAseat, &currentSchedule[num].totalSeat, &currentSchedule[num].eachPrice) != EOF) {

            num++;
        }


        for (int j = 0; j < num; j++) {
            if (strcmp(currentSchedule[j].tId, book) == 0) {
                found = 1;
                if (currentSchedule[j].tAseat > 0) {
                    //read the available seat of train
                    while (fscanf(fptrSt, "%4[^|]|%199[^\n]\n", bookSeat.trainID, bookSeat.seat) == 2) {

                        if (strcmp(bookSeat.trainID, book) == 0) {
                            //print all the available seat of a specific trainID
                            printf("\n%s\n\n", bookSeat.seat);

                            printf("Enter the seat number : ");
                            rewind(stdin);
                            scanf("%s", seatNumber);

                            fprintf(tempSt, "%s|", bookSeat.trainID);
                            //to read each of the seat 
                            char* token = strtok(bookSeat.seat, " ");
                            while (token != NULL) {
                                if (strcmp(token, seatNumber) == 0) {
                                    fseat = 1;
                                    printf("Booking successful!\n");

                                    //available seat will be minus 1
                                    currentSchedule[j].tAseat = currentSchedule[j].tAseat - 1;

                                    //update the scheduleTrain.txt
                                    fptra = fopen("scheduleTrain.txt", "w");

                                    for (int i = 0; i < num; i++) {
                                        fprintf(fptra, "%s|%s|%s|%d|%d|%d|%d|%.2f|\n",
                                            currentSchedule[i].tId, currentSchedule[i].tDs, currentSchedule[i].tAs,
                                            currentSchedule[i].tDtime, currentSchedule[i].tAtime, currentSchedule[i].tAseat, currentSchedule[i].totalSeat, currentSchedule[i].eachPrice);
                                    }

                                    fclose(fptra);

                                    int lastID = 1000;
                                    FILE* fp = fopen("bookingId.txt", "r");

                                    if (fp != NULL) {
                                        fscanf(fp, "%d", &lastID);
                                        fclose(fp);
                                    }

                                    int newID = lastID + 1;

                                    fp = fopen("bookingId.txt", "w");

                                    if (fp != NULL) {
                                        fprintf(fp, "%d", newID);
                                        fclose(fp);
                                    }
                                    else {
                                        printf(RED "Invalid.\n" RESET);
                                    }

                                    sprintf(bookingID, "B%d", newID);

                                    fprintf(ptr, "%s|%s|%s|%s|%s|%d|%d|%s|%s\n",
                                        loginM.mId,
                                        bookingID,
                                        currentSchedule[j].tId,           // Train ID
                                        currentSchedule[j].tDs,           // Departure Station
                                        currentSchedule[j].tAs,           // Arrival Station
                                        currentSchedule[j].tDtime,        // Departure Time
                                        currentSchedule[j].tAtime,        // Arrival Time
                                        seatNumber,                    // Seat Number
                                        "Pending",
                                        1);
                                    // Number of Tickets (assuming it's always 1 for now)
                                    ; // Assuming "MemberID" is the logged-in member's ID


                                }
                                else {
                                    fprintf(tempSt, "%s ", token);
                                }

                                token = strtok(NULL, " ");
                            }

                            if (token == NULL) {
                                fprintf(tempSt, "\n");
                            }

                        }
                        else {
                            fprintf(tempSt, "%s|%s\n", bookSeat.trainID, bookSeat.seat);
                        }


                        numSt++;
                    }



                    if (fseat != 1) {
                        printf(RED "Seat %s is not available for booking.\n" RESET, seatNumber);
                    }

                }
                else {
                    printf(RED "Sorry, this train is fully booked.\n" RESET);
                }
                break;
            }
        }
        if (!found) {
            printf(RED "Train with ID %s not found.\n" RESET, book);
        }


        fclose(fptrSt);
        fclose(fptr);
        fclose(ptr);
        fclose(tempSt);
        remove("seat.txt");
        if (rename("temp.txt", "seat.txt") == 0) {
            printf(RED "" RESET);
        }
        else {
            printf(RED "" RESET);
        }
        printf("Do you want to book another ticket? (1 for Yes, 0 for No): ");
        scanf("%d", &choice);
    } while (choice != 0);
}

void staffProfile() {
    printf("NOT DONE Yet\n");
}
void memberProfile() {
    printf("NOT DONE Yet\n");
}

void viewbook() {
    FILE* ptr;
    char memberId[100], trainId[100], bookingId[100], departure[100], arrival[100], seatNumber[5], tStatus[15];
    int departureTime, arrivalTime, numTickets;

    ptr = fopen("booking.txt", "r");
    if (ptr == NULL) {
        printf(RED "Error opening booking file.\n" RESET);
        return;
    }

    printf("Booked Tickets:\n");
    printf("---------------------------------------------------------------------------------------------------\n");
    printf("| Train ID | Booking ID | Departure | Arrival | Departure Time | Arrival Time | Seat Number | Ticket Status |\n");
    printf("---------------------------------------------------------------------------------------------------\n");

    while (fscanf(ptr, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%[^|]|%s\n",
        memberId, bookingId, trainId, departure, arrival, &departureTime, &arrivalTime, seatNumber, tStatus) != EOF) {

        // compare to the global variable loginM
        if (strcmp(loginM.mId, memberId) == 0) {
            // Read the number of tickets (assuming it's always 1 for now)
            fscanf(ptr, "|%d|\n", &numTickets);
            printf("| %-9s| %-11s| %-9s| %-9s| %-10d| %-8d| %-12s| %-13s |\n", trainId, bookingId, departure, arrival, departureTime, arrivalTime, seatNumber, tStatus);
        }

    }

    fclose(ptr);
}


//////////////////////////////////////////////////////////function for restore the seat
int restoreSeat(char* seatNum, char* trainId) {

    FILE* fptrSt, * temp, * fptra, * fptrT;
    struct Seat aSeat;
    struct Schedule currentSchedule[100];
    char letter, cancelLetter;
    int number, cancelNumber, restore = 0, num = 0, numRecords = 0;


    fptrSt = fopen("seat.txt", "r");
    fptrT = fopen("scheduleTrain.txt", "r");

    if (fptrSt == NULL || fptrT == NULL) {
        printf(RED "Error opening schedule file.\n" RESET);
        return;
    }

    temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf(RED "Error creating temporary file!\n" RESET);
        return;
    }


    while (fscanf(fptrT, "%[^|]|%[^|]|%[^|]|%d|%d|%d|%d|%f|\n",
        currentSchedule[num].tId, currentSchedule[num].tDs, currentSchedule[num].tAs,
        &currentSchedule[num].tDtime, &currentSchedule[num].tAtime, &currentSchedule[num].tAseat, &currentSchedule[num].totalSeat, &currentSchedule[num].eachPrice) != EOF) {

        num++;
    }

    fclose(fptrT);


    while (fscanf(fptrSt, "%4[^|]|%199[^\n]\n", aSeat.trainID, aSeat.seat) == 2) {
        if (strcmp(aSeat.trainID, trainId) == 0) {

            fprintf(temp, "%s|", aSeat.trainID);

            char* token = strtok(aSeat.seat, " ");
            while (token != NULL) {
                sscanf(token, "%c%02d", &letter, &number);
                sscanf(seatNum, "%c%02d", &cancelLetter, &cancelNumber);

                if (cancelLetter == letter) {
                    if (restore != 1) {
                        if (cancelNumber < number) {
                            fprintf(temp, "%s ", seatNum);
                            fprintf(temp, "%s ", token);
                            restore = 1;
                            currentSchedule[numRecords].tAseat = currentSchedule[numRecords].tAseat + 1;

                        }
                        else {
                            fprintf(temp, "%s ", token);
                        }
                    }
                    else {
                        fprintf(temp, "%s ", token);
                    }

                }
                else {
                    if (restore == 1) {
                        fprintf(temp, "%s ", token);
                    }
                    else {
                        if (cancelLetter < letter) {
                            fprintf(temp, "%s ", seatNum);
                            fprintf(temp, "%s ", token);
                            restore = 1;
                            currentSchedule[numRecords].tAseat = currentSchedule[numRecords].tAseat + 1;
                        }
                        else {
                            fprintf(temp, "%s ", token);
                        }
                    }
                }

                token = strtok(NULL, " ");
            }

            if (token == NULL) {
                fprintf(temp, "\n");
            }

        }
        else {
            fprintf(temp, "%s|%s\n", aSeat.trainID, aSeat.seat);
        }
        numRecords++;
    }
    fclose(fptrSt);

    //update the scheduleTrain.txt
    fptra = fopen("scheduleTrain.txt", "w");
    if (fptra == NULL) {
        printf(RED "Error opening scheduleTrain.txt for writing.\n" RESET);
        return 1;
    }

    for (int i = 0; i < num; i++) {
        fprintf(fptra, "%s|%s|%s|%d|%d|%d|%d|%.2f|\n",
            currentSchedule[i].tId, currentSchedule[i].tDs, currentSchedule[i].tAs,
            currentSchedule[i].tDtime, currentSchedule[i].tAtime, currentSchedule[i].tAseat, currentSchedule[i].totalSeat, currentSchedule[i].eachPrice);
    }

    fclose(fptra);
    fclose(temp);
    if (remove("seat.txt") == 0) {
        if (rename("temp.txt", "seat.txt") == 0) {
            printf(GREEN "File renamed successfully.\n" RESET);
        }
        else {
            printf(RED "Error renaming file.\n" RESET);
            return 1;
        }
    }
    else {
        printf(RED "Error removing seat.txt.\n" RESET);
        return 1;
    }

}

void cancelBooking() {
    FILE* fptr, * temp;
    struct Booking currentBooking[1000];
    char memberId[100], trainId[100], bookingId[100], departure[100], arrival[100], seatNumber[5], tStatus[15], cancel[100], ans, choice, cancelSeatNum[100], cancelTrain[100];
    int departureTime, arrivalTime, numTickets;
    int i = 0, a, foundM = 0, foundB = 0, cancelBooking = 0, succful = 0;

    fptr = fopen("booking.txt", "r");
    if (fptr == NULL) {
        printf(RED "Error opening booking file.\n" RESET);
        return;
    }


    temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf(RED "Error opening booking file.\n" RESET);
        return;
    }

    viewbook();
    printf("\nEnter your booking ID to cancel: ");
    rewind(stdin);
    scanf("%s", cancel);

    while (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%[^|]|%s\n",
        memberId, bookingId, trainId, departure, arrival, &departureTime, &arrivalTime, seatNumber, tStatus) != EOF) {
        if (strcmp(loginM.mId, memberId) == 0) {
            foundM = 1;
            if (strcmp(bookingId, cancel) == 0) {
                foundB = 1;
                printf("Booked Tickets:\n");
                printf("---------------------------------------------------------------------------------------------------\n");
                printf("| Train ID | Booking ID | Departure | Arrival | Departure | Arrival | Seat Number | Ticket Status |\n");
                printf("---------------------------------------------------------------------------------------------------\n");
                printf("| %-9s| %-11s| %-9s| %-9s| %-10d| %-8d| %-12s| %-13s |\n", trainId, bookingId, departure, arrival, departureTime, arrivalTime, seatNumber, tStatus);
                printf("---------------------------------------------------------------------------------------------------\n");


                do {

                    printf("Are you sure to cancel the booking? (Y/N): ");
                    rewind(stdin);
                    scanf("%c", &choice);
                    if (toupper(choice) != 'Y' && toupper(choice) != 'N') {
                        printf(RED "Invalid, please try again (Y/N)\n" RESET);
                    }

                } while (toupper(choice) != 'Y' && toupper(choice) != 'N');


                if (toupper(choice) == 'Y') {

                    strcpy(cancelSeatNum, seatNumber);
                    strcpy(cancelTrain, trainId);

                    cancelBooking = 1;
                    printf(GREEN "Cancel Booking Successful.\n" RESET);


                }
                else {
                    fprintf(temp, "%s|%s|%s|%s|%s|%d|%d|%s|%s\n", memberId, bookingId, trainId, departure, arrival, departureTime, arrivalTime, seatNumber, tStatus);

                }

            }
            else {
                fprintf(temp, "%s|%s|%s|%s|%s|%d|%d|%s|%s\n", memberId, bookingId, trainId, departure, arrival, departureTime, arrivalTime, seatNumber, tStatus);
            }

        }
        else {
            fprintf(temp, "%s|%s|%s|%s|%s|%d|%d|%s|%s\n", memberId, bookingId, trainId, departure, arrival, departureTime, arrivalTime, seatNumber, tStatus);

        }

        i++;
    }

    fclose(fptr);
    fclose(temp);
    remove("booking.txt");
    if (rename("temp.txt", "booking.txt") == 0) {
        printf(GREEN "" RESET);
    }
    else {
        printf(RED "" RESET);
    }


    if (!foundM) {
        printf(RED "No any booking for you\n" RESET);
    }

    if (!foundB) {
        printf(RED "The booking ID is not found\n" RESET);
    }


    if (cancelBooking) {
        succful = restoreSeat(cancelSeatNum, cancelTrain);

    }





}
void report() {
    FILE* file;
    char memberId[100], bookingId[100], trainId[100], departure[100], arrival[100], seatNumber[5], tStatus[15];
    int departureTime, arrivalTime;

    file = fopen("booking.txt", "r");
    if (file == NULL) {
        printf(RED "Error opening booking file.\n" RESET);
        return;
    }

    printf("\n---------------------------------------------------------------------------------------------------------------\n");
    printf("                                                 Booking Report(PAYMENT)                     ");
    printf("\n---------------------------------------------------------------------------------------------------------------\n");
    printf("| %-10s | %-10s | %-10s | %-15s | %-15s | %-8s | %-8s | %-10s |\n", "Member ID", "Booking ID", "Train ID", "Departure", "Arrival", "Dep Time", "Arr Time", "Status");
    printf("---------------------------------------------------------------------------------------------------------------\n");

    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%[^|]|%s\n",
        memberId, bookingId, trainId, departure, arrival, &departureTime, &arrivalTime, seatNumber, tStatus) != EOF) {
        if (strcmp(tStatus, "Pending") != 0) { // Check if status is not "Pending"
            printf("| %-10s | %-10s | %-10s | %-15s | %-15s | %02d:%02d    | %02d:%02d    | %-10s |\n", memberId, bookingId, trainId, departure, arrival, departureTime / 100, departureTime % 100, arrivalTime / 100, arrivalTime % 100, tStatus);
        }
    }

    fclose(file);
    printf("---------------------------------------------------------------------------------------------------------------\n");
}



void report2() {
    FILE* file;
    char memberId[100], bookingId[100], trainId[100], departure[100], arrival[100], seatNumber[5], tStatus[15];
    int departureTime, arrivalTime;

    file = fopen("booking.txt", "r");
    if (file == NULL) {
        printf(RED "Error opening booking file.\n" RESET);
        return;
    }

    printf("\n---------------------------------------------------------------------------------------------------------------\n");
    printf("                                                 Booking Report(PAYMENT)                     ");
    printf("\n---------------------------------------------------------------------------------------------------------------\n");
    printf("| %-10s | %-10s | %-10s | %-15s | %-15s | %-8s | %-8s | %-10s |\n", "Member ID", "Booking ID", "Train ID", "Departure", "Arrival", "Dep Time", "Arr Time", "Status");
    printf("---------------------------------------------------------------------------------------------------------------\n");

    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%[^|]|%s\n",
        memberId, bookingId, trainId, departure, arrival, &departureTime, &arrivalTime, seatNumber, tStatus) != EOF) {
        if (strcmp(tStatus, "Pending") == 0) {
            printf("| %-10s | %-10s | %-10s | %-15s | %-15s | %02d:%02d    | %02d:%02d    | %-10s |\n", memberId, bookingId, trainId, departure, arrival, departureTime / 100, departureTime % 100, arrivalTime / 100, arrivalTime % 100, tStatus);
        }
    }

    fclose(file);
    printf("---------------------------------------------------------------------------------------------------------------\n");
}
// Function to display current time
void showTime() {
    time_t currentTime;
    time(&currentTime);
    printf("Current time: %s", ctime(&currentTime));
}