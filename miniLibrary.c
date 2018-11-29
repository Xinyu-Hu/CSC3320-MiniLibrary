
#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 200

const char *userTxt = "./UserInfo.txt";
const char *bookTxt = "./MyLibrary.txt";
typedef struct {
	int userID;
	char *firstName;
	char *lastName;
	char *accountName;
	char *psw;
	char userType;
	int size;
}User;
typedef struct {
	int bookID;
	char *bookName;
	char *author;
	char *status;
	struct tm borrowDate;
	struct tm  dueDate;
}Book;

User userPool[MAX];
Book bookPool[MAX];

int buildUserPool(char* filePath) {
	char buffer[MAX];
	int size = 0;
	FILE *info = fopen(filePath, "r+");
	if (!info) {
		printf("Failed to open User.txt\n");
		exit(1);
	}
	while (!feof(info)) {
		fgets(buffer, MAX, info);
		char *content = strtok(_strdup(buffer), ",");
		User myUser;
		myUser.userID = atoi(content);
		myUser.firstName = strtok(NULL, ",");
		myUser.lastName = strtok(NULL, ",");
		myUser.accountName = strtok(NULL, ",");
		myUser.psw = strtok(NULL, ",");
		myUser.userType = *(strtok(NULL, ","));
		userPool[myUser.userID] = myUser;
		size = myUser.userID;
	}
	return size;	 
}

int buildBookPool(char* filePath) {
	char buffer[MAX];
	int size;
	FILE *info = fopen(filePath, "r+");
	if (!info) {
		printf("Failed to open User.txt\n");
		exit(1);
	}
	while (!feof(info)) {
		fgets(buffer, MAX, info);
		char *content = strtok(_strdup(buffer), ",");
		Book myBook;
		myBook.bookID = atoi(content);
		myBook.bookName = strtok(NULL, ",");
		myBook.author = strtok(NULL, ",");
		myBook.status = strtok(NULL, ",");
		char *borrowDate = _strdup(strtok(NULL, ","));
		char *dueDate = _strdup(strtok(NULL, ","));
		//if (strcmp(borrowDate, "null") != 0) {
		//	myBook.borrowDate.tm_year = atoi(strtok(borrowDate, "-"));
		//	myBook.borrowDate.tm_mon = atoi(strtok(NULL, "-"));
		//	myBook.borrowDate.tm_mday = atoi(strtok(NULL, "-"));
		//}
		//if (strcmp(dueDate, "null") != 0) {
		//	myBook.dueDate.tm_year = atoi(strtok(dueDate, "-"));
		//	myBook.dueDate.tm_mon = atoi(strtok(NULL, "-"));
		//	myBook.dueDate.tm_mday = atoi(strtok(NULL, "-"));
		//}
		bookPool[myBook.bookID] = myBook;
		size = myBook.bookID;
	}
	return size;
}


//return user index if user is found.
int findUserByName(char myAccountName[], char myPsw[],User userPool[], int size) {
	for (int i = 1; i <= size; i++) {
		if (strcmp(userPool[i].accountName, myAccountName) == 0) {
			if (strcmp(userPool[i].psw, myPsw) == 0) {
				printf("Found.");
				return	i;
			}
		}
	}
	printf("Not found.");
	return 0;
}

//find all books of same author and save book array to Book list[]
void findBookByAuthor(char* author, int bookNum) {
	for (int i = 1; i <= bookNum; i++) {
		if (strcmp(bookPool[i].author, author) == 0) {
			printf("%s\n",bookPool[i].bookName);
		}
	}
}

void addBook(int bookNum) {
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
	int bookID = bookNum + 1;
	fprintf(bookInfo, "%d, %s, %s, Library, null, null", bookID, title, author);
	fprintf(bookInfo, "\n");
	fclose(bookInfo);
	printf("Book: %s author: %s added successfully!\n", title, author);
	/*go back to main menu or ask for enter more book*/
	}

int main(void) {
	int bookNum = buildBookPool(bookTxt);
	int userNum = buildUserPool(userTxt);

	//char myAccountName[MAX], myPsw[MAX];
	//printf("Enter your account name: ");
	//scanf("%s", &myAccountName);
	//printf("Enter your password: ");
	//scanf("%s", &myPsw);
	//int r = findUserByName(myAccountName, myPsw, userPool, userNum);
	//
	//if (r != 0) {
	//	printf("%s %s", userPool[r].firstName, userPool[r].lastName);
	//}
	findBookByAuthor("J. K. Rowling", bookNum);
	

	
	return EXIT_SUCCESS;
}



