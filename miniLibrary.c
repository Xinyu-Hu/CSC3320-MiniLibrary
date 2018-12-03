#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define LEGAL 0
#define ILLEGAL 1
#define MAX 100



const char *userTxt = "./UserInfo.txt";
const char *bookTxt = "./MyLibrary.txt";
typedef struct {
	int year;
	int month;
	int day;
}mtime;
typedef struct {
	int userID;
	char *firstName;
	char *lastName;
	char *accountName;
	char *psw;
	char userType;
	int size;
	int fine;
}User;
typedef struct {
	int bookID;
	char *bookName;
	char *author;
	char *status;
	mtime borrowDate;
	mtime dueDate;
}Book;

User userPool[MAX];
Book bookPool[MAX];
//Log In
/*update from database to userPool array*/
int buildUserPool(char* filePath);
/*update from databaes to bookPool array*/
int buildBookPool(char* filePath);
int findUserByName(char myAccountName[], char myPsw[], User userPool[], int userNum, int bookNum);
int findAcct(char *acct, User userPool[], int userNum, int bookNum);
//Librarian
/*Check Out Book for user*/
int checkOutBook(int userNum, int bookNum, User userPool[]);
/*delete book from library*/
int deleteBook(int userNum, int bookNum);
/*add book to library*/
void addBook(int userNum, int bookNum);
/*check out book for the given user*/
int checkOutBook(int userNum, int bookNum, User userPool[]);
/*return book for the given user*/
void returnBook(int userNum, int bookNum);
/*check whether the book exist through input bookID*/
int checkBook(int bookNum, int bookID);
/*traverse through bookPool array and update book database*/
void updateBookSheet(char *pathName, int bookNum);
/*Librarian Menu*/
void LibrarianMenu(char Choice, int userNum, int bookNum, User userPool[]);
int LibrarianBackToMenu(int userNum, int bookNum);

/*convert borrow day to due day by adding 30 days*/
mtime convertTime(mtime date);

//Borrower
void findBookByName(char *BookName, int bookNum, char* AccountName);
void findBookByAuthor(char *author, int bookNum, char* AccountName);
int TryAgainQ(int bookNum, char* AccountName);
int TryAgainS(int bookNum, char* AccountName);
int TryAgainU(int year, int month, int date, int bookNum, char *AccountName);
/*traverse through userPool array and update user database*/
void updateUserSheet(char* pathName, int userNum);
int ListBooksDueDate(int year, int month, int day, int bookNum, char *AccountName);
int CheckIllegalDate(int year, int month, int date);
/*Borrower Menu*/
void BorrowerMenu(char BorrowerChoice, int bookNum, char* AccountName);
int BorrowerBackToMenu(int bookNum, char *AccountName);


int buildUserPool(char* filePath) {
	char buffer[MAX];
	int userNum = 0;
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
		myUser.fine = 0;
		userPool[myUser.userID] = myUser;
		userNum = myUser.userID;
	}
	fclose(info);
	return userNum;
}
int buildBookPool(char* filePath) {
	char buffer[MAX] = { 0 };
	int bookNum = 0;
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
			myBook.borrowDate.year = atoi(strtok(borrowDate, "-"));
			myBook.borrowDate.month = atoi(strtok(NULL, "-"));
			myBook.borrowDate.day = atoi(strtok(NULL, "-"));
		}
		else {
			myBook.borrowDate.year = 0;
			myBook.borrowDate.month = 0;
			myBook.borrowDate.day = 0;
		}
		int cmp = strcmp(dueDate, "null\n");
		if (strcmp(dueDate, "null\n") != 0 && strcmp(dueDate, "null\r\n") != 0) {
			myBook.dueDate.year = atoi(strtok(dueDate, "-"));
			myBook.dueDate.month = atoi(strtok(NULL, "-"));
			myBook.dueDate.day = atoi(strtok(NULL, "-"));
		}
		else {
			myBook.dueDate.year = 0;
			myBook.dueDate.month = 0;
			myBook.dueDate.day = 0;
		}
		bookPool[myBook.bookID] = myBook;
		bookNum = myBook.bookID;
	}
	fclose(info);
	return bookNum;
}
int findUserByName(char myAccountName[], char myPsw[], User userPool[], int userNum, int bookNum) {
	char Choice;
	for (int i = 1; i <= userNum; i++) {
		if (strcmp(userPool[i].accountName, myAccountName) == 0) {
			if (strcmp(userPool[i].psw, myPsw) == 0) {
				printf("Account is found.\n");
				if (userPool[i].userType == 'B') {
					printf("Borrower Menu:\n");
					printf("Enter 'q' for book query by author sorted by book title.\n");
					printf("Enter 's' for book status query	by name.\n");
					printf("Enter 'u' to list books	checked	out by a given user.\n");
					scanf(" %c", &Choice);
					BorrowerMenu(Choice, bookNum, userPool[i].accountName);
					return i;
				}else if (userPool[i].userType == 'L') {
					printf("Librarian Menu:\n");
					printf("Enter 'a' to add a book into library.\n");
					printf("Enter 'd' to delete a book from library.\n");
					printf("Enter 'o' to check out a book.\n");
					printf("Enter 'r' to return a book.\n");
					printf("Enter 'x' to quit.\n");
					scanf(" %c", &Choice);
					LibrarianMenu(Choice,userNum, bookNum,userPool);
					return i;
				}
			}
		}	
	}
	printf("Account is not found.");
}
int findAcct(char *acct, User userPool[], int userNum, int bookNum) {
	for (int i = 1; i <= userNum; i++) {
		if (strcmp(userPool[i].accountName, acct) == 0)
			return i;
	}
}
void findBookByAuthor(char *author, int bookNum, char* AccountName) {
	char answer;
	for (int i = 1; i <= bookNum; i++) {
		if (strcmp(bookPool[i].author, author) == 0) {
			printf("%s\n", bookPool[i].bookName);
		}
	}
	printf("Press T for Try again or B for Back to menu\n");
	if (scanf("%c", &answer) == 1 && answer == 'T')
		TryAgainQ(bookNum, AccountName);
	else if (answer == 'B') {
		BorrowerBackToMenu(bookNum, AccountName);
	}
}
void findBookByName(char *BookName, int bookNum, char* AccountName) {
	char answer;//, year[10] = {0};
	int i = 1;
	while (i <= bookNum) {
		//searchName = bookPool[i] + "\n"
		char *searchName = _strdup(bookPool[i].bookName, strlen(bookPool[i].bookName) + 1);
		strcat(searchName, "\n");
		if (strcmp(searchName, BookName) == 0 || strcmp(bookPool[i].bookName, BookName) == 0) {
			char* status = bookPool[i].status;
			if (strcmp(status, "Library") == 0) {
				printf("%d %s is in library.\n", bookPool[i].bookID, bookPool[i].bookName);
				break;
			}
			else {
				printf("%d %s Avaliable after %d-%d-%d\n", bookPool[i].bookID, bookPool[i].bookName, bookPool[i].dueDate.year, bookPool[i].dueDate.month, bookPool[i].dueDate.day);
				break;
			}
		}
		i++;
	}
	if (i > bookNum)
		printf("No such book!\n");
	printf("Press T for Try again or B for Back to menu\n");
	if (scanf("%c", &answer) == 1 && answer == 'T')
		TryAgainS(bookNum, AccountName);
	if (answer == 'B')
		BorrowerBackToMenu(bookNum, AccountName);
}
int TryAgainQ(int bookNum, char* AccountName) {
	char Author[MAX] = {0};
	printf("Please enter Author's name: [FirstName LastName]\n");
	scanf(" %[^\n]%*c", &Author);
	printf("Author %s has book(s) below:\n", Author);
	findBookByAuthor(Author, bookNum, AccountName);
	return EXIT_SUCCESS;
}
int TryAgainS(int bookNum, char* AccountName) {
	char BookName[MAX] = {0};
	printf("Please enter Book name: \n");
	scanf(" %[^\n]%*c", &BookName);
	findBookByName(BookName, bookNum,AccountName);
	return EXIT_SUCCESS;
}
int TryAgainU( int year, int month, int date, int bookNum, char *AccountName) {
	//printf("Please enter current date: Year-Month-Date \n");
	//scanf("%d-%d-%d", &year, &month, &date);
	ListBooksDueDate(year, month, date, bookNum, AccountName);
	return EXIT_SUCCESS;
}
void BorrowerMenu(char Choice, int bookNum, char* AccountName) {
	char Author[MAX], BookName[MAX];
	int year, month, date;
	switch (Choice) {
	case('q'):
		printf("Please enter Author's name: [FirstName LastName]\n");
		scanf(" %[^\n]%*c", &Author);
		printf("Author %s has book(s) below:\n", Author);
		findBookByAuthor(Author, bookNum, AccountName);
		break;
	case('s'):
		printf("Please enter Book name: \n");
		scanf(" %[^\n]%*c", &BookName);
		findBookByName(BookName, bookNum, AccountName);
		break;
	case('u'):
		printf("Please enter current date: Year-Month-Date\n");
		scanf(" %d-%d-%d", &year, &month, &date);
		ListBooksDueDate(year, month, date, bookNum, AccountName);
		break;
	default:
		printf("Invalid input.\n");
	}

}
int BorrowerBackToMenu(int bookNum, char* AccountName) {
	char Choice;
	printf("Borrower Menu:\n");
	printf("Enter 'q' for book query by author sorted by book title.\n");
	printf("Enter 's' for book status query	by name.\n");
	printf("Enter 'u' to list books	checked	out by a given user.\n");
	scanf(" %c", &Choice);
	BorrowerMenu(Choice, bookNum, AccountName);
	exit(1);
}
int LibrarianBackToMenu(int userNum, int bookNum) {
	char Choice;
	printf("Librarian Menu:\n");
	printf("Enter 'a' to add a book into library.\n");
	printf("Enter 'd' to delete a book from library.\n");
	printf("Enter 'o' to check out a book.\n");
	printf("Enter 'r' to return a book.\n");
	printf("Enter 'x' to quit.\n");
	scanf(" %c", &Choice);
	LibrarianMenu(Choice, bookNum,userNum, userPool);
	exit(1);
}
void LibrarianMenu(char Choice, int userNum, int bookNum, User userPool[]) {
	char Author[MAX], BookName[MAX];
	switch (Choice) {
	case('a'): //add book
		addBook(userNum, bookNum);
		break;
	case('d')://delete book
		deleteBook(userNum, bookNum);
		break;
	case('o')://check out book
		checkOutBook(userNum, bookNum, userPool);
		break;
	case('r')://return book
		returnBook(userNum, bookNum);
		break;
	case('x')://quit
		break;
	default:
		printf("Invalid input.\n");
	}
}

void updateUserSheet(char* pathName, int userNum) {
	FILE *fp = fopen(pathName, "w+");
	//int size = 4;
	int n = 1;
	while (n < userNum) {
		User tempUser = userPool[n];

		fprintf(fp, "%d,%s,%s,%s,%s,%s", tempUser.userID, tempUser.firstName, tempUser.lastName,
			tempUser.accountName, tempUser.psw, tempUser.userType);
		fprintf(fp, "\n");
		n++;
	}
	fclose(fp);
}
void updateBookSheet(char *pathName, int bookNum) {
	FILE *fp = fopen(pathName, "w+");
	//int size = buildBookPool(bookTxt);
	int n = 1;
	while (n <= bookNum) {
		Book tempBook = bookPool[n];
		fprintf(fp, "%d,%s,%s,%s,%d-%d-%d,%d-%d-%d\n", tempBook.bookID, tempBook.bookName, tempBook.author, tempBook.status,
			tempBook.borrowDate.year, tempBook.borrowDate.month, tempBook.borrowDate.day,
			tempBook.dueDate.year, tempBook.dueDate.month, tempBook.dueDate.day);

	//	fprintf(fp, "\n");
		n++;
	}
	fclose(fp);
}
mtime convertTime(mtime date) {//pass struct time as argument
	mtime newDate = date;
	if (date.month == 1 || date.month == 3 || date.month == 5 || date.month == 7 || date.month == 8 || date.month == 10) {
		if (date.day + 30 <= 31) {
			newDate.year = date.year;
			newDate.month = date.month;
			newDate.day = date.day + 30;
		}
		else {
			newDate.year = date.year;
			newDate.month = date.month + 1;
			newDate.day = date.day + 30 - 31;
		}
	}
	else if (date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11) {
		newDate.year = date.year;
		newDate.month = date.month + 1;
		newDate.day = date.day;
	}
	else if (date.month == 2) {
		newDate.year = date.year;
		newDate.month = date.month + 1;
		newDate.day = date.day + 30 - 28;
	}
	else {// the 12th month
		if (date.day + 30 <= 31) {
			newDate.year = date.year;
			newDate.month = date.month;
			newDate.day = date.day + 30;
		}
		else {
			newDate.year = date.year + 1;
			newDate.month = 1;
			newDate.day = date.day + 30 - 31;
		}
	}
	return newDate;
}

int checkBook(int bookNum, int bookID) {
	int n = 1;
	while (n <= bookNum) {
		if (bookPool[n].bookID == bookID) {
			return 0;
			break;
		}
		n++;
	}
	return 1;
}

//TEST THIS!!!
int ListBooksDueDate(int year, int month, int day, int bookNum, char *AccountName) {
	char *answer;
	time_t curtime;
	struct tm *currentDate;
	time(&curtime);
	currentDate = localtime(&curtime); //generate current time and save it as borrow day; 
	currentDate->tm_year += 1900;
	currentDate->tm_mon += 1;

	if (CheckIllegalDate(year, month, day) == ILLEGAL) {
		printf("Illegal input date!\n");
		printf("Press T for Try again with system current date or B for Back to menu\n");
		if (scanf(" %c", &answer) == 1 && answer == 'T')
			TryAgainU(currentDate->tm_year + 1900, currentDate->tm_mon + 1, currentDate->tm_mday, bookNum, AccountName);
		if (answer == 'B')
			BorrowerBackToMenu(bookNum, AccountName);
	}

	for (int i = 1; i <= bookNum; i++) {
		if (strcmp(bookPool[i].status, AccountName) == 0) {
			mtime DueDate = bookPool[i].dueDate;
			//if (currentDate->tm_year > DueDate.year) {
			//	printf("%d,%s,%s,%s,%d-%d-%d,%d-%d-%d,Over Due", bookPool[i].bookID, bookPool[i].bookName, bookPool[i].author, bookPool[i].status,
			//		bookPool[i].borrowDate.year, bookPool[i].borrowDate.month, bookPool[i].borrowDate.day, bookPool[i].dueDate.year, bookPool[i].dueDate.month, bookPool[i].dueDate.day);
			//}else {
			//	printf("%d,%s,%s,%d-%d-%d,%d-%d-%d,%s", bookPool[i].bookID, bookPool[i].bookName, bookPool[i].author, bookPool[i].status,
			//		bookPool[i].borrowDate.year,bookPool[i].borrowDate.month, bookPool[i].borrowDate.day, bookPool[i].dueDate.year, bookPool[i].dueDate.month, bookPool[i].dueDate.day);
			//}
			//Date current date
			//int bookYear = bookPool[i].dueDate.year;
			//int bookMon = bookPool[i].dueDate.month;
			//int bookDay = bookPool[i].dueDate.day;
			if (DueDate.year < currentDate->tm_year) {
				printf("%d,%s,%s,%s,%d-%d-%d,%d-%d-%d,Over Due\n", bookPool[i].bookID, bookPool[i].bookName, bookPool[i].author, bookPool[i].status,
					bookPool[i].borrowDate.year, bookPool[i].borrowDate.month, bookPool[i].borrowDate.day, bookPool[i].dueDate.year, bookPool[i].dueDate.month, bookPool[i].dueDate.day);
			}
			else if (DueDate.month < currentDate->tm_mon ) {
				printf("%d,%s,%s,%s,%d-%d-%d,%d-%d-%d,Over Due\n", bookPool[i].bookID, bookPool[i].bookName, bookPool[i].author, bookPool[i].status,
					bookPool[i].borrowDate.year, bookPool[i].borrowDate.month, bookPool[i].borrowDate.day, bookPool[i].dueDate.year, bookPool[i].dueDate.month, bookPool[i].dueDate.day);
			}
			else if (DueDate.day < currentDate->tm_mday){
				printf("%d,%s,%s,%s,%d-%d-%d,%d-%d-%d,Over Due\n", bookPool[i].bookID, bookPool[i].bookName, bookPool[i].author, bookPool[i].status,
					bookPool[i].borrowDate.year, bookPool[i].borrowDate.month, bookPool[i].borrowDate.day, bookPool[i].dueDate.year, bookPool[i].dueDate.month, bookPool[i].dueDate.day);
			}else {
				printf("%d,%s,%s,%d-%d-%d,%d-%d-%d,%s, not over due\n", bookPool[i].bookID, bookPool[i].bookName, bookPool[i].author, bookPool[i].status,
					bookPool[i].borrowDate.year, bookPool[i].borrowDate.month, bookPool[i].borrowDate.day, bookPool[i].dueDate.year, bookPool[i].dueDate.month, bookPool[i].dueDate.day);
			}	
		}	
	}
	printf("Press T for Try again or B for Back to menu\n");
	if (scanf(" %c", &answer) == 1 && answer == 'T')
		TryAgainU(currentDate->tm_year, currentDate->tm_mon, currentDate->tm_mday, bookNum, AccountName);
	if (answer == 'B')
		BorrowerBackToMenu(bookNum, AccountName);
	//return EXIT_SUCCESS;
}

int CheckIllegalDate(int yy, int mm, int dd){
	//check year
	if (yy >= 1900 && yy <= 9999){
		//check month
		if (mm >= 1 && mm <= 12){
			//check days
			if ((dd >= 1 && dd <= 31) && (mm == 1 || mm == 3 || mm == 5 || mm == 7 || mm == 8 || mm == 10 || mm == 12)) {
				printf("Date is valid.\n");
				return LEGAL;
			}else if ((dd >= 1 && dd <= 30) && (mm == 4 || mm == 6 || mm == 9 || mm == 11)) {
				printf("Date is valid.\n");
				return LEGAL;
			}else if ((dd >= 1 && dd <= 28) && (mm == 2)) {
				printf("Date is valid.\n");
				return LEGAL;
			}else if (dd == 29 && mm == 2 && (yy % 400 == 0 || (yy % 4 == 0 && yy % 100 != 0))) {
				printf("Date is valid.\n");
				return LEGAL;
			}else {
				printf("Day is invalid.\n");
				return ILLEGAL;
			}
		}else{
			printf("Month is not valid.\n");
			return ILLEGAL;
		}
	}else{
		printf("Year is not valid.\n");
		return ILLEGAL;
	}
}
//int CheckIllegalDate(int year, int month, int date){
//	struct tm input;
//	time_t res;
//	input.tm_year = year -1900;
//	input.tm_mon = month -1;
//	input.tm_mday = date;
//	res = mktime(&input);
//	char resStr[MAX] = {0};
//	char *str = localtime(&res);
//	strftime(resStr, 100, "%Y-%m-%d", str);
//	char *yearStr = strtok(resStr, "-");
//	int yearR = atoi(yearStr) - 1900;
//	char * monStr = strtok(NULL, "-");
//	int monR = atoi(monStr) - 1;
//	char *dayStr = strtok(NULL, "-");
//	int dayR = atoi(dayStr);
//	if (year == yearR && month == monR && date == dayR)
//		return LEGAL;
//	return ILLEGAL;
//}



void addBook(int userNum, int bookNum) {
	char author[MAX] = { 0 };
	char title[MAX] = { 0 };
	char choice;

	printf("Enter book title:\n");
	scanf(" %[^\n]%*c", &title);
	/*input string with whitespace.
	[^\n]take input until newline;
	*c reads newline and discard it to prevent further problem for next input*/
	printf("Enter author name:\n");
	scanf(" %[^\n]s%*c", &author);

	if (strlen(author) == 0 || strlen(title) == 0) {
		printf("Invalid Entry! Enter T to Try again or B back to Main menu.\n");
		/*here go back to main menu*/
		scanf(" %c", &choice);
		if (choice == "T") {
			addBook(userNum, bookNum);
		}
		if (choice == "B") {
			LibrarianBackToMenu(userNum, bookNum);
		}
	}

	if (bookNum < MAX) {
		bookPool[bookNum + 1].author = author;
		bookPool[bookNum + 1].bookID = bookNum + 1;
		bookPool[bookNum + 1].bookName = title;
		bookPool[bookNum + 1].status = "Library";
		bookPool[bookNum + 1].borrowDate.year = 0;
		bookPool[bookNum + 1].borrowDate.month = 0;
		bookPool[bookNum + 1].borrowDate.day = 0;
		bookPool[bookNum + 1].dueDate.year = 0;
		bookPool[bookNum + 1].dueDate.month = 0;
		bookPool[bookNum + 1].dueDate.day = 0;
		bookNum++;
		updateBookSheet(bookTxt, bookNum);
		printf("Book %s is sucessfully added\n", title);
		LibrarianBackToMenu(userNum, bookNum);
	}
	/*if bookself is full not able to addBook*/
	else {
		printf("Bookself is Full\n");
		LibrarianBackToMenu(userNum, bookNum);
	}
}
int deleteBook(int userNum, int bookNum) {
	int enterBookID, n = 1;
	char choice;
	printf("Please Enter Book ID.\n");
	scanf(" %d", &enterBookID);

	if (enterBookID == 0) {
		printf("Invalid Entry! Enter T to Try again or B back to Main menu.\n");
		scanf(" %c", &choice);
		if (choice == 'T')
			deleteBook(userNum, bookNum);
		if (choice == 'B')
			LibrarianBackToMenu(userNum, bookNum);
	}else{	
		while (n <= bookNum) {/*shift array to the left*/
			Book temptBook = bookPool[n];
			if (enterBookID == temptBook.bookID) {
				for (int i = n; i <= bookNum; i++) {
					bookPool[i] = bookPool[i + 1];
				}
				break;
			}
			n++;
		}
		bookNum--;
		updateBookSheet(bookTxt, bookNum);
		printf("Book successfully deleted\n");
		LibrarianBackToMenu(userNum, bookNum);
	}
}
int checkOutBook(int userNum, int bookNum, User userPool[]) {
	int n = 1, counter = 0, i = 1;
	int bookID = 0, userID;
	char acct[MAX];
	printf("Enter borrower account name\n");
	scanf(" %s", &acct);
	userID = findAcct( acct, userPool,  userNum,  bookNum);
	/*Find the number of book  associated with user account name*/
	for (i=1; i <= bookNum; i++) {
		if (strcmp(userPool[userID].accountName, bookPool[i].status) == 0) {
			counter++;
		}
	}
	printf("Enter Book ID\n");
	scanf(" %d", &bookID);
	if (counter >= 3) {
		printf("User %s\'s check out slot is full.\n", userPool[userID].userID);
		char choice;
		printf("Enter T to Try again or B back to Main menu");
		scanf(" %c", &choice);
		if (choice == 'T')
			deleteBook(userNum, bookNum);
		if (choice == 'B')
			LibrarianBackToMenu(userNum, bookNum);
	}else if (checkBook(bookNum, bookID) == 0) {//check if book exist
		time_t curtime;
		struct tm *checkin;
		time(&curtime);
		checkin = localtime(&curtime); //generate current time and save it as borrow day; 

		bookPool[bookID].status = acct;

		bookPool[bookID].borrowDate.year = checkin->tm_year + 1900;
		bookPool[bookID].borrowDate.month = checkin->tm_mon + 1;
		bookPool[bookID].borrowDate.day = checkin->tm_mday;

		mtime checkoutDate = convertTime(bookPool[bookID].borrowDate);

		bookPool[bookID].dueDate.year = checkoutDate.year;
		bookPool[bookID].dueDate.month = checkoutDate.month;
		bookPool[bookID].dueDate.day = checkoutDate.day;
		printf("Book Successfully Checked Out.\n");
		updateBookSheet(bookTxt, bookNum);
		LibrarianBackToMenu(userNum, bookNum);

	}else {
		char choice;
		printf("Invalid Entry! Enter T to Try again or B back to Main menu.\n");
		scanf(" %c", &choice);
		if (choice == 'T') {
			checkOutBook(userNum, bookNum,userPool);
		}else if (choice == 'B') {
			LibrarianBackToMenu(userNum, bookNum);
		}
	}
}
void returnBook(int userNum, int bookNum) {
	int n = 1, counter = 0, year, month, day;
	int bookID = 0;
	printf("Enter Book ID.\n");
	scanf(" %d", &bookID);

	while (n <= userNum) {
		if (strcmp(bookPool[bookID].status, userPool[n].accountName)) {
			break;
		}
		else {
			n++;
		}
	}

	if (checkBook(bookNum, bookID) != 0) {
		char choice;
		printf("No Such Book. Enter T to Try again or B back to Main menu\n");
		scanf(" %c", &choice);
		if (choice == 'T')
			returnBook(userNum, bookNum);
		if (choice == 'B')
			LibrarianBackToMenu(userNum, bookNum);
	}else {
		//time_t curtime;
		//time(&curtime);
		//struct tm *checkout;
		//checkout = localtime(curtime);
		time_t curtime;
		struct tm *checkout;
		time(&curtime);
		checkout = localtime(&curtime); //generate current time and save it as borrow day;
		if (checkout->tm_year + 1900 < bookPool[bookID].dueDate.year) {
			bookPool[bookID].borrowDate.month = 0;
			bookPool[bookID].borrowDate.day = 0;
			bookPool[bookID].borrowDate.year = 0;

			bookPool[bookID].dueDate.month = 0;
			bookPool[bookID].dueDate.day = 0;
			bookPool[bookID].dueDate.year = 0;

			bookPool[bookID].status = "Library";

			printf("Book %s successfully check out.\n", bookPool[bookID].bookName);
			updateBookSheet(bookTxt, bookNum);
			LibrarianBackToMenu(userNum, bookNum);
		}
		else if (checkout->tm_mon + 1 < bookPool[bookID].dueDate.month) {
			bookPool[bookID].borrowDate.month = 0;
			bookPool[bookID].borrowDate.day = 0;
			bookPool[bookID].borrowDate.year = 0;

			bookPool[bookID].dueDate.month = 0;
			bookPool[bookID].dueDate.day = 0;
			bookPool[bookID].dueDate.year = 0;

			bookPool[bookID].status = "Library";

			printf("Book %s successfully check out.\n", bookPool[bookID].bookName);
			updateBookSheet(bookTxt, bookNum);
			LibrarianBackToMenu(userNum, bookNum);
		}else if(checkout->tm_mday <= bookPool[bookID].dueDate.day){
			bookPool[bookID].borrowDate.month = 0;
			bookPool[bookID].borrowDate.day =0;
			bookPool[bookID].borrowDate.year = 0;

			bookPool[bookID].dueDate.month = 0;
			bookPool[bookID].dueDate.day =0;
			bookPool[bookID].dueDate.year = 0;

			bookPool[bookID].status = "Library";

			printf("Book %s successfully check out.\n", bookPool[bookID].bookName);
			updateBookSheet(bookTxt, bookNum);
			LibrarianBackToMenu(userNum, bookNum);
		}else{
			printf("Book is Pass Due.\n");
			printf("A fine of 500 is imposed.");
			userPool[n].fine = 500;
			updateBookSheet(bookTxt, bookNum);
			LibrarianBackToMenu(userNum, bookNum);
		}
	}
	return EXIT_SUCCESS;
}


int main(void) {
	 int bookNum = buildBookPool(bookTxt);
	 int userNum = buildUserPool(userTxt);

	char bookName[MAX] = {0};
	char myAccountName[MAX], myPsw[MAX];
	printf("Enter your account name: ");
	scanf("%s", &myAccountName);
	printf("Enter your password: ");
	scanf("%s", &myPsw);
	int r = findUserByName(myAccountName, myPsw, userPool, userNum, bookNum);	
	if (r != 0) {
		printf("User %s %s found.\n", userPool[r].firstName, userPool[r].lastName);
	}
	char userAccount[MAX] = { 0 };
	userAccount[MAX] =_strdup(userPool[r].accountName);
	return EXIT_SUCCESS;
}


