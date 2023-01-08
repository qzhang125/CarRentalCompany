/*
Assignment 2 Q2
Name: Qiuyu Zhang
Student Number: 1244549
Date: Oct 22, 2022
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Stack {
  double number;
  struct Stack* nextNum;
};

int isEmpty(struct Stack* head);


//Push a new node to the list
struct Stack* addNode(struct Stack* head, int value){
  struct Stack* newNode = (struct Stack*)malloc(sizeof(struct Stack));
  if(isEmpty(head)){
    newNode->number = value;
    newNode->nextNum = NULL;
  }else{
    newNode->number = value;
    newNode->nextNum = head;
  }
  return newNode;
}

//check if the list is empty
int isEmpty(struct Stack* head){
  return !head;
}

//Delete the first node
struct Stack* pop(struct Stack* head){
  if(isEmpty(head)){
    printf("Error: empty list to pop.\n");
    exit(1);
  }
  //Point the temp to the second node
  struct Stack* temp = head->nextNum;
  //Delete the first item
  free(head);
  //Return the rest list
  return temp;
}

//Read and do the operation
struct Stack* operation(char op, struct Stack* head){
  double result = 0.0;
  //Check the operator
  if(op == '+'){
    result = head->nextNum->number + head->number;
  }else if(op == '-'){
    result = head->nextNum->number - head->number;
  }else if(op == '*'){
    result = head->number * head->nextNum->number;
  }else if(op == '/'){
    result =  head->nextNum->number / head->number;
  }else{
    //Exception
    printf("Cannot read the operator, please try another.\n");
  }
  //Delete the first node
  head = pop(head);
  //save the result for the next calculation
  head->number = result;
  return head;
}

int main(int argc, char* argv[]) {
  //Create a list
  struct Stack* head = NULL;
  int number = 0;
  char tempInput;
  //Check the argument number
  if(argc != 2){
    printf("The argument number is incorrect.\n");
    return -1;
  }
  //check the next element
  for(int i = 0; i < strlen(argv[1]); i++){
    tempInput = argv[1][i];
    //if the element is operator
    if(tempInput == '+' || tempInput == '-' || tempInput == '*' || tempInput == '/'){
      //calculate
      head = operation(tempInput, head);
    }else{
      //If the next element is number
      //Push the number into the stack
      number = argv[1][i] - '0';
      head = addNode(head, number);
    }
  }
  printf("Answer is: %.2f.\n", head->number);
  free(head);
  return 0;
}
