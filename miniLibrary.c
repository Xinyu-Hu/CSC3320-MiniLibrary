#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 100

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
static Book bookPool[MAX];


void BorrowerMenu(char BorrowerChoice,int bookNum);
void findBookByName(char *BookName, int bookNum);
void findBookByAuthor(char *author, int bookNum);
int TryAgainQ(int bookNum);
int TryAgainS(int bookNum);
int BackToMenu(int bookNum);


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
		if (strcmp(borrowDate, "null") != 0) {
			//myBook.borrowDate.tm_year = atoi(strtok(borrowDate, "-"));
			//myBook.borrowDate.tm_mon = atoi(strtok(NULL, "-"));
			//myBook.borrowDate.tm_mday = atoi(strtok(NULL, "-"));
			struct tm borrow;
			borrow.tm_year = atoi(strtok(borrowDate, "-"));
			borrow.tm_mon = atoi(strtok(NULL, "-"));
			borrow.tm_mday = atoi(strtok(NULL, "-"));
			myBook.borrowDate = borrow;
		}
		int cmp = strcmp(dueDate, "null\n");
		if (strcmp(dueDate, "null\n") != 0 && strcmp(dueDate, "null\r\n") != 0) {
			//myBook.dueDate.tm_year = atoi(strtok(dueDate, "-"));
			//myBook.dueDate.tm_mon = atoi(strtok(dueDate, "-"));
			//myBook.dueDate.tm_mday = atoi(strtok(dueDate, "-"));
			struct tm due;
			due.tm_year = atoi(strtok(dueDate, "-"));
			due.tm_mon = atoi(strtok(NULL, "-"));
			due.tm_mday = atoi(strtok(NULL, "-"));
			myBook.dueDate = due;
		}
		bookPool[myBook.bookID] = myBook;
		size = myBook.bookID;
	}
	return size;
}

//int buildDatePool(char* filePath) {
//	char buffer[MAX];
//	int size;
//	FILE *info = fopen(filePath, "r+");
//	if (!info) {
//		printf("Failed to open User.txt\n");
//		exit(1);
//	}
//	while (!feof(info)) {
//		fgets(buffer, MAX, info);
//		char *content = strtok(_strdup(buffer), ",");
//		int ID = atoi(content);
//		strtok(NULL, ",");
//		strtok(NULL, ",");
//		strtok(NULL, ",");
//		char *borrowDate = _strdup(strtok(NULL, ","));
//		char *dueDate = _strdup(strtok(NULL, ","));
//		struct tm borrow;
//		struct tm due;
//		if (strcmp(borrowDate, "null") == 0) {
//			return size = ID;
//		}else {
//			borrow.tm_year = atoi(strtok(borrowDate, "-"));
//			borrow.tm_mon = atoi(strtok(NULL, "-"));
//			borrow.tm_mday = atoi(strtok(NULL, "-"));
//		}
//		if (strcmp(dueDate, "null\n") == 0) {
//			return size = ID;
//		}else {
//			due.tm_year = atoi(strtok(dueDate, "-"));
//			due.tm_mon = atoi(strtok(NULL, "-"));
//			due.tm_mday = atoi(strtok(NULL, "-"));
//		}
//		datePool[ID].borrowDate = borrow;
//		datePool[ID].dueDate = due;
//		size = ID;
//	}
//	return size;
//}

void BorrowerMenu(char BorrowerChoice, int bookNum) {
	char *Author, *BookName;
	switch (BorrowerChoice) {
	case('q'):
		printf("Please enter Author's name: [FirstName LastName]\n");
		scanf("%[^\n]%*c", &Author);
		printf("Author %s has book(s) below:\n", Author);
		findBookByAuthor(Author, bookNum);
		break;
	case('s'):
		printf("Please enter Book name: \n");
		scanf("%[^\n]%*c",&BookName);
		findBookByName(BookName, bookNum);
		break;
	}
}
	//case('u'):
//return user index if user is found.
//int findUserByName(char myAccountName[], char myPsw[],User userPool[], int size) {
//	for (int i = 1; i <= size; i++) {
//		if (strcmp(userPool[i].accountName, myAccountName) == 0) {
//			if (strcmp(userPool[i].psw, myPsw) == 0) {
//				printf("Found.");
//				return	i;
//			}
//		}
//	}
//	printf("Not found.");
//	return 0;
//}


//return user index if user is found.
int findUserByName(char myAccountName[], char myPsw[], User userPool[], int size, int bookNum) {
	char BorrowerChoice;
	for (int i = 1; i <= size; i++) {
		if (strcmp(userPool[i].accountName, myAccountName) == 0) {
			if (strcmp(userPool[i].psw, myPsw) == 0) {
				printf("Found.");
				if (userPool[i].userType == 'B')
					printf("Borrower Menu:\n");
				printf("Enter “q” for book query by	author sorted by book title\n");
				printf("Enter “s” for book status query	by name\n");
				printf("Enter “u” to list books	checked	out	by a given user\n");
				scanf("%c", &BorrowerChoice);
				BorrowerMenu(BorrowerChoice, bookNum);
			}
			printf("Not found.");
		}
		return	i;
	}
}


//find all books of same author and save book array to Book list[]
//void findBookByAuthor(char* author, int bookNum) {
//	for (int i = 1; i <= bookNum; i++) {
//		if (strcmp(bookPool[i].author, author) == 0) {
//			printf("%s\n",bookPool[i].bookName);
//		}
//	}
//}
void findBookByAuthor(char *author, int bookNum) {
	char answer;
	for (int i = 1; i <= bookNum; i++) {
		if (strcmp(bookPool[i].author, author) == 0) {
			printf("%s\n", bookPool[i].bookName);
		}
	}
	printf("Press T for Try again or B for Back to menu\n");
	if (scanf("%c", &answer) == 1 && answer == 'T')
		TryAgainQ(bookNum);
	else if (answer == 'B') {
		BackToMenu(bookNum);
	}
}

int TryAgainQ(int bookNum) {
	char Author[MAX] = {0};
	printf("Please enter Author's name: [FirstName LastName]\n");
	scanf(" %[^\n]%*c", &Author);
	printf("Author %s has book(s) below:\n", Author);
	findBookByAuthor(Author, bookNum);
	return EXIT_SUCCESS;
}
int TryAgainS(int bookNum) {
	char BookName[MAX] = {0};
	printf("Please enter Book name: \n");
	scanf(" %[^\n]%*c", &BookName);
	findBookByName(BookName, bookNum);
	return EXIT_SUCCESS;
}
int BackToMenu(int bookNum) {
	char BorrowerChoice;
	printf("Borrower Menu:\n");
	printf("Enter 'q' for book query by	author sorted by book title\n");
	printf("Enter 's' for book status query	by name\n");
	printf("Enter 'u' to list books	checked	out	by a given user\n");
	scanf("%c", &BorrowerChoice);
	BorrowerMenu(BorrowerChoice, bookNum);
	exit(1);
}
void findBookByName(char *BookName, int bookNum) {
	char answer, year[10] = {0};
	for (int i = 1; i <= bookNum; i++) {
		char *searchName = _strdup(bookPool[i].bookName, strlen(bookPool[i].bookName) + 1);
		strcat(searchName, "\n");
		if (strcmp(searchName, BookName) == 0) {
			char buff[10] = {0};
			int a = bookPool[i].dueDate.tm_year;
			sprintf(buff, "%d",a );
			if (strcmp(buff, "null") == 0)
				printf("%d %s in library.\n", bookPool[i].bookID, bookPool[i].bookName);
			printf("%d %s Avaliable after %d-%d-%d\n", bookPool[i].bookID, bookPool[i].bookName, bookPool[i].dueDate.tm_year, bookPool[i].dueDate.tm_mon, bookPool[i].dueDate.tm_mday);
		}
	}
	printf("No such book!\n");
	printf("Press T for Try again or B for Back to menu\n");
	if (scanf("%c", &answer) == 1 && answer == 'T')
		TryAgainS(bookNum);
	if (answer == 'B')
		BackToMenu(bookNum);
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
	fprintf(bookInfo, "%d,%s,%s,Library,null,null", bookID, title, author);
	fprintf(bookInfo, "\n");
	fclose(bookInfo);
	printf("Book: %s author: %s added successfully!\n", title, author);
	/*go back to main menu or ask for enter more book*/
	}


int main(void) {
	int bookNum = buildBookPool(bookTxt);
	int userNum = buildUserPool(userTxt);

	char bookName[MAX] = {0};
	//char myAccountName[MAX], myPsw[MAX];
	//printf("Enter your account name: ");
	//scanf("%s", &myAccountName);
	//printf("Enter your password: ");
	//scanf("%s", &myPsw);
	//int r = findUserByName(myAccountName, myPsw, userPool, userNum, bookNum);
	//
	//if (r != 0) {
	//	printf("%s %s", userPool[r].firstName, userPool[r].lastName);
	//}
	//findBookByAuthor("J. K. Rowling", bookNum);

	printf("Enter book name: ");
	fgets(bookName, 50, stdin);
	findBookByName(bookName ,bookNum);
	printf("%d", bookPool[2].dueDate.tm_mon);
	

	return EXIT_SUCCESS;
}
