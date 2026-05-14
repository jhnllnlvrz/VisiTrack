#include <iostream>  //cout & cin
#include <fstream>   //for saving the visitor data
#include <string>    //use to store a sequence of char
#include <algorithm> //for transform() used in case-insensitive search
#include <ctime>     //for automatic time in and time out

using namespace std;

struct Visitor { // create a struct named Visitor
  int id;
  string name;
  string contact;
  string purpose;
  string dateIn;
  string timeIn;
  string timeOut;

  // linked list pointer
  Visitor *next;
};

// a global head pointer
Visitor *head = nullptr;
int countRecords = 0;
int nextId = 1; // separate ID counter that never decrements
string fileName = "visitors.txt";

// automatic date in, time in record
string getCurrentTimestamp(string format) {
  time_t now = time(0);
  struct tm *tstruct = localtime(&now);
  char buf[80];

  if (format == "date") {
    strftime(buf, sizeof(buf), "%m/%d/%y", tstruct);
  } else {
    strftime(buf, sizeof(buf), "%I:%M %p", tstruct);
  }
  return string(buf);
}

// helper function for case-insensitive partial name matching
bool containsIgnoreCase(string text, string keyword) {
  transform(text.begin(), text.end(), text.begin(), ::tolower);
  transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
  return text.find(keyword) != string::npos;
}

// basic contact number validation, only allows digits, +, -, (), and spaces
bool isValidContact(const string &s) {
  if (s.length() < 7)
    return false;
  return s.find_first_not_of("0123456789+-() ") == string::npos;
}

// the load from file is to keep the data saved and persistence
void loadFromFile() {
  ifstream file(fileName);
  if (!file)
    return;

  // eof() is only set AFTER a failed read
  while (true) {
    Visitor *temp = new Visitor();
    if (!(file >> temp->id)) {
      delete temp;
      break;
    }
    file.ignore();
    getline(file, temp->name);
    getline(file, temp->contact);
    getline(file, temp->purpose);
    getline(file, temp->dateIn);
    getline(file, temp->timeIn);
    getline(file, temp->timeOut);
    temp->next = nullptr;

    // add to linked list
    if (head == nullptr) {
      head = temp;
    } else {
      Visitor *curr = head;
      while (curr->next != nullptr)
        curr = curr->next;
      curr->next = temp;
    }
    countRecords++;

    // keep nextId always ahead of the highest loaded ID
    if (temp->id >= nextId)
      nextId = temp->id + 1;
  }
  file.close();
}

// once the visitor fill up the form, it directly save to this function.
void saveToFile() {
  ofstream file(fileName);
  Visitor *curr = head;
  while (curr != nullptr) {
    file << curr->id << endl;
    file << curr->name << endl;
    file << curr->contact << endl;
    file << curr->purpose << endl;
    file << curr->dateIn << endl;
    file << curr->timeIn << endl;
    file << curr->timeOut << endl;
    curr = curr->next;
  }
  file.close();
}

// time in form function
void timeIn() {
  cout << "\n========== TIME IN ==========\n\n";

  Visitor *v = new Visitor();

  // use nextId++ for prevent collision
  v->id = nextId++;

  cout << "Full Name: ";
  cin.ignore();
  getline(cin, v->name);

  // validate contact number before accepting it 
  do {
    cout << "Contact Number: ";
    getline(cin, v->contact);
    if (!isValidContact(v->contact))
      cout << "Invalid contact number. Please enter a valid phone number.\n";
  } while (!isValidContact(v->contact));

  cout << "Purpose of Visit: ";
  getline(cin, v->purpose);

  v->dateIn = getCurrentTimestamp("date");
  v->timeIn = getCurrentTimestamp("time");
  v->timeOut = "";
  v->next = nullptr;

  // insert at the end of the list
  if (head == nullptr) {
    head = v;
  } else {
    Visitor *curr = head;
    while (curr->next != nullptr)
      curr = curr->next;
    curr->next = v;
  }

  countRecords++;
  saveToFile();

  cout << "\nRecord saved successfully at " << v->timeIn << " on " << v->dateIn
       << "!\n";
}

// time out form function
void timeOut() {
  cout << "\n========== TIME OUT ==========\n\n";

  string name;
  cout << "Enter Name: ";
  cin.ignore();
  getline(cin, name);

  bool found = false;
  Visitor *curr = head;
  while (curr != nullptr) {
    // use containsIgnoreCase() instead of exact match (==)
    if (containsIgnoreCase(curr->name, name) && curr->timeOut == "") {
      curr->timeOut = getCurrentTimestamp("time");
      found = true;
      break;
    }
    curr = curr->next;
  }

  if (found) {
    saveToFile();
    cout << "\nTime out recorded automatically!\n";
  } else {
    cout << "\nActive record not found for this name.\n";
  }
}

// for view all records of the visitors
void viewRecords() {
  cout << "\n~~~~~~~~~~ VISITOR RECORDS ~~~~~~~~~~\n\n";
  if (head == nullptr) {
    cout << "No records found.\n";
    return;
  }

  Visitor *curr = head;
  while (curr != nullptr) {
    cout << "ID: " << curr->id << endl;
    cout << "Name: " << curr->name << endl;
    cout << "Contact: " << curr->contact << endl;
    cout << "Purpose: " << curr->purpose << endl;
    cout << "Date In: " << curr->dateIn << endl;
    cout << "Time In: " << curr->timeIn << endl;
    cout << "Time Out: "
         << (curr->timeOut == "" ? "Still Inside" : curr->timeOut) << endl;
    cout << "-----------------------------\n";
    curr = curr->next;
  }
}

// for deleting visitor data
void deleteRecord() {
  cout << "\n========== DELETE RECORD ==========\n\n";
  int id;
  cout << "Enter ID to delete: ";
  cin >> id;

  if (head == nullptr)
    return;

  // ask for confirmation before permanently deleting a record
  char confirm;
  cout << "Are you sure you want to delete record #" << id << "? (y/n): ";
  cin >> confirm;
  if (confirm != 'y' && confirm != 'Y') {
    cout << "Deletion cancelled.\n";
    return;
  }

  Visitor *temp = head;
  Visitor *prev = nullptr;

  // if head node itself holds the ID to be deleted
  if (temp != nullptr && temp->id == id) {
    head = temp->next;
    delete temp;
    countRecords--;
    saveToFile();
    cout << "\nRecord deleted successfully!\n";
    return;
  }

  // search id for the deleted data of visitor
  while (temp != nullptr && temp->id != id) {
    prev = temp;
    temp = temp->next;
  }

  if (temp == nullptr) {
    cout << "\nRecord not found.\n";
    return;
  }

  // unlink the node from linked list
  prev->next = temp->next;
  delete temp;
  countRecords--;
  saveToFile();
  cout << "\nRecord deleted successfully!\n";
}

// for searching the visitor data or information
void searchRecord() {
  cout << "\n========== SEARCH VISITOR ==========\n\n";
  string searchName;
  bool found = false;
  cout << "Enter Name to search: ";
  cin.ignore();
  getline(cin, searchName);

  Visitor *curr = head;
  while (curr != nullptr) {
    // use containsIgnoreCase() for partial and case-insensitive matching
    if (containsIgnoreCase(curr->name, searchName)) {
      cout << "\nRecord Found:\n";
      cout << "ID: " << curr->id << endl;
      cout << "Name: " << curr->name << endl;
      cout << "Contact: " << curr->contact << endl;
      cout << "Purpose: " << curr->purpose << endl;
      cout << "Date In: " << curr->dateIn << endl;
      cout << "Time In: " << curr->timeIn << endl;
      cout << "Time Out: " << curr->timeOut << endl;
      cout << "-----------------------------\n";
      found = true;
    }
    curr = curr->next;
  }
  if (!found)
    cout << "\nNo record found for: " << searchName << endl;
}

// main function of the VisiTrack program
int main() {
  loadFromFile();
  int choice;
  do {
    cout << "\n========================================\n";
    cout << "     SCHOOL VISITOR LOGGING SYSTEM\n";
    cout << "========================================\n\n";
    cout << "1. Time In\n";
    cout << "2. Time Out\n";
    cout << "3. View Records\n";
    cout << "4. Delete Record\n";
    cout << "5. Search Visitor\n";
    cout << "6. Exit\n\n";
    cout << "Enter choice (1-6): ";

    if (!(cin >> choice)) {
      cin.clear();           // reset the error flags
      cin.ignore(100, '\n'); // discard the invalid input in the buffer
      choice = 0;
    }

    switch (choice) {
    case 1:
      timeIn();
      break;
    case 2:
      timeOut();
      break;
    case 3:
      viewRecords();
      break;
    case 4:
      deleteRecord();
      break;
    case 5:
      searchRecord();
      break;
    case 6:
      cout << "\nExiting...\n";
      break;
    default:
      cout << "\nInvalid choice! Please enter a number between 1-6.\n";
    }
  } while (choice != 6);

  // delete the list before exiting for not leaking
  Visitor *curr = head;
  while (curr != nullptr) {
    Visitor *next = curr->next;
    delete curr;
    curr = next;
  }

  return 0;
}