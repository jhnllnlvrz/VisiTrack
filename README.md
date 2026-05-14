# VisiTrack - System for Effecient Logging and Tracking of School Visitors
Developed by John Allen P. Alvarez | CS1203 | Data Structures & Algorithms


## 📚 Project Overview
VisiTrack is a terminal-based School Visitor Logging System written in C++. It allows school administrators or security personnel to track visitors going in and out of the campus. The system provides functionalities to log visitor entries (Time In), record their exits (Time Out), view all recorded logs, search for specific visitors, and delete records. The program ensures data persistence by saving and loading records from a text file (`visitors.txt`).

## 💻 Features
- **Automatic Timestamping:** The system automatically captures the current system date and time when a visitor times in or times out.
- **Persistent Storage:** Visitor data is automatically saved to and loaded from a text file (`visitors.txt`), ensuring that records are not lost when the program is closed.
- **Time In / Time Out Management:** Easily register new visitors and securely log their departure times.
- **Search and View:** Search for specific visitors by name or view the complete list of visitor records.
- **Data Deletion:** Remove specific visitor records using their unique ID with a safety confirmation prompt.
- **Memory Management:** Dynamically allocates memory for new visitors and properly deallocates the entire list upon exit to prevent memory leaks.
- **Input Validation:** Includes contact number format checking and robust menu handling to prevent crashes from invalid inputs.
- **Smart Search:** Supports case-insensitive and partial name matching (e.g., searching "allen" will find "John Allen Alvarez").

## ⚙️ Data Structures Implemented
The core data structure used in this project is a **Singly Linked List**. 

Each node in the linked list represents a `Visitor` and is defined by a `struct` that contains:
- **Attributes:** 
  - `id` (Integer) - Unique identifier for the visitor.
  - `name`, `contact`, `purpose`, `dateIn`, `timeIn`, `timeOut` (Strings) - Visitor details.
- **Pointer:** 
  - `next` (`Visitor*`) - A pointer to the next visitor node in the list.

The program uses a global `head` pointer to keep track of the beginning of the linked list and a `nextId` counter to ensure every new visitor receives a unique identifier that never repeats, even after records are deleted.

## 🗳️ Core Functions
- `getCurrentTimestamp(string format)`: Generates the current system date or time based on the specified format.
- `loadFromFile()`: Reads visitor data from `visitors.txt` during the program startup and rebuilds the linked list in memory.
- `saveToFile()`: Traverses the linked list and writes all current visitor records to `visitors.txt`.
- `timeIn()`: Prompts the user for visitor details, automatically assigns an ID and timestamp, creates a new node, adds it to the end of the linked list, and saves the updated list to the file.
- `timeOut()`: Searches for a visitor by name (supporting partial matches) who hasn't timed out yet and automatically stamps their `timeOut` field.
- `viewRecords()`: Iterates through the linked list to display all visitor records, clearly marking visitors who are "Still Inside."
- `deleteRecord()`: Prompts for a visitor ID and asks for a 'y/n' confirmation before securely unlinking and deleting the node.
- `searchRecord()`: Uses case-insensitive matching to find and display all visitors whose names contain the search keyword.
- `containsIgnoreCase()`: A helper function that converts strings to lowercase to enable flexible searching.
- `isValidContact()`: Validates that contact numbers contain only digits and valid symbols (+, -, parenthesis), rejecting letters or invalid characters.

## 🎮 Workflow
1. **Initialization:** Upon execution, `main()` calls `loadFromFile()`. If `visitors.txt` exists, it reads the data and reconstructs the Linked List in memory.
2. **Main Menu:** The user is presented with a continuously looping menu (1 to 6) asking for an action.
3. **Execution of Actions:**    - The system validates inputs (like contact numbers and menu choices) to ensure the program remains stable even with incorrect user input.
    - When adding or modifying a record (Time In, Time Out, Delete), the Linked List in memory is updated first.
    - Immediately after modifying the memory, `saveToFile()` is called to overwrite the text file with the updated linked list, ensuring data is never lost.
4. **Termination:** When the user selects `Exit (6)`, the program iterates through the entire linked list to `delete` all dynamically allocated nodes, freeing up memory before terminating safely.

## 🖲️ Technical Details
- **Language:** C++
- **Memory Management:** Dynamic Memory Allocation (using `new` and `delete`) to handle a flexible number of visitor records.
- **Data Persistence:** File I/O (`fstream`) used to store records in `visitors.txt`.
- **Time Library:** `<ctime>` utilized for real-time data capture.
- **Platform:** Console-based application.

## How to Compile and Run
To run this project on your local machine, follow these steps:

### Prerequisites
- A C++ compiler (like `g++` or `clang++`).

### Compilation
Open your terminal and run the following command:
```bash
g++ VisiTrack.cpp -o VisiTrack
```

### Execution
After successful compilation, run the program using:
```bash
./VisiTrack
```
