// Tessa Ashlin (tea230000)
// A program that reads an auditorium file and presents users with a menu that allows them to
// select seats from that auditorium. If their desired seats are not available, it will present them
// with the next best option, based on where the middle of the auditorium is and what seats are still
// available. Lastly, the program generates a report on the earnings of the auditorium and puts
// the updated auditorium into a file.

#include <iostream>
#include <string>
#include<fstream>
#include <iomanip>
#include <memory>
#include <cmath>
using namespace std;

// Struct storing seating information for each row

struct node{
    int row;
    int seat;
    char type;
    node* next;
};

// Struct that stores size information about the auditorium

struct auditorium{
    int columns;
    int rows;
};

// Struct storing information about the best seating in the auditorium

struct bestSeats{
    int row;
    char seat;
    int seat_num;
    bool seatsAvailable = 0;
};

// Constant values for the price of each ticket type

const float ADULT_PRICE = 10.0;
const float CHILD_PRICE = 5.0;
const float SENIOR_PRICE = 7.5;

// Function prototypes

auditorium getSize(string filename);
void displayMenu (node* *arr, auditorium size);
void displayAuditorium(node* *arr, auditorium size);
void reserveSeats (node* *arr, int userCol_num, int userRow, int adultTickets, int seniorTickets,
                    int childTickets, auditorium size);
bool validateSeats (node* *arr, int userRow, int userCol_num, int totalTickets, auditorium size);
void generateReport (node* *arr, auditorium size);
bestSeats findBestSeats (node* *arr, auditorium size, int totalTickets);

int main()
{
    // Get the file from the user

    cout << "Enter the file name: ";
    string filename;
    cin >> filename;

    // Open the file, and do not proceed if the file cannot open
    
    ifstream inputFile;
    inputFile.open(filename);

    // Store the auditorium information in a linked list

    if(inputFile)
    {
        // Get the size of the auditorium

        auditorium size = getSize(filename);

        // Create the list

        node* *arr;
        arr = new node* [size.rows];
        
        // Loop through and assign each head pointer to nullptr

        for (int i = 0; i < size.rows; i++)
        {
            arr[i] = nullptr;
        }

        // Make nodes for the list corresponding with seats in the auditorium rows

        for (int rowCount = 0; rowCount < size.rows; rowCount++)
        {
            for (int colCount = 0; colCount < size.columns; colCount++)
            {
                // Create a new node and assign it's row, seat, and type

                node* newNode = new node();
                newNode->row = rowCount + 1;
                newNode->seat = colCount + 1;
                inputFile >> newNode->type;
                newNode->next = nullptr;

                // If the head is a nullptr (meaning its the only node), then assign the head
                // to a new node. Otherwise, go through the entire list to find the last variable
                // and assign that node's next variable to newNode

                if (arr[rowCount] == nullptr)
                {
                    arr[rowCount] = newNode;
                }
                else
                {
                    node* cur = arr[rowCount];
                        while (cur->next != nullptr)
                        {
                            cur = cur->next;  
                        } 
                    cur->next = newNode;
                }

            } // End of for ((int rowCount = 0; rowCount < size.rows; rowCount++))
        }

        // Display the menu if the user wants to reserve seats

        int userChoice;

        do{
            cout << endl << "1. Reserve seats\n2. Exit\n\n";
            cin >> userChoice;
            cout << endl;

            if (userChoice == 1)
            {
                displayMenu(arr, size);
            }
        } while(userChoice == 1);

        // Generate a report about the auditorium's earnings and tickets

        generateReport (arr, size);

        // Write the auditorium seat information to a file

        ofstream outputFile;
        outputFile.open("A1.txt");

        cout << endl;

        // Go through the entire array

        for (int i = 0; i < size.rows; i++)
        { 
            // Set current node to the beginning
        
            node* cur = arr[i];

            // Go through each column and output the type of seat to the file

            for(int j = 0; j < size.columns; j++)
                {
                    outputFile << cur->type;

                // If the next node isn't the end, keep going

                if (cur->next != nullptr)
                {
                    cur = cur->next;
                }
            }

            // Display a new line at the end of each column to the file

            outputFile << endl;

        }

        // Close the output file

        outputFile.close();

        // Delete the linked list-- first, go through each row

        for (int i = 0; i < size.rows - 1; i++)
        {
            node* cur = arr[i];
            node* nextNode = nullptr;

            // Go through each node of each row and delete them

            while (cur->next != nullptr)
            {
                nextNode = cur->next;
                delete cur;
                cur = nextNode;
            }

            // Set the head of the row to point at nothing

            arr[i] = nullptr;
        }
    }
    else
    {
        // Display an error message if the file cannot open

        cout << filename << " could not be opened.";
    }

    // Close the input file

    inputFile.close();

    return 0;
}

void generateReport (node* *arr, auditorium size)
{

    // Initialize variables to be displayed

    int totalSeats = 0, totalTickets = 0, adultTickets = 0, childTickets = 0, seniorTickets = 0;
    float totalSales = 0;

    // Go through the entire auditorium

    for (int i = 0; i < size.rows; i++)
    {
        // Create a node to track where we are in the array
        
        node* cur = arr[i];

        // Go through each column

        for(int j = 0; j < size.columns; j++)
        {
            // Increase the amount of adult, child, and senior tickets depending on what kind of
            // ticket is assigned to the seat
            
            if (cur->type == 'A')
            {
                adultTickets++;
            }
            if (cur->type == 'C')
            {
                childTickets++;
            }
            if (cur->type == 'S')
            {
                seniorTickets++;
            }

            // If the next node isn't the end, keep going

            if (cur->next != nullptr)
            {
                cur = cur->next;
            }
        }
    }

    // Initialize total tickets by adding up the adult, senior, and child tickets

    totalTickets = adultTickets + childTickets + seniorTickets;

    // Initialize total sales by adding up each ticket type by their ticket price

    totalSales = (static_cast<float>(adultTickets) * ADULT_PRICE)
               + (static_cast<float>(childTickets) * CHILD_PRICE)
               + (static_cast<float>(seniorTickets) * SENIOR_PRICE);

    // Initialize total seats by multiplying the total columns by the total rows

    totalSeats = (size.columns * size.rows);

    // Display the values found

    cout << "\nTotal Seats: " << totalSeats << "\nTotal Tickets: " << totalTickets << "\nAdult Tickets: "
         << adultTickets << "\nChild Tickets: " << childTickets << "\nSenior Tickets: " << seniorTickets
         << "\nTotal Sales: $" << showpoint << fixed << setprecision(2) << totalSales;
}

// A function that displays a menu to the user and verifies if their selection is available

void displayMenu (node* *arr, auditorium size)
{
    displayAuditorium(arr, size);

    // Prompt the user for their row number and seat letter

    int userRow;
    char userCol;
    int userCol_num = 1;
    int adultTickets;
    int seniorTickets;
    int childTickets;
    int totalTickets;

    // Get and validate the user's row number
    
    char userRowChr;

    do
    {
        cout << endl << "Enter your row number: ";
        cin >> userRowChr;
        
        while (!isdigit(userRowChr))
        {
            cout << "You must enter a number, please try again: ";
            cin >> userRowChr;
        }
            
        // Convert string to integer

        userRow = userRowChr - '0';
            
        // Validate again

        if (userRow < 1 || userRow > size.rows)
            cout << "Incorrect row number, please try again.";
            
    } while (userRow < 1 || userRow > size.rows);

    // If the user enters a lowercase number for their seat letter, change it to an uppercase one

    cout << "Enter your seat letter: ";
    cin >> userCol;
    userCol = static_cast<char>(toupper(userCol));

    // Change the user's seat letter into a number

    for (int i = 'A'; i < userCol; i++)
    {
        userCol_num++;
    }

    // Validate the user's seat letter

    while ((userCol_num > size.columns || userCol_num < 0) || !isalpha(userCol))
    {
        cout << "Incorrect seat letter, please enter a different seat: ";
        cin >> userCol;
        userCol = static_cast<char>(toupper(userCol));

        // Reset the column number back to 1

        userCol_num = 1;

        // Change the user's seat letter into a number

        for (int i = 'A'; i < userCol; i++)
        {
            userCol_num++;
        } 

        // Debugging statements

        //cout << userCol;
        //cout << userCol_num;
    }

    // Prompt the user for their ticket types

    cout << "Enter number of adult tickets: ";
    cin >> adultTickets;

    // Validate

    while(adultTickets < 0)
    {
        cout << "Incorrect number of adult tickets, please re-enter: ";
        cin >> adultTickets;
    }

    cout << "Enter number of child tickets: ";
    cin >> childTickets;
    
    // Validate

    while(childTickets < 0)
    {
        cout << "Incorrect number of child tickets, please re-enter: ";
        cin >> childTickets;
    }

    cout << "Enter number of senior tickets: ";
    cin >> seniorTickets;

    // Validate

    while(seniorTickets < 0)
    {
        cout << "Incorrect number of senior tickets, please re-enter: ";
        cin >> seniorTickets;
    }

    // Assign variables to move through the seats

    totalTickets = adultTickets + seniorTickets + childTickets;
    bool isAvailable = 1;

    // Validate the seats
    
    isAvailable = validateSeats (arr, userRow, userCol_num, totalTickets, size);

    // Reserve the user's seats if they are available, then let them know the seats have been reserved

    if(isAvailable && totalTickets > 1)
    {
        reserveSeats(arr, userCol_num, userRow, adultTickets, seniorTickets, childTickets, size);
        cout << "\nYour seats have been reserved.\n";
    }
    else if (isAvailable)
    {
        reserveSeats(arr, userCol_num, userRow, adultTickets, seniorTickets, childTickets, size);
        cout << "\nYour seat has been reserved.\n";
    }

    // Find the next best seat if the user's seat is unavailable

    if (isAvailable == 0)
    {
        bestSeats newSeats = findBestSeats (arr, size, totalTickets);

        // If the user's seats cannot be reserved, let them know, then ask if they want the next best
        // seats

        char yesOrNo = 'N';

        if (newSeats.seatsAvailable)
        {
            if (totalTickets > 1)
            {
                cout << "\nThose seats are not available. Would you like the next best seats at "
                     << newSeats.row << newSeats.seat << " - " << newSeats.row 
                     << static_cast<char>(newSeats.seat + totalTickets - 1) << "? Enter Y or N.\n";
                cin >> yesOrNo;
            }
            else
            {
                cout << "\nThat seat is not available. Would you like the next best seat at "
                     << newSeats.row << newSeats.seat << "? Enter Y or N.\n";
                cin >> yesOrNo;
            }

            // If the user wants the new seats, reserve them. If not, do not reserve them and return to
            // the main menu

            if ((yesOrNo == 'Y' || yesOrNo == 'y') && totalTickets > 1)
            {
                reserveSeats(arr, newSeats.seat_num, newSeats.row, adultTickets, seniorTickets, 
                             childTickets,size);
                cout << "\nYour seats have been reserved.\n";
            }
            else if (yesOrNo == 'Y' || yesOrNo == 'y')
            {
                reserveSeats(arr, newSeats.seat_num, newSeats.row, adultTickets, seniorTickets, 
                             childTickets, size);
                cout << "\nYour seat has been reserved.\n";
            }
        }

    // If there are absolutely no seats available to the user, let them know

        if (newSeats.seatsAvailable == 0)
        {
            cout << "\nno seats available\n";
        }
    }
}

// A function that displays the auditorium for user viewing
// Receives: The array of node pointers, and the auditorium size
// Returns: N/A

void displayAuditorium(node* *arr, auditorium size)
{
    // Display letters above the auditorium after a space

    cout << "  ";
    char letters = 'A';

    for (int i = 0; i < size.columns; i++)
    {
        cout << letters;
        letters++;
    }

    cout << endl;

    // Go through the entire array

    for (int i = 0; i < size.rows; i++)
    { 

        // Display numbers beside the columns

        cout << i + 1 << " ";

        // Create a node to track where we are in the array
        
        node* cur = arr[i];

        // Go through each column

        for(int j = 0; j < size.columns; j++)
        {
            // If the seat is empty, display a "." symbol-- Otherwise, display a "#" symbol
            
            if(cur->type != '.')
            {
                cout << "#";
            }
            else
            {
                cout << ".";
            }

            // If the next node isn't the end, keep going

            if (cur->next != nullptr)
            {
                cur = cur->next;
            }
        }

        // Display a new line at the end of each column

        cout << endl;
    }
}

// A function that validates the seat choice of the user
// Recieves: The auditorium array, the user's row and seat choice, the total tickets they asked for,
//           and the size of the auditorium
// Returns: A true/false value noting whether or not the user's chosen seats are available

bool validateSeats (node* *arr, int userRow, int userCol_num, int totalTickets, auditorium size)
{

    // Create variables to track the first seat and the count of seats passed

    bool isAvailable = 1;
    bool firstSeat = 0;
    int count = 0;
    node* cur = arr[userRow - 1];

    // If the user asks for more seats than are on a row, do not validate them

    if (size.columns < totalTickets || size.columns - userCol_num + 1 < totalTickets)
    {
        isAvailable = 0;
    }

    // Go through the entire array until the seat is reached, then see if it's open

    while (cur->next != nullptr && isAvailable)
    {

        // Check if the first seat of the row is the user's choice 

        if (cur->seat == userCol_num)
        {
            firstSeat = 1;

            if (cur->type != '.')
            {
                 isAvailable = 0;
            }

            // If the user only wants one ticket at the first seat and it's available, return

            if (cur->type == '.' && totalTickets == 1)
            {
                return isAvailable;
            }

         }
        
        // Go to the next seat

        cur = cur->next;
        count++;

        // The seat has been found-- now check if it's available

        if ((cur->seat == userCol_num || firstSeat) && isAvailable)
        {
            while (cur->seat <= count + totalTickets && cur->next != nullptr)
            {
                // If the first seat was chosen, decrement count by one

                if (firstSeat)
                {
                    count--;
                    firstSeat = 0;
                }

                // Note if any of the chosen seats are not available

                if (cur->type != '.')
                {
                    isAvailable = 0;
                }

                //Debugging statement
                //cout << "\ntype: " << cur->type << " | seat: " << cur->row << cur->seat << "\n";

                // Go to the next seat

                cur = cur->next;
            }
        }
    }

    // Return whether or not the seat is available

    return isAvailable;
}

// A function that reserves seats in the auditorium
// Receives: The node array, the user's choice of row and seat, and the amount of adult, senior, and
//           child tickets they want to reserve
// Returns: N/A

void reserveSeats (node* *arr, int userCol_num, int userRow, int adultTickets, int seniorTickets,
                    int childTickets, auditorium size)
{

    // Create a variable to traverse the auditorium

    node* cur = arr[userRow - 1];

    // If the variable is the last of the row, reserve it

    if (userCol_num == size.columns)
    {
        // Get to the end of the row

        for (int i = 0; i < size.columns - 1; i++)
            cur = cur->next;
        
        // Reserve the seats depending on ticket type

        if (adultTickets)
            cur->type = 'A';
        if (childTickets)
            cur->type = 'C';
        if(seniorTickets)
            cur->type = 'S';
    }

    // Run through each array element

    while (cur->next != nullptr)
    {

        if (cur->seat == userCol_num)
        {
        
            // The seats have been found-- now reserve them. First, reserve the adult seats

            for (int i = 1; i <= adultTickets; i++)
            {
                cur -> type = 'A';
                
                if (cur->next != nullptr)
                    cur = cur->next;
            }

            // Next, reserve the child seats

            for (int i = 1; i <= childTickets; i++)
            {
                cur->type = 'C';

                if (cur->next != nullptr)
                    cur = cur->next;
            }

            // Finally, reserve the senior seats

            for (int i = 1; i <= seniorTickets; i++)
            {
                cur->type = 'S';

                if (cur->next != nullptr)
                    cur = cur->next;
            }
        }

        // Move to the next node

        if (cur->next != nullptr)
            cur = cur->next;
    }
}

// A function that reads an auditorium file to get its rows and columns
// Receives: The file name of the auditorium
// Returns: The auditorium struct, containing its updated rows and columns

auditorium getSize(string filename)
{
    // Create an auditorium

    auditorium size;
    size.rows = 0;
    size.columns = 0;

    // Open the file

    ifstream inputFile;
    inputFile.open(filename);

    // Read from the file to get the row and column information based on a dummy string

    string column;
    while(getline(inputFile, column))
    {
        size.columns = static_cast<int>(column.length());
        size.rows++;
    }

    // Close the file and return the auditorium size

    inputFile.close();

    return size;
}

// A function that finds the best available seats in the auditorium if the user's choice wasn't there
// Receives: The array of pointer nodes storing the auditorium, the size of the auditorium, and the
//           total number of tickets to reserve
// Returns: The new best seats, initialized with new seat values or a boolean that says the seats
//          are not available

bestSeats findBestSeats (node* *arr, auditorium size, int totalTickets)
{

    //  Make a new seat variable and a placeholder one

    bestSeats newSeats;

    double middleRow = static_cast<double>(size.rows + 1) / 2.0;
    double middleSeat = static_cast<double>(size.columns + 1) / 2.0;

    // Debugging statement

    //cout << "middle row: " << middleRow << " middle seat: " << middleSeat;

    // Check if any rows have the available seats and note that in an array of true/false values

    unique_ptr<bool []> arrayAvailable(new bool[size.rows]);
    int count = 0;

    // Go through the entire auditorium checking which rows have availability in them

    for(int i = 0; i < size.rows; i++)
    {
        // Create a node to check each row in the auditorium

        node* cur = arr[i];

        // Go through the rows

        while (cur != nullptr)
        {
            // If a seat is available, increase the count. If the next seat is unavailable,
            // set count back to 0

            if (cur->type == '.')
            {
                count++;
            }
            else
            {
                count = 0;
            }

            // If there are enough seats available, mark the row as having availability

            if (count == totalTickets)
            {
                arrayAvailable[i] = 1;
                newSeats.row = i + 1;
                
            }

            // Go to the next node

            cur = cur->next;
        }
    }

        // If there are seats available, note that

    for (int i = 0; i < size.rows; i++)
    {
        if (arrayAvailable[i])
        {
            newSeats.seatsAvailable = 1;
        }
    }

    // Only continue to find the best seats if there are seats available

    if (newSeats.seatsAvailable)
    {

        // Find the closest available seat to the middle within the available rows
        
        int count = 0;
        double currentClosest = 100.0;
        newSeats.seat_num = 1;
        newSeats.row = 1;

        for (int i = 0; i < size.rows; i++)
        {
            node* cur = arr[i];
            while (cur != nullptr)
            {
                // Increase the count if the seat is empty

                if (cur->type == '.')
                {
                    count++;   
                }

                // If there's an unavailable seat, set count back to 0

                if (cur->type != '.')
                {
                    count = 0;
                }   

                // Once the program finds an area with enough seats, calculate the distance
                // of the current closest seat and the placeholder seat

                if (count == totalTickets)
                {
                    // Set count back to 0

                    count = 0;

                    double seatDistance = sqrt((pow((middleRow - static_cast<double>((i + 1))), 2) 
                                             + pow((middleSeat - ((static_cast<double>(cur->seat)
                                             + static_cast<double>(cur->seat - totalTickets + 1))/2)), 2)));
                    
                    // Debugging statement

                    /*cout << "\nrow: " << i + 1 << " seat: " << cur->seat << " middle: "
                         << (static_cast<double>(cur->seat) + static_cast<double>(cur->seat - totalTickets + 1))/2 
                         << " distance: " << seatDistance;*/

                    // If there is a tie for distance, pick the closer row. If there is a tie for
                    // closest row, pick the lesser row

                    if (currentClosest == seatDistance)
                    {
                        // Debug statement

                        //cout << " TIE ";

                        double currentRowDistance = (middleRow - (newSeats.row));
                        double heldRowDistance = (middleRow - (i + 1));

                        // Make sure the values aren't negative

                        if (currentRowDistance < 0)
                            currentRowDistance *= -1;
                        if (heldRowDistance < 0)
                            heldRowDistance *= -1;

                        // Debugging statement

                        //cout << "\ncurrent seats row: " << newSeats.row << " distance: " << currentRowDistance;
                        //cout << "\ncontestor seats row " << i + 1 << " distance: " << heldRowDistance << "\n";

                        // Pick the closest row

                        if (heldRowDistance < currentRowDistance)
                        {
                            newSeats.seat_num = cur->seat - totalTickets + 1;
                            newSeats.row = i + 1;   
                        }

                        // If the rows are the same distance from eachother, pick the row closer to
                        // the screen

                        if (currentRowDistance == heldRowDistance)
                        {
                            if (newSeats.row > i + 1)
                            {
                                newSeats.seat_num = cur->seat - totalTickets + 1;
                                newSeats.row = i + 1;
                            }
                        }
                    }

                    // If the current closest is a greater distance than seatDistance, then
                    // initialize the new seats to the starting seat of the group

                    if (currentClosest > seatDistance)
                    {
                        newSeats.seat_num = cur->seat - totalTickets + 1;
                        newSeats.row = i + 1;
                        currentClosest = seatDistance;
                    }
                }

              // Move to the next seat

              cur = cur->next;
            }
        }
    }

    // Initialize the seat column letter according to the chosen seat number

    newSeats.seat = 'A' + static_cast<char>(newSeats.seat_num - 1);

    return newSeats;

}

