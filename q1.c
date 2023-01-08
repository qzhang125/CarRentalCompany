/*
Assignment 2 Q1
Name: Qiuyu Zhang
Student Number: 1244549
Date: Oct 22, 2022
*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

// Cars informations
struct Car{
    char _name[20];
    int _mileage;
    int _expectedReturnDate;
    char _plateNumber[20]; 
    struct Car *nextCar;
};

void pushNewCar(struct Car** head, char name[20], int mileage, char plateNumber[20]);
void pushCarsToRented(struct Car** head, char name[20], int mileage, char plateNumber[20], int returnDate);
struct Car* createNewCar(char name[20], int mileage, char plateNumber[20], int);
int isEmpty(struct Car* head);
void createInventoryForAvailable(struct Car** head);
void addCarsToFile(char name[20], int mileage, char plateNumber[20]);
int isEmpty(struct Car* head);
void display(struct Car* headForAvailable, struct Car* headForRented, struct Car* headForRepair);
void menu();
void createInventoryForRented(struct Car**head);
void createInventoryForRepair(struct Car**head);
void swap(struct Car* first, struct Car* second);
void sortByMileage(struct Car* head);
void sortByReturnDate();
void returnedCarToAvailable(struct Car** headForRented, struct Car** headForAvailable, char plateNumber[20], unsigned int newMileage);
void updateFile(struct Car* headForAvailable, struct Car* headForRented, struct Car* headForRepair, int identifier);
void sendCarToRepair(struct Car** headForRented, struct Car** headForRepair, char plateNumber[20], unsigned int newMileage);
void repairedCarToAvailable(struct Car** headForRepair, struct Car** headForAvailable, char plateNumber[20]);
void rentFirstCar(struct Car** headForAvailable, struct Car** headForRented, int returnDate);
//Todo


//Read car info from file and insert to Available-for-rent list
void createInventoryForAvailable(struct Car** head){
  
  FILE* file = fopen("availableForRent.txt", "r");
  if(file == NULL){
    printf("The file does not exist");
    exit(1);
  }
  char name[20];
  int mileage;
  char plateNumber[20];
  while(fscanf(file, "%s %d %s ", name, &mileage, plateNumber) > 0){
    //push front
    pushNewCar(head, name, mileage, plateNumber);
  }
  fclose(file);
}

//Add cars to the file of available-for-rent list
void addCarsToFile(char name[20], int mileage, char plateNumber[20]){
  FILE* file  = fopen("availableForRent.txt", "a");
  if(file ==NULL){
    printf("ERROR: Can't find the file.\n");
  }
  fprintf(file, " %s %d %s ", name, mileage, plateNumber);
  fclose(file);
}

//Read rented cars from Rented.txt and push to linked list
void createInventoryForRented(struct Car**head){
  FILE* file = fopen("Rented.txt", "r");
  if(file == NULL){
    printf("The file does not exist");
    exit(1);
  }
  char name[20];
  int mileage;
  char plateNumber[20];
  int returnDate;
  while(fscanf(file, "%s %d %s %d", name, &mileage, plateNumber, &returnDate) > 0){
    //push front
    pushCarsToRented(head, name, mileage, plateNumber, returnDate);
  }
  fclose(file);
}

//Add cars to the file of Rented list
void addCarsToRentedFile(char name[20], int mileage, char plateNumber[20], int returnDate){
  FILE* file  = fopen("Rented.txt", "a");
  if(file ==NULL){
    printf("ERROR: Can't find the file.\n");
  }
  fprintf(file, " %s %d %s %d", name, mileage, plateNumber, returnDate);
  fclose(file);
}

//Read repair cars from disk file
void createInventoryForRepair(struct Car**head){
  FILE* file = fopen("inRepair.txt", "r");
  if(file == NULL){
    printf("The file does not exist");
    exit(1);
  }
  char name[20];
  int mileage;
  char plateNumber[20];
  while(fscanf(file, "%s %d %s", name, &mileage, plateNumber) > 0){
    //push front
    pushNewCar(head, name, mileage, plateNumber);
  }
  fclose(file);
}

//Add cars to the file of repair list
void addCarsToRepairFile(char name[20], int mileage, char plateNumber[20]){
  FILE* file  = fopen("inRepair.txt", "a");
  if(file ==NULL){
    printf("ERROR: Can't find the file.\n");
  }
  fprintf(file, " %s %d %s ", name, mileage, plateNumber);
  fclose(file);
}

//Create a New Car for Available-for-rent list
struct Car* createNewCar(char name[20], int mileage, char plateNumber[20], int returnDate){
  struct Car* Cars = (struct Car*)malloc(sizeof(struct Car));
  strcpy(Cars->_name, name);
  Cars->_mileage = mileage;
  strcpy(Cars->_plateNumber, plateNumber);
  Cars->_expectedReturnDate = returnDate;
  Cars->nextCar = NULL;
  return Cars;
}

//Push a new car to Available-for-rent list
void pushNewCar(struct Car** head, char name[20], int mileage, char plateNumber[20]){
  //initialize a return date
  int returnDate = 000000;
  struct Car* newCar = createNewCar(name, mileage, plateNumber, returnDate);
  newCar->nextCar = *head;
  *head = newCar;
}

//Push car to the rented list
void pushCarsToRented(struct Car** head, char name[20], int mileage, char plateNumber[20], int returnDate){
  struct Car* rentedCar = createNewCar(name, mileage, plateNumber, returnDate);
  rentedCar->nextCar = *head;
  *head = rentedCar;
}

//Check if the linked list is empty
int isEmpty(struct Car* head) {
    return !head;
}

//Return cars from rented list to available for rent list
void returnedCarToAvailable(struct Car** headForRented, struct Car** headForAvailable, char plateNumber[20], unsigned int newMileage){
  if(isEmpty(*headForRented)){
    printf("The rented list is empty, rent a car before return.\n");
    return;
  }
  
  struct Car* curr = *headForRented, *prev;
  //Check the first node 
  if(curr != NULL && strcmp(curr->_plateNumber,plateNumber) == 0){
    printf("current plateNumber is %s\n\n", curr->_plateNumber);
    *headForRented = curr->nextCar;
    printf("Car Returned!\n");

    //Transfer current car to Available-For-Rent list
    //Retrieve the new mileage from the customer
    curr->_mileage += newMileage;
  
    //Move the rented node to the Availble-For-Rent list
    pushNewCar(headForAvailable, curr->_name, curr->_mileage, curr->_plateNumber);
    printf("ACTION: %s is transferred from Rented list to Availble-For-Rent list.\n\n", curr->_plateNumber);
    //Delete the current car from rented list
    free(curr);
    
    return;
  }
  
  //Looping to check each of the node
  while(curr != NULL && strcmp(curr->_plateNumber,plateNumber) != 0){
    prev = curr;
    curr = curr->nextCar;
  }

  //If the car does not exist in rented list
  if(curr == NULL){
    printf("Cannot find the car in the rented list, please check the plate number again.\n\n");
    return;
  }

  //Retrieve the new mileage from the customer
  curr->_mileage += newMileage;
  
  //Move the rented node to the Availble-For-Rent list
  pushNewCar(headForAvailable, curr->_name, curr->_mileage, curr->_plateNumber);

  //Print message
  printf("Car Returned!\n");
  printf("ACTION: %s is transferred from Rented list to Availble-For-Rent list.\n\n", curr->_plateNumber);
  //Unlink the node 
  prev->nextCar = curr->nextCar;

  //Delete the node
  free(curr);
}

//Return cars from rented list to repair list
void sendCarToRepair(struct Car** headForRented, struct Car** headForRepair, char plateNumber[20], unsigned int newMileage){
  if(isEmpty(*headForRented)){
    printf("The rented list is empty, rent a car before return.\n");
    return;
  }
  
  struct Car* curr = *headForRented, *prev;
  if(curr != NULL && strcmp(curr->_plateNumber,plateNumber) == 0){
    //printf("current plateNumber is %s\n\n", curr->_plateNumber);
    *headForRented = curr->nextCar;
    //printf("Car founded!\n");

    //Transfer current car to repair list
    //Retrieve the new mileage from the customer
    curr->_mileage += newMileage;
  
    //Move the rented node to the Repair list
    pushNewCar(headForRepair, curr->_name, curr->_mileage, curr->_plateNumber);
    printf("ACTION: %s is transferred from Rented list to Repair list.\n\n", curr->_plateNumber);
    //Delete the current car from rented list
    free(curr);
    
    return;
  }
  
  //Looping to check each of the node
  while(curr != NULL && strcmp(curr->_plateNumber,plateNumber) != 0){
    prev = curr;
    curr = curr->nextCar;
  }

  //If the car does not exist in rented list
  if(curr == NULL){
    printf("Cannot find the car in the rented list, please check the plate number again.\n\n");
    return;
  }

  //Retrieve the new mileage from the customer
  curr->_mileage += newMileage;
  
  //Move the rented node to the Repair list
  pushNewCar(headForRepair, curr->_name, curr->_mileage, curr->_plateNumber);

  //Print message
  printf("ACTION: %s is transferred from Rented list to Availble-For-Rent list.\n\n", curr->_plateNumber);
  //Unlink the node 
  prev->nextCar = curr->nextCar;

  //Delete the node
  free(curr);
}

//Send car from repair list to available list
void repairedCarToAvailable(struct Car** headForRepair, struct Car** headForAvailable, char plateNumber[20]){
  if(isEmpty(*headForRepair)){
    printf("The Repair list is empty, rent a car before return.\n");
    return;
  }
  struct Car* curr = *headForRepair, *prev;
  //Check the first node 
  if(curr != NULL && strcmp(curr->_plateNumber,plateNumber) == 0){
    *headForRepair = curr->nextCar;
    
    //Move the repair car to the Availble-For-Rent list
    pushNewCar(headForAvailable, curr->_name, curr->_mileage, curr->_plateNumber);
    printf("ACTION: %s is transferred from Repair list to Availble-For-Rent list.\n\n", curr->_plateNumber);
    
    //Delete the current car from rented list
    free(curr); 
    return;
  }
  
  //Looping to check each of the node
  while(curr != NULL && strcmp(curr->_plateNumber,plateNumber) != 0){
    prev = curr;
    curr = curr->nextCar;
  }

  //If the car does not exist in repair list
  if(curr == NULL){
    printf("Cannot find the car in the repair list, please check the plate number again.\n\n");
    return;
  }
  
  //Move the repaired car to the Availble-For-Rent list
  pushNewCar(headForAvailable, curr->_name, curr->_mileage, curr->_plateNumber);

  //Print message
  printf("ACTION: %s is transferred from Repair list to Availble-For-Rent list.\n\n", curr->_plateNumber);
  
  //Unlink the node 
  prev->nextCar = curr->nextCar;
  //Delete the node
  free(curr);

}

//Rent the first car from available-for-rent list
void rentFirstCar(struct Car** headForAvailable, struct Car** headForRented, int returnDate){
  if(isEmpty(*headForAvailable)){
    printf("The available-for-rent list is empty.\n");
    return;
  }
  //Create a temp to point to the head of the available list
  struct Car* top = *headForAvailable;
  //Make the head to point to the second car in the available list
  *headForAvailable = (*headForAvailable)->nextCar;
  //Add the first car to the rented list
  pushCarsToRented(headForRented, top->_name, top->_mileage, top->_plateNumber, returnDate);
  //print msg
  printf("ACTION: %s is rented list to Availble-For-Rent list.\n\n", top->_plateNumber);
  //Delete top from available list
  free(top);
}

//Update the 3 files
void updateFile(struct Car* headForAvailable, struct Car* headForRented, struct Car* headForRepair, int identifier){
  struct Car* curr = NULL;
  
  //update Available-For-Rent file
  if(identifier == 1){
    FILE* availableFile = fopen("availableForRent.txt", "w");
    if(availableFile == NULL){
      printf("ERROR: Can't find the availableForRent.txt file");
      return;
    }
    for(curr = headForAvailable; curr != NULL; curr = curr->nextCar){
      fprintf(availableFile, " %s %d %s ", curr->_name, curr->_mileage, curr->_plateNumber);
    }
    fclose(availableFile);
  }
  
  //Update the rented list
  if(identifier == 2){
    FILE* rentedFile = fopen("Rented.txt", "w");
    if(rentedFile == NULL){
      printf("ERROR: Can't find the Rented.txt file");
      return;
    }
    for(curr = headForRented; curr != NULL; curr = curr->nextCar){
      fprintf(rentedFile, " %s %d %s %d", curr->_name, curr->_mileage, curr->_plateNumber, curr->_expectedReturnDate);
    }
    fclose(rentedFile);
  }
  
  //Update the in repair file
  if(identifier == 3){
    FILE* repairFile = fopen("inRepair.txt", "w");
    if(repairFile == NULL){
      printf("ERROR: Can't find the inRepair.txt file");
      return;
    }
    for(curr = headForRepair; curr != NULL; curr = curr->nextCar){
      fprintf(repairFile, " %s %d %s ", curr->_name, curr->_mileage, curr->_plateNumber);
    }
    fclose(repairFile);
  }

  free(curr);
}

//Calculate the charge for each return car
int chargeCalculator(int newMileage){
  double charge = 80.00;
  if(newMileage < 0){
    printf("Wrong mileage.\n");
    exit(1);
  }
  if(newMileage <= 200){
    return charge;
  }
  if(newMileage > 200){
    charge += (newMileage - 200) * 0.15;
  }
  return charge;
}

//Display all the lists
void display(struct Car* headForAvailable, struct Car* headForRented, struct Car* headForRepair) {
  if(isEmpty(headForAvailable)){
    printf("\nAvailable-For-Rent list is empty\n");
  }else{
    //Print the available-for-rent list
    printf("--------------Available-For-Rent----------------\n");
    printf("Car Name ----------- Mileage ----------Plate Number\n");
    struct Car *temp = headForAvailable;
    while (temp->nextCar != NULL) {
        printf("  %s                %d               %s\n", temp->_name, temp->_mileage, temp->_plateNumber);
        temp = temp->nextCar;
    }
    printf("  %s                  %d                %s\n", temp->_name, temp->_mileage, temp->_plateNumber);
  }
  
  if(isEmpty(headForRented)){
    printf("\nRented list is empty\n");
  }else{
    //Print the rented car list
    printf("\n\n\n");
    printf("--------------Rented----------------\n");
    printf("Car Name ----------- Mileage ----------Plate Number----------Expected Return Date\n");
    struct Car* temp1 = headForRented;
    while(temp1->nextCar != NULL){
      printf("  %s                %d               %s             %d\n", temp1->_name, temp1->_mileage, temp1->_plateNumber, temp1->_expectedReturnDate);
      temp1 = temp1->nextCar;
    }
    printf("  %s                %d               %s             %d\n", temp1->_name, temp1->_mileage, temp1->_plateNumber, temp1->_expectedReturnDate);
    printf("\n\n\n");
  }
  
  if(isEmpty(headForRepair)){
    printf("\nRepair list is empty\n");
  }else{
    //Print the repair car list
    printf("--------------In Repair----------------\n");
    printf("Car Name ----------- Mileage ----------Plate Number\n");
    struct Car *temp2 = headForRepair;
    while (temp2->nextCar != NULL) {
        printf("  %s                %d               %s\n", temp2->_name, temp2->_mileage, temp2->_plateNumber);
        temp2 = temp2->nextCar;
    }
    printf("  %s                  %d                %s\n", temp2->_name, temp2->_mileage, temp2->_plateNumber);
    printf("\n\n\n");
  }

}

void menu(){
  //Display the message for the menu
  printf("1. Add a new car to the available-for-rent list\n");
  printf("2. Add a returned car to the available-for-rent list\n");
  printf("3. Add a returned car to the repair list\n");
  printf("4. Transfer a car from the repair list to the available-for-rent list\n");
  printf("5. Rent the first available car\n");
  printf("6. Print all the lists\n");
  printf("7. Quit\n");
}

//Swap two node
void swap(struct Car* first, struct Car* second){
  char tempName[20];
  int tempMileage;
  char tempPlateNumber[20];
  int tempReturnDate;

  //swap
  strcpy(tempName,first->_name);
  strcpy(tempPlateNumber, first->_plateNumber);
  tempMileage = first->_mileage;
  tempReturnDate = first->_expectedReturnDate;

  strcpy(first->_name,second->_name);
  strcpy(first->_plateNumber, second->_plateNumber);
  first->_mileage = second->_mileage;
  first->_expectedReturnDate = second->_expectedReturnDate;

  strcpy(second->_name,tempName);
  strcpy(second->_plateNumber, tempPlateNumber);
  second->_mileage = tempMileage;
  second->_expectedReturnDate = tempReturnDate;
}

//Bubble sort the linked list by mileage
void sortByMileage(struct Car* head){
  int sorted;
  struct Car* first; 
  struct Car* last = NULL; 
  //Checking for empty list
  if (isEmpty(head)){
    printf("The sorting list is empty");
    return; 
  }
  do{ 
    sorted = 0; 
    first = head; 
    while (first->nextCar != last){ 
      if (first->_mileage > first->nextCar->_mileage){ 
        swap(first, first->nextCar); 
        sorted = 1; 
      } 
      first = first->nextCar; 
    } 
    last = first; 
  } 
  while (sorted); 
}

//Bubble sort the linked list by expected return date 
void sortByReturnDate(struct Car* head){
  int sorted;
  struct Car* first; 
  struct Car* last = NULL; 
  //Checking for empty list
  if (isEmpty(head)){
    printf("The sorting list is empty");
    return; 
  }
  do{ 
    sorted = 0; 
    first = head; 
    while (first->nextCar != last){ 
      if (first->_expectedReturnDate > first->nextCar->_expectedReturnDate){ 
        swap(first, first->nextCar); 
        sorted = 1; 
      } 
      first = first->nextCar; 
    } 
    last = first; 
  } 
  while (sorted); 
}

//Free the three lists
void freeList(struct Car* head1, struct Car* head2, struct Car* head3){
  struct Car* temp1;
  struct Car* temp2;
  struct Car* temp3;
  //Looping until the list is deleted
  while(head1 != NULL){
    temp1 = head1;
    head1 = head1->nextCar;
    free(temp1);
  }
  while(head2 != NULL){
    temp2 = head2;
    head2 = head2->nextCar;
    free(temp2);
  }
  while(head3 != NULL){
    temp3 = head3;
    head3 = head3->nextCar;
    free(temp3);
  }
  printf("ACTION: Lists are deleted.\n\n");
}

int main() {
  int ch, returnDate = 000000, mileage = 0;
  char carName[20];
  char plateNumber[20];
  double charge = 0.0;
  static double totalCharge = 0.0;

  //Create lists for available-for-rent, rented, and in repair
  struct Car* headForAvailable = NULL;
  struct Car* headForRented = NULL;
  struct Car* headForRepair = NULL;

  //Load the inventory or Available-for-Rent list
  createInventoryForAvailable(&headForAvailable);
  createInventoryForRented(&headForRented);
  createInventoryForRepair(&headForRepair);
  

  //Menu
  while(1) {
    menu();
    printf("Choose Your Option: \n");
    scanf("%d", &ch);
    switch(ch) {
      case 1:
        printf("ACTION: Add new car to Available-For-Rent list\n\n");
        printf("Enter the car name: ");
        scanf("%s", carName);
        printf("Enter the car milage(km): ");
        scanf("%d", &mileage);
        printf("Enter the plate number: ");
        scanf("%s", plateNumber);
        
        //Add a new car to availble for rent list
        pushNewCar(&headForAvailable, carName, mileage, plateNumber);

        //Write the new car to the file
        addCarsToFile(carName, mileage, plateNumber);
        
        //Sort the linked list by mileage
        sortByMileage(headForAvailable);
        //update the available file, 1 represents Available file
        updateFile(headForAvailable, headForRented, headForRepair, 1);
        printf("Car pushed.\n");
        break;

      case 2: 
        //Return cars from rented list to available list
        printf("ACTION: Return a rented car to the Available-For-Rent list\n\n");
        printf("Enter the plate number of the rented car: ");
        scanf("%s", plateNumber);
        printf("Enter the new mileage added to the rented car this time(km): ");
        scanf("%d", &mileage);
        returnedCarToAvailable(&headForRented, &headForAvailable, plateNumber, mileage);
        
        //sort the linked list
        sortByMileage(headForAvailable);
        sortByReturnDate(headForRented);
        //Update the available for rent file
        updateFile(headForAvailable, headForRented, headForRepair, 1);
        //update the rented file
        updateFile(headForAvailable, headForRented, headForRepair, 2);
    
        //Calculate the charge
        charge = chargeCalculator(mileage);
        totalCharge += charge;
        printf("CHARGE: \n • A flat rate of $80.00 for up to 200 km\n • 15 cents per km for the additional (i.e. beyond 200) kilometres");
        printf("\n Plate Number: %s \n The charge is $ %.2f. \n\n", plateNumber, charge);
        break;
      
      case 3:
        //Sent rented list to repair list
        printf("ACTION: Send a rented car to the Repair list\n\n");
        printf("Enter the plate number of the rented car: ");
        scanf("%s", plateNumber);
        printf("Enter the new mileage added to the rented car this time(km): ");
        scanf("%d", &mileage);
        sendCarToRepair(&headForRented, &headForRepair, plateNumber, mileage);

        //Sort the linked list
        sortByReturnDate(headForRented);
        //Update the repair file, 3 represents repair file
        updateFile(headForAvailable, headForRented, headForRepair, 3);
        //update the rented file, 2 represents rented file
        updateFile(headForAvailable, headForRented, headForRepair, 2);

        //Calculate the charge
        charge = chargeCalculator(mileage);
        totalCharge += charge;
        printf("CHARGE: \n • A flat rate of $80.00 for up to 200 km\n • 15 cents per km for the additional (i.e. beyond 200) kilometres");
        printf("\n Plate Number: %s \n The charge is $ %.2f. \n\n", plateNumber, charge);
        break;
      
      case 4:
        printf("ACTION: Send a repaired car to the Available-For-Rent list\n\n");
        printf("Enter the plate number of the rented car: ");
        scanf("%s", plateNumber);
        
        //Send repaired car to available list 
        repairedCarToAvailable(&headForRepair, &headForAvailable, plateNumber);

        //Sort by mileage
        sortByMileage(headForAvailable);

        //Update the repair file, 3 represents repair file
        updateFile(headForAvailable, headForRented, headForRepair, 3);
        //update the available file, 1 represents Available file
        updateFile(headForAvailable, headForRented, headForRepair, 1);
        break;
      
      case 5:
        printf("ACTION: Rent the first car from Available-For-Rent list\n\n");
        printf("Enter the expect return date (integer: yymmdd): ");
        scanf("%d", &returnDate);
        rentFirstCar(&headForAvailable, &headForRented, returnDate);
        printf("Car rented!\n\n");
        
        //Sort by return date
        sortByReturnDate(headForRented);
        //Update the rented file, 2 represents rented file
        updateFile(headForAvailable, headForRented, headForRepair, 2);
        //update the available file, 1 represents Available file
        updateFile(headForAvailable, headForRented, headForRepair, 1);
        break;
      
      case 6:
        printf("ACTION: Display all the lists\n\n");
        // printf("Before Sorted");
        // display(headForAvailable, headForRented, headForRepair);
        // printf("\n\n");
        sortByMileage(headForAvailable);
        sortByReturnDate(headForRented);
        display(headForAvailable, headForRented, headForRepair);
        //Update the rented file, 2 represents rented file
        updateFile(headForAvailable, headForRented, headForRepair, 2);
        //update the available file, 1 represents Available file
        updateFile(headForAvailable, headForRented, headForRepair, 1);
        break;
      
      case 7:
	printf("ACTION: Exit program.\n");
        printf("Bye.\n\n");
        charge = chargeCalculator(0);
        printf("TOTAL CHARGE:$ %.2f\n", totalCharge);
        //Delete the list
        freeList(headForAvailable, headForRented, headForRepair);
        exit(1);
        break;
      
      default:
        printf("Error, please re-enter your choice again.\n");
        break;
    }
  }
    return 0;
}
