
#include "pch.h"
//#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 200

const char userTxt[MAX] = "./UserInfo.txt";
const char bookTxt[MAX] = "./MyLibrary.txt";
//static int userID = 0, userNum = 0;

typedef struct {
	int year;
	int month;
	int day;
}Date;
typedef struct {
	long userID;
	char firstName[MAX];
	char lastName[MAX];
	int accountName;
	char psw[MAX];
	char userType;
}User;
typedef struct {
	int bookID;
	char bookName[MAX];
	char author[MAX];
	char user[MAX];
	Date borrowD;
	Date dueD;
}Book;

int findUserByName(char myAccountName[], char myPsw[]) {
	//printf("%s",path);
	char buffer[MAX],BorrowerChoice;
	FILE *userInfo = fopen(userTxt, "r+");
	if (!userInfo) {
		printf("Failed to open User.txt\n");
		exit(1);
	}
	while (!feof(userInfo)) {
		fgets(buffer, MAX, userInfo); // gets profile of user in buffer as char[]
		char *account, *psw, *userType;
		//Gets account info in account and compare.
		account = _strdup(buffer);
		account = strtok(account, ",");
		for (int i = 0; i < 3; i++) {
			account = strtok(NULL, ",");
		}
		//If account matched, compare password.
		if (strcmp(account, myAccountName) == 0) {
			psw = strtok(NULL, ",");
			if (strcmp(psw, myPsw) == 0) {
				printf("Log in successfully.\n");
				userType = strtok(NULL, "\n");
				if (strcmp(userType, "B") == 0) {
					printf("You are borrower.\n");
					printf("You are borrower.\n");
					printf("Borrower Menu\n");
					printf("Enter q: for book query by author sorted by book title\n");
					printf("Enter s: for book status query by name\n");
					printf("Enter u: to list books checked out by a given user\n");					
					scanf("%c", &BorrowerChoice);
				}
				else if (strcmp(userType, "L") == 0) {
					printf("You are librarian.\n");
				}
				return EXIT_SUCCESS;
			}
		}	
	}
	fclose(userInfo);
	printf("Log in failed\n");
	return EXIT_SUCCESS;
}
/* Get the last book's id number from myLibrary.txt.*/
int getBookID() {
	char buffer[MAX];
	FILE *bookInfo = fopen(bookTxt, "r+");
	if (!bookInfo) {
		printf("Failed to open MyLibrary.txt\n");
		exit(1);
	}
	while (!feof(bookInfo)) 
		fgets(buffer, MAX, bookInfo);
	char* bookIdString = strtok(_strdup(buffer),",");
	int bookID = strtol(bookIdString,bookIdString+strlen(bookIdString-1),10);
	return bookID;
}

void addBook() {
	char author[MAX] = { 0 };
	char title[MAX] = { 0 };

	printf("Enter book title:\n");
	scanf(" %[^\n]%*c", &title);
	/*input string with whitespace.
	[^\n]take input until newline;
	*c reads newline and discard it to prevent further problem for next input*/
	printf("Enter author name:\n");
	scanf(" %[^\n]s%*c", &author);
	
	if (strlen(author) == 0 || strlen(title) == 0) {
		printf("Invalid Entry! Try again");
		/*here go back to main menu*/
	}
	FILE *bookInfo = fopen(bookTxt, "a+");
	int bookID = getBookID();
	bookID++;
	fprintf(bookInfo, "%d, %s, %s, Library, null, null", bookID, title, author);
	fprintf(bookInfo, "\n");
	fclose(bookInfo);
	printf("Book: %s author: %s was added successfully!\n", title, author);
	/*go back to main menu or ask for enter more book*/

}

int main(void) {
	char myAccountName[MAX], myPsw[MAX];
	printf("Enter your account name: ");
	scanf("%s", &myAccountName);
	printf("Enter your password: ");
	scanf("%s", &myPsw);
	findUserByName(myAccountName, myPsw);
	addBook();
	return EXIT_SUCCESS;
}
