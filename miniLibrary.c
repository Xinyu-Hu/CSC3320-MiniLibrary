
#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 200

const char userTxt[MAX] = "./UserInfo.txt";
const char bookTxt[MAX] = "./MyLibrary.txt";

typedef struct {
	int year;
	int month;
	int day;
}Date;
typedef struct {
	int userID;
	char *firstName;
	char *lastName;
	char *accountName;
	char *psw;
	char userType;
}User;
typedef struct {
	int bookID;
	char bookName[MAX];
	char author[MAX];
	char user[MAX];
	Date borrowDate;
	Date dueDate;
}Book;

User* findUser(int fieldNum, char* value) {
	char buffer[MAX];
	FILE *userInfo = fopen(userTxt, "r+");
	if (!userInfo) {
		printf("Failed to open User.txt\n");
		exit(1);
	}
	while (!feof(userInfo)) {
		fgets(buffer, MAX, userInfo);
		char *content = _strdup(buffer);
		content = strtok(content, ",");
		char *userID = content;
		if (fieldNum > 1) {
			for (int j = 1; j < fieldNum; j++)
				content = strtok(NULL, ",");
		}
		if (strcmp(content, value) == 0) {
			//printf("Found user.");
			User foundUser;
			foundUser.userID = atoi(userID);
			return &foundUser;
		}
	}
	fclose(userInfo);
	//printf("User not found.");
	return NULL;
}

//return user* containing userID, accountName, password info if user is found.
User* findUserByName(char myAccountName[], char myPsw[]) {
	if (findUser(4, myAccountName) == NULL) {
		printf("Log in failed\n");
		return NULL;
	}
	
	if (findUser(5, myPsw) == NULL) {
		printf("Log in failed\n");
		return NULL;
	}
	printf("Log in successfully.\n");
	User* foundUser = findUser(5, myPsw);
	foundUser->accountName = myAccountName;
	foundUser->psw = myPsw;
	return foundUser;
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
	printf("Book: %s author: %s added successfully!\n", title, author);
	/*go back to main menu or ask for enter more book*/
	}

int main(void) {
	char myAccountName[MAX], myPsw[MAX];
	printf("Enter your account name: ");
	scanf("%s", &myAccountName);
	printf("Enter your password: ");
	scanf("%s", &myPsw);
	findUserByName(myAccountName, myPsw);
	return EXIT_SUCCESS;
}



