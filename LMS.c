#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Structure for Book records
struct Book 
{
    char name[50];
    char author[50];
    char category[30];
    int quantity;
    int issued;
};






// Structure for Borrower records
struct Borrower 
{
    char name[50];
    char contact[20];
    char bookNames[100][50]; // Array to store multiple book names
    int issueDates[100];     // Array to store issue dates for each book
    int returnDates[100];    // Array to store return dates for each book
    int bookCount;           // Number of books borrowed
};







// Global variables and file pointers
struct Book books[100];
struct Borrower borrowers[100];
int bookCount = 0, borrowerCount = 0;







// File names
const char *bookFile = "books.txt";
const char *borrowerFile = "borrowers.txt";






// Function prototypes
void loadRecords();
void saveRecords();
void addBook();
void searchBook();
void issueBook();
void returnBook();
void calculateFine();
void displayBooks();
void displayBorrowers();
void deleteBook();
void editBook();
void deleteBorrower();
void editBorrower();
void clearRecords();









// Load records from files
// Load records from files
void loadRecords() 
{
    FILE *fp;
    int i;

    fp = fopen(bookFile, "r");
    if (fp != NULL) 
    {
        fscanf(fp, "%d\n", &bookCount);
        for (i = 0; i < bookCount; i++) 
        {
            fscanf(fp, "%s %s %d\n", books[i].name, books[i].author, &books[i].quantity);
        }
        fclose(fp);
    }

    fp = fopen(borrowerFile, "r");
    if (fp != NULL) 
    {
        fscanf(fp, "%d\n", &borrowerCount);
        for (i = 0; i < borrowerCount; i++) 
        {
            fscanf(fp, "%s %s %d\n", borrowers[i].name, borrowers[i].contact, &borrowers[i].bookCount);
        }
        fclose(fp);
    }
}







// Save records to files
void saveRecords() 
{
    FILE *fp;
    int i;

    fp = fopen(bookFile, "w");
    if (fp != NULL) 
    {
        fprintf(fp, "%d\n", bookCount);
        for (i = 0; i < bookCount; i++) 
        {
            fprintf(fp, "%s %s %d\n", books[i].name, books[i].author, books[i].quantity);
        }
        fclose(fp);
    }

    fp = fopen(borrowerFile, "w");
    if (fp != NULL) {
        fprintf(fp, "%d\n", borrowerCount);
        for (i = 0; i < borrowerCount; i++) 
        {
            fprintf(fp, "%s %s %d\n", borrowers[i].name, borrowers[i].contact, borrowers[i].bookCount);
        }
        fclose(fp);
    }
}









// Add a new book
void addBook() 
{
    printf("Enter book name: ");
    scanf(" %[^\n]", books[bookCount].name);
    printf("Enter author name: ");
    scanf(" %[^\n]", books[bookCount].author);
    printf("Enter category: ");
    scanf(" %[^\n]", books[bookCount].category);
    printf("Enter quantity: ");
    scanf("%d", &books[bookCount].quantity);
    books[bookCount].issued = 0;
    bookCount++;
    saveRecords();
    printf("Book added successfully!\n");
}








// Search for a book
void searchBook() 
{
    char searchKey[50];
    printf("Enter book name, author name, or category to search: ");
    scanf(" %[^\n]", searchKey);
    printf("Search Results:\n");
    for (int i = 0; i < bookCount; i++) 
    {
        if (strstr(books[i].name, searchKey) || strstr(books[i].author, searchKey) || strstr(books[i].category, searchKey)) 
        {
            printf("Book Name: %s, Author: %s, Category: %s, Available: %d\n",
                   books[i].name, books[i].author, books[i].category,
                   books[i].quantity - books[i].issued);
        }
    }
}








// Issue a book to a borrower
void issueBook() 
{
    char borrowerName[50], contact[20], bookName[50];
    int issueDate, returnDate;

    printf("Rules: A fee of 30 taka per day will be charged for late returns of borrowed books\n");

    printf("Enter borrower's name: ");
    scanf(" %[^\n]", borrowerName);
    printf("Enter contact number: ");
    scanf(" %[^\n]", contact);

    // Check if borrower already exists
    int borrowerIndex = -1;
    for (int i = 0; i < borrowerCount; i++) 
    {
        if (strcmp(borrowers[i].name, borrowerName) == 0) 
        {
            borrowerIndex = i;
            break;
        }
    }

    if (borrowerIndex == -1) 
    {
        // New borrower
        borrowerIndex = borrowerCount++;
        strcpy(borrowers[borrowerIndex].name, borrowerName);
        strcpy(borrowers[borrowerIndex].contact, contact);
        borrowers[borrowerIndex].bookCount = 0;
    }

    printf("Enter book name to issue: ");
    scanf(" %[^\n]", bookName);

    // Find the book
    int found = -1;
    for (int i = 0; i < bookCount; i++) 
    {
        if (strcmp(books[i].name, bookName) == 0) 
        {
            found = i;
            break;
        }
    }

    if (found == -1 || books[found].issued >= (books[found].quantity - 1)) 
    {
        printf("Book not available.\n");
        return;
    }

    printf("Enter issue date (YYYYMMDD): ");
    scanf("%d", &issueDate);
    printf("Enter return date (YYYYMMDD): ");
    scanf("%d", &returnDate);

    // Update book and borrower records
    books[found].issued++;
    strcpy(borrowers[borrowerIndex].bookNames[borrowers[borrowerIndex].bookCount], bookName);
    borrowers[borrowerIndex].issueDates[borrowers[borrowerIndex].bookCount] = issueDate;
    borrowers[borrowerIndex].returnDates[borrowers[borrowerIndex].bookCount] = returnDate;
    borrowers[borrowerIndex].bookCount++;
    saveRecords();
    printf("Book issued successfully!\n");
}












// Return a book
void returnBook() 
{
    char borrowerName[50], bookName[50];
    int actualReturnDate;
    printf("Enter borrower's name: ");
    scanf(" %[^\n]", borrowerName);

    // Find the borrower
    int borrowerIndex = -1;
    for (int i = 0; i < borrowerCount; i++) 
    {
        if (strcmp(borrowers[i].name, borrowerName) == 0) 
        {
            borrowerIndex = i;
            break;
        }
    }

    if (borrowerIndex == -1) 
    {
        printf("Borrower not found.\n");
        return;
    }

    printf("Enter book name to return: ");
    scanf(" %[^\n]", bookName);

    // Find the book in borrower's record
    int bookIndex = -1;
    for (int i = 0; i < borrowers[borrowerIndex].bookCount; i++) 
    {
        if (strcmp(borrowers[borrowerIndex].bookNames[i], bookName) == 0) 
        {
            bookIndex = i;
            break;
        }
    }

    if (bookIndex == -1) 
    {
        printf("Book not found in borrower's record.\n");
        return;
    }

    printf("Enter actual return date (YYYYMMDD): ");
    scanf("%d", &actualReturnDate);

    // Calculate fine
    int fine = 0;
    if (actualReturnDate > borrowers[borrowerIndex].returnDates[bookIndex]) 
    {
        fine = (actualReturnDate - borrowers[borrowerIndex].returnDates[bookIndex]) * 30; // 30 Tk per day
    }
    printf("Fine for %s: %d Tk\n", borrowerName, fine);

    // Update book and borrower records
    for (int i = 0; i < bookCount; i++) 
    {
        if (strcmp(books[i].name, bookName) == 0) 
        {
            books[i].issued--;
            break;
        }
    }

    // Remove the book from borrower's record
    for (int i = bookIndex; i < borrowers[borrowerIndex].bookCount - 1; i++) 
    {
        strcpy(borrowers[borrowerIndex].bookNames[i], borrowers[borrowerIndex].bookNames[i + 1]);
        borrowers[borrowerIndex].issueDates[i] = borrowers[borrowerIndex].issueDates[i + 1];
        borrowers[borrowerIndex].returnDates[i] = borrowers[borrowerIndex].returnDates[i + 1];
    }
    borrowers[borrowerIndex].bookCount--;

    // If borrower has no more books, remove the borrower
    if (borrowers[borrowerIndex].bookCount == 0) 
    {
        for (int i = borrowerIndex; i < borrowerCount - 1; i++) 
        {
            borrowers[i] = borrowers[i + 1];
        }
        borrowerCount--;
    }

    saveRecords();
    printf("Book returned successfully!\n");
}











// Calculate fine for late return
void calculateFine() 
{
    char borrowerName[50];
    int actualReturnDate;
    printf("Enter borrower's name: ");
    scanf(" %[^\n]", borrowerName);
    printf("Enter actual return date (YYYYMMDD): ");
    scanf("%d", &actualReturnDate);

    for (int i = 0; i < borrowerCount; i++) 
    {
        if (strcmp(borrowers[i].name, borrowerName) == 0) 
        {
            for (int j = 0; j < borrowers[i].bookCount; j++) 
            {
                int fine = 0;
                if (actualReturnDate > borrowers[i].returnDates[j]) 
                {
                    fine = (actualReturnDate - borrowers[i].returnDates[j]) * 30; // 30 Tk per day
                }
                printf("Fine for book '%s': %d Tk\n", borrowers[i].bookNames[j], fine);
            }
            return;
        }
    }
    printf("Borrower not found.\n");
}













// Display all books
void displayBooks() 
{
    printf("Book List:\n");
    for (int i = 0; i < bookCount; i++) 
    {
        printf("Name: %s, Author: %s, Category: %s, Quantity: %d, Issued: %d\n",
               books[i].name, books[i].author, books[i].category,
               books[i].quantity, books[i].issued);
    }
}










// Display all borrowers
void displayBorrowers() 
{
    printf("Borrower List:\n");
    for (int i = 0; i < borrowerCount; i++) 
    {
        printf("Name: %s, Contact: %s, Books: ", borrowers[i].name, borrowers[i].contact);
        for (int j = 0; j < borrowers[i].bookCount; j++) 
        {
            printf("%s (Issue Date: %d, Return Date: %d)", borrowers[i].bookNames[j], borrowers[i].issueDates[j], borrowers[i].returnDates[j]);
            if (j < borrowers[i].bookCount - 1) 
            {
                printf(", ");
            }
        }
        printf("\n");
    }
}












// Delete a book from the list
void deleteBook() 
{
    char bookName[50];
    printf("Enter the name of the book to delete: ");
    scanf(" %[^\n]", bookName);

    int found = -1;
    for (int i = 0; i < bookCount; i++) 
    {
        if (strcmp(books[i].name, bookName) == 0) 
        {
            found = i;
            break;
        }
    }

    if (found == -1) 
    {
        printf("Book not found.\n");
        return;
    }

    for (int i = found; i < bookCount - 1; i++) 
    {
        books[i] = books[i + 1];
    }
    bookCount--;
    saveRecords();
    printf("Book deleted successfully!\n");
}













// Edit a book's information
void editBook() 
{
    char bookName[50];
    printf("Enter the name of the book to edit: ");
    scanf(" %[^\n]", bookName);

    int found = -1;
    for (int i = 0; i < bookCount; i++) 
    {
        if (strcmp(books[i].name, bookName) == 0) 
        {
            found = i;
            break;
        }
    }

    if (found == -1) 
    {
        printf("Book not found.\n");
        return;
    }

    printf("Enter new book name: ");
    scanf(" %[^\n]", books[found].name);
    printf("Enter new author name: ");
    scanf(" %[^\n]", books[found].author);
    printf("Enter new category: ");
    scanf(" %[^\n]", books[found].category);
    printf("Enter new quantity: ");
    scanf("%d", &books[found].quantity);
    saveRecords();
    printf("Book information updated successfully!\n");
}












// Delete a borrower from the list
void deleteBorrower() 
{
    char borrowerName[50];
    printf("Enter the name of the borrower to delete: ");
    scanf(" %[^\n]", borrowerName);

    int found = -1;
    for (int i = 0; i < borrowerCount; i++) 
    {
        if (strcmp(borrowers[i].name, borrowerName) == 0) 
        {
            found = i;
            break;
        }
    }

    if (found == -1) 
    {
        printf("Borrower not found.\n");
        return;
    }

    for (int i = found; i < borrowerCount - 1; i++) 
    {
        borrowers[i] = borrowers[i + 1];
    }
    borrowerCount--;
    saveRecords();
    printf("Borrower deleted successfully!\n");
}











// Edit a borrower's information
void editBorrower() 
{
    char borrowerName[50];
    printf("Enter the name of the borrower to edit: ");
    scanf(" %[^\n]", borrowerName);

    int found = -1;
    for (int i = 0; i < borrowerCount; i++) 
    {
        if (strcmp(borrowers[i].name, borrowerName) == 0) 
        {
            found = i;
            break;
        }
    }

    if (found == -1) 
    {
        printf("Borrower not found.\n");
        return;
    }

    printf("Enter new borrower name: ");
    scanf(" %[^\n]", borrowers[found].name);
    printf("Enter new contact number: ");
    scanf(" %[^\n]", borrowers[found].contact);
    saveRecords();
    printf("Borrower information updated successfully!\n");
}










// Clear all records
void clearRecords() 
{
    // Reset global variables
    bookCount = 0;
    borrowerCount = 0;

    // Clear the files
    FILE *fp;
    fp = fopen(bookFile, "wb");
    if (fp != NULL) 
    {
        fwrite(&bookCount, sizeof(int), 1, fp);
        fclose(fp);
    }

    fp = fopen(borrowerFile, "wb");
    if (fp != NULL) 
    {
        fwrite(&borrowerCount, sizeof(int), 1, fp);
        fclose(fp);
    }

    printf("All records have been cleared.\n");
}












// Main function
int main() 
{
    int choice;

    loadRecords(); // Load existing records from files

    while (1) 
    {
        printf("\n--- Library Management System ---\n");
        printf("1. Add Book\n");
        printf("2. Search Book\n");
        printf("3. Issue Book\n");
        printf("4. Return Book\n");
        printf("5. Calculate Fine\n");
        printf("6. Display Books\n");
        printf("7. Display Borrowers\n");
        printf("8. Delete Book\n");
        printf("9. Edit Book\n");
        printf("10. Delete Borrower\n");
        printf("11. Edit Borrower\n");
        printf("12. Clear All Records\n");
        printf("13. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) 
        {
            case 1:
                addBook();
                break;
            case 2:
                searchBook();
                break;
            case 3:
                issueBook();
                break;
            case 4:
                returnBook();
                break;
            case 5:
                calculateFine();
                break;
            case 6:
                displayBooks();
                break;
            case 7:
                displayBorrowers();
                break;
            case 8:
                deleteBook();
                break;
            case 9:
                editBook();
                break;
            case 10:
                deleteBorrower();
                break;
            case 11:
                editBorrower();
                break;
            case 12:
                clearRecords();
                break;
            case 13:
                printf("Exiting the system. Goodbye!\n");
                saveRecords(); // Save records before exiting
                exit(0);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}