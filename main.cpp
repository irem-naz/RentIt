#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Renter;
class Rentee;
class User;
class House;
vector<Renter *> renters;
vector<Rentee *> rentees;
vector<User *> users;
vector<House *> houses;

template <typename T> inline void answerMe(T &x) { cin >> x; }

class User {
private:
  // needed data for each user
  string name;
  string email;
  string phoneNum;
  friend class Admin;

public:
  // the constructor for each user
  User(string n, string e, string p) {
    name = n;
    email = e;
    phoneNum = p;
  }
  // information on each users data
  string getName() { return name; }
  string getEmail() { return email; }
  string getPhone() { return phoneNum; }
  void getInfo() {
    cout << "Name: " << name << "\nEmail: " << email
         << "\nPhone No: " << phoneNum << endl;
  }
};

class House {
private:
  // the variable for the available dates of each stored in a vector
  vector<int> dateA;
  // location and the owner name are necessary for constructing the house
  string location, ownerName;
  // the renter class can make changes to the details of their house
  friend class Renter;
  // checking whether the inputted dates are valid (2023 onwards date are
  // accepted)
  bool dateValidityChecker(int d, int m, int y) {
    //day=d, month=h, year=y
    if (y > 2022) {
      if ((m == 2 && (d < 29 && d > 0)) ||
          ((m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 ||
            m == 12) &&
           (d < 32 && d > 0)) ||
          ((m == 4 || m == 6 || m == 9 || m == 11) && (d < 31 && d > 0))) {
        dateA.push_back(d);
        dateA.push_back(m);
        dateA.push_back(y);
        cout << "succesful!\n";
        return false;
      } else {
        cout << "invalid input, try again.\n";
        return true;
      }
    } else {
      cout << "invalid input, try again.\n";
      return true;
    }
  }
  // setting a start date for the house availability
  void setStartDates() {
    int d, m, y;
    bool corr = true;
    while (corr) {
      cin >> d >> m >> y;
      corr = dateValidityChecker(d, m, y);
    }
  }
  // setting an end date for the house availability
  void setEndDates() {
    int d, m, y;
    bool corr = true;
    while (corr) {
      cin >> d >> m >> y;
      if (dateA[dateA.size() - 1] == y) {
        if (dateA[dateA.size() - 2] == m) {
          if (dateA[dateA.size() - 3] < d) {
            corr = dateValidityChecker(d, m, y);
          } else
            cout << "invalid input, try again.\n";
        } else if (dateA[dateA.size() - 2] < m) {
          corr = dateValidityChecker(d, m, y);
        } else
          cout << "invalid input, try again.\n";
      } else if (dateA[dateA.size() - 1] < y) {
        corr = dateValidityChecker(d, m, y);
      } else
        cout << "invalid input, try again.\n";
    }
  }
  // constructing a house
  House(string n, string l) {
    ownerName = n;
    location = l;
    cout << "Beginning day, month, year. Year must be 2023 and onwards. Both day and month must be written without a preceding 0.\n";
    setStartDates();
    cout << "Ending day, month, year. Year must be 2023 and onwards. Both day and month must be written without a preceding 0.\n";
    setEndDates();
  }
  // constructor only used to clasify the houses of the renters stored in the
  // users.txt file
  House(string n, string l, int d, int m, int y, int d2, int m2, int y2) {
    ownerName = n;
    location = l;
    dateA.push_back(d);
    dateA.push_back(m);
    dateA.push_back(y);
    dateA.push_back(d2);
    dateA.push_back(m2);
    dateA.push_back(y2);
  }

public:
  // the public placebo constructor for house
  House(string n) : ownerName(n){};
  // function used to make bookings for period of time called by the rentee
  void setDateExceptions() {
    cout << "Year must be 2023 and onwards. Both day and month must be written without a preceding 0. Start dates:\n";
    setStartDates();
    cout << "Year must be 2023 and onwards. Both day and month must be written without a preceding 0. End dates:\n";
    setEndDates();
  }
  // getters for the house class variables
  string getLocation() { return location; }
  string getOwner() { return ownerName; }
  // provides information regarding the house, including owner, location, and
  // the dates available
  void getInfo() {
    cout << "Owner name " << ownerName << ", at the location " << location
         << " in Abu Dhabi.";
    getAvailability();
  }
  // prints the dates which the house was set on the market for as well as the
  // periods it is booked till now
  void getAvailability() {
    cout << " The house is available between the dates:\n ";
    for (int i = 0; i < 6; i += 3) {
      cout << dateA[i] << "/" << dateA[i + 1] << "/" << dateA[i + 2] << " ";
    }
    cout << ",";
    if (dateA.size() > 6) {
      for (int i = 6; i < dateA.size(); i += 6) {
        cout << "\nBooked from " << dateA[i] << "/" << dateA[i + 1] << "/"
             << dateA[i + 2] << " till " << dateA[i + 3] << "/" << dateA[i + 4]
             << "/" << dateA[i + 5];
      }
    }
    cout << endl << "---------------------" << endl;
  }
};
// renters
class Renter : public User {
private:
  // renters have their own vector of pointers for the houses under their name
  int indexRenter;
  vector<House *> myHouses;

public:
  // renter constructor
  Renter(string n, string e, string p) : User(n, e, p){};
  // only used to put the houses that are given in the houseinfo.txt file into
  // the houses vector
  void addToMyHouses(string n, string l, int d, int m, int y, int d2, int m2,
                     int y2) {
    House *newHouse = new House(n, l, d, m, y, d2, m2, y2);
    houses.push_back(newHouse);
    myHouses.push_back(newHouse);
  }
  // class function that is called when the renter wants to create a new house
  // in the program
  void createHouse(int id) {
    string l;
    indexRenter = id;
    cout << "Provide location inside Abu Dhabi (only 1 word ex. Reem)\n";
    answerMe(l);
    House *newHouse = new House(renters[indexRenter]->getName(), l);
    myHouses.push_back(newHouse);
    houses.push_back(newHouse);
  }
  // printing the info of the houses under the renters name
  void getInfo() {
    for (int i = 0; i < myHouses.size(); i++) {
      cout << "The house index no: " << i << ".\n";
      myHouses[i]->getInfo();
    }
  }
};

// rentees
class Rentee : public User, public House {
private:
  // houses that the rentee has booked
  vector<House *> rentedHouses;

public:
  // the constructor of the rentee object
  Rentee(string n, string e, string p) : User(n, e, p), House(n){};
  // the class member function to rent a house as a rentee
  int rentHouse(int choice) {
    int d, m, y;
    houses[choice]->getInfo();
    cout << "Please choose dates according to availability printed above.\n";
    houses[choice]->setDateExceptions();
    cout
        << "\nYou have successfully rented the house. Here is the contact info "
           "of owner:\n\n";
    for (int i = 0; i < renters.size(); i++) {
      if (houses[choice]->getOwner() == renters[i]->getName()) {
        renters[i]->User::getInfo();
        rentedHouses.push_back(houses[choice]);
        return i;
      }
    }
    return 0;
  }
  // prints the information on the rented houses of the rentee
  void myRentedHouses() {
    for (int i = 0; i < rentedHouses.size(); i++) {
      rentedHouses[i]->getInfo();
    }
  }
};
// gets the initial data to open the program with a data base
// has information on 3 renters with each 1 property and 1 rentee
void setUpFunction() {
  ifstream infile;
  ifstream onfile;
  infile.open("user.txt", ios::in);
  // getting the user, renter, rentee info
  // integer markers at each line to show renter (1) and rentee (0)
  while (!infile.eof()) {
    int emp;
    string temp;
    infile >> emp;
    string *Arr = new string[3];
    for (int i = 0; i < 3; i++) {
      infile >> temp;
      Arr[i] = temp;
    }
    User *newUser = new User(Arr[0], Arr[1], Arr[2]);
    users.push_back(newUser);
    if (emp == 1) {
      Renter *newRenter = new Renter(Arr[0], Arr[1], Arr[2]);
      renters.push_back(newRenter);
    } else if (emp == 0) {
      Rentee *newRentee = new Rentee(Arr[0], Arr[1], Arr[2]);
      rentees.push_back(newRentee);
    }
    delete[] Arr;
  }
  infile.close();
  // getting the inital 3 house listings
  onfile.open("houseinfo.txt");
  while (!onfile.eof()) {
    string temp, emp;
    int *Arr = new int[6];
    int mp;
    onfile >> temp;
    for (int i = 0; i < renters.size(); i++) {
      if (renters[i]->getName() == temp) {
        onfile >> emp;
        for (int c = 0; c < 6; c++) {
          onfile >> mp;
          Arr[c] = mp;
        }
        renters[i]->addToMyHouses(temp, emp, Arr[0], Arr[1], Arr[2], Arr[3],
                                  Arr[4], Arr[5]);
      }
    }
    delete[] Arr;
  }
  onfile.close();
}
// admin, runs the program
class Admin {
public:
  // checks whether the user is listed in the user vector
  int checkIfExists(string nam) {
    for (int i = 0; i < users.size(); i++) {
      if (users[i]->getName() == nam) {
        cout << "user found ";
        return i;
      }
    }
    // if it does not it creates one
    string n, e, p;
    cout << "user not found. Please create a new user by inputting name, "
            "email and phone number one by one.\n";
    cin >> n >> e >> p;
    User *newUser = new User(n, e, p);
    users.push_back(newUser);
    return users.size() - 1;
  }
  // checks whether the user is a renter
  int checkIfRenter(string name, int index) {
    for (int i = 0; i < renters.size(); i++) {
      if (renters[i]->getName() == name)
        return i;
    }
    // if not makes a new renter pointer vector
    Renter *newRenter =
        new Renter(users[index]->getName(), users[index]->getEmail(),
                   users[index]->getPhone());
    renters.push_back(newRenter);
    return renters.size() - 1;
  }
  // checks whether the user is a rentee
  int checkIfRentee(string name, int index) {
    for (int i = 0; i < rentees.size(); i++) {
      if (rentees[i]->getName() == name)
        return i;
    }
    // if not makes a new rentee pointer vector
    Rentee *newRentee =
        new Rentee(users[index]->getName(), users[index]->getEmail(),
                   users[index]->getPhone());
    rentees.push_back(newRentee);
    return rentees.size() - 1;
  }
  // the main menu of the program that is run with each selection is possible
  void display() {
    setUpFunction();
    string name, answer;
    int choice, choice1, choice2, index;
    bool global = true;
    while (global) {
      // checks whether the user exists
      cout << "Please provide your name in lower case so that we can check "
              "whether you have an account.\n";
      answerMe(name);
      index = checkIfExists(name);
      // asks whether you want to continue with the renter or rentee account
      cout << "\nAre you renter (press 1) or a rentee (press 2)?";
      answerMe(choice);
      // renter account
      if (choice == 1) {
        bool gen = true;
        while (gen) {
          int id = checkIfRenter(name, index);
          // two actions that can be taken by the renter is listed
          cout << "\nDo you want to create a new house (press 1) or print "
                  "information "
                  "about your houses (press 2?)";
          bool x = true;
          answerMe(choice1);
          // a new house is listed
          if (choice1 == 1) {
            while (x) {
              renters[id]->createHouse(id);
              cout << "Do you want to add more, type no if not. Any other "
                      "input would prompt you to add more.\n";
              answerMe(answer);
              if (answer == "no")
                x = false;
            }
            cout << "\nPrinting the updated details of your house:\n";
            renters[id]->getInfo();
          }
          // the details of the renter's house is listed
          else if (choice1 == 2) {
            cout << "\nPrinting the details of your house:\n";
            renters[id]->getInfo();
            x = false;
          }
          // option to get out of the renter account
          cout << "\nWould you like to log off from the renter account? Please "
                  "type "
                  "yes if so. Any other input would redirect you to renter "
                  "menu.\n";
          answerMe(answer);
          if (answer == "yes")
            gen = false;
        }
      }
      // continuing as a rentee
      else if (choice == 2) {
        int i = checkIfRentee(name, index);
        bool curr = true;
        while (curr) {
          cout << "\nDo you want to check the houses you rented (press 0) or "
                  "rent a new house (press 1)?";
          answerMe(choice1);
          switch (choice1) {
          // Seeing all the available houses and renting the house that is
          // available
          case 1:
            cout << "\nHere is the list of all the houses available.\n";
            for (int m = 0; m < houses.size(); m++) {
              cout << "-------------------------------------" << endl
                   << m << " is the house index\n";
              houses[m]->getInfo();
            }
            cout << "-------------------------------------" << endl;
            // renting the house
            cout << "Plese input the index of the house you want to rent. It "
                    "is written above each individual listing of the house.";
            answerMe(choice2);
            int ind;
            ind = rentees[i]->rentHouse(choice2);
            break;
          // just checking the houses that are renteed by the rentee already
          case 0:
            cout << "\nHere is the list of your rented houses and details.\n";
            rentees[i]->myRentedHouses();
            break;
          }
          // Option to log off from rentee account
          cout << "\n\nDo you want to look for houses again? If not type no. "
                  "Any other key will keep you logged in to the rentee account "
                  "prompting back to the rentee menu.\n";
          answerMe(answer);
          if (answer == "no")
            curr = false;
        }
      }
      // option to log off from the program
      cout << "\n\nDo you want to quit the program? Type yes to quit, any "
              "other input will make the program re-run.\n";
      answerMe(answer);
      cout << endl << endl;
      if (answer == "yes")
        global = false;
    }
  }
};

int main() {
  // pointer to an object from Admin class runs the program
  Admin *ad;
  ad->display();
  // the admin pointer and the vectors are emptied out
  delete ad;
  for (User *user : users)
    delete user;
  users.clear();
  for (House *house : houses)
    delete house;
  houses.clear();
}