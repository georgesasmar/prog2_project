#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

//structs

struct User
{
    int id;
    string firstName;
    string lastName;
    string password;
    string email;
    string phone;
    string role;

};

struct Course {
    string courseCode;
    string courseName;
    int* instructorIDs;
    int instructorCount;

};


struct Grade {
    string courseCode;
    int studentID;
    int instructorID;
    double grade;
};

//global vars

const string USERS_FILE = "data/users.json";
const string COURSES_FILE = "data/courses.json";
const string GRADES_FILE = "data/grades.json";
User currentUser;

//passwd functions

string hashPassword(const string& password) {
    hash<string> hasher;
    size_t hashValue = hasher(password);
    stringstream ss;
    ss << hex << hashValue;
    return ss.str();
}

bool isValidPassword(const string& password) {
    if (password.length() < 8) return false;
    
    bool hasDigit = false, hasLetter = false, hasSpecial = false;
    
    for (char c : password) {
        if (isdigit(c)) hasDigit = true;
        else if (isalpha(c)) hasLetter = true;
        else if (!isspace(c)) hasSpecial = true;
    }
    
    return hasDigit && hasLetter && hasSpecial;
}

//input validation

bool isValidEmail(const string& email) {
    const regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return regex_match(email, pattern);
}

bool isValidPhone(const string& phone) {
    const regex pattern(R"(^[\d\s\+\-\(\)]{8,15}$)");
    return regex_match(phone, pattern);
}

//id gen

int findlastid() {
    int count;
    User* users = loadUsers(count);

    if (users == nullptr) return 1000; 

    int maxId = 0;
    for (int i = 0; i < count; i++) {
        if (users[i].id > maxId) {
            maxId = users[i].id;
        }
    }

    delete[] users;
    return maxId;
}

int genID() {
    return findlastid() + 1;
}



//user file manip

User* loadUsers(int& count) {
    ifstream file(USERS_FILE);
    count = 0;
    
    if (!file.is_open()) {
        ofstream newFile(USERS_FILE);
        newFile << "[]";
        newFile.close();
        return nullptr;
    }
    
    json j;
    try {               //ta shayyik eza fi meshkle bl json files, syntax aw shi kermel ma ya3mel crash
        file >> j;
    } catch (...) {
        file.close();
        return nullptr;
    }
    file.close();
    
    count = j.size();
    if (count == 0) return nullptr;
    
    User* users = new User[count];
    
    for (int i = 0; i < count; i++) {
        users[i].id = j[i]["id"];
        users[i].firstName = j[i]["firstName"];
        users[i].lastName = j[i]["lastName"];
        users[i].password = j[i]["password"];
        users[i].email = j[i]["email"];
        users[i].phone = j[i]["phone"];
        users[i].role = j[i]["role"];
    }
    
    return users;
}

void saveUsers(User* users, int count) {
    json j = json::array();
    
    for (int i = 0; i < count; i++) {
        json userObj;
        userObj["id"] = users[i].id;
        userObj["firstName"] = users[i].firstName;
        userObj["lastName"] = users[i].lastName;
        userObj["password"] = users[i].password;
        userObj["email"] = users[i].email;
        userObj["phone"] = users[i].phone;
        userObj["role"] = users[i].role;
        j.push_back(userObj);   //pushback kermel zid kaza users tahet ba3d [{user 1}, {user 2}]
    }
    
    ofstream file(USERS_FILE);
    file << j.dump(4);          //kermel l json ykun fiyo formatting mrattab w readable, 4 yaane indention of 4
    file.close();
}


//course file manip 

Course* loadCourses(int& count) {
    ifstream file(COURSES_FILE);
    count = 0;
    
    if (!file.is_open()) {
        ofstream newFile(COURSES_FILE);
        newFile << "[]";
        newFile.close();
        return nullptr;
    }
    
    json j;
    try {
        file >> j;
    } catch (...) {
        file.close();
        return nullptr;
    }
    file.close();
    
    count = j.size();
    if (count == 0) return nullptr;
    
    Course* courses = new Course[count];
    
    for (int i = 0; i < count; i++) {
        courses[i].courseCode = j[i]["courseCode"];
        courses[i].courseName = j[i]["courseName"];
        
        json instructors = j[i]["instructorIDs"];
        courses[i].instructorCount = instructors.size();
        
        if (courses[i].instructorCount > 0) {
            courses[i].instructorIDs = new int[courses[i].instructorCount];
            for (int k = 0; k < courses[i].instructorCount; k++) {
                courses[i].instructorIDs[k] = instructors[k];
            }
        }
    }
    
    return courses;
}
 
void saveCourses(Course* courses, int count) {
    json j = json::array();
    
    for (int i = 0; i < count; i++) {
        json courseObj;
        courseObj["courseCode"] = courses[i].courseCode;
        courseObj["courseName"] = courses[i].courseName;
        
        json instructorsArray = json::array();
        for (int k = 0; k < courses[i].instructorCount; k++) {
            instructorsArray.push_back(courses[i].instructorIDs[k]);
        }
        courseObj["instructorIDs"] = instructorsArray;
        
        j.push_back(courseObj);
    }
    
    ofstream file(COURSES_FILE);
    file << j.dump(4);
    file.close();
}

//grade file manip

Grade* loadGrades(int& count) {
    ifstream file(GRADES_FILE);
    count = 0;
    
    if (!file.is_open()) {
        ofstream newFile(GRADES_FILE);
        newFile << "[]";
        newFile.close();
        return nullptr;
    }
    
    json j;
    try {
        file >> j;
    } catch (...) {
        file.close();
        return nullptr;
    }
    file.close();
    
    count = j.size();
    if (count == 0) return nullptr;
    
    Grade* grades = new Grade[count];
    
    for (int i = 0; i < count; i++) {
        grades[i].courseCode = j[i]["courseCode"];
        grades[i].studentID = j[i]["studentID"];
        grades[i].instructorID = j[i]["instructorID"];
        grades[i].grade = j[i]["grade"];
    }
    
    return grades;
}


void saveGrades(Grade* grades, int count) {
    json j = json::array();
    
    for (int i = 0; i < count; i++) {
        json gradeObj;
        gradeObj["courseCode"] = grades[i].courseCode;
        gradeObj["studentID"] = grades[i].studentID;
        gradeObj["instructorID"] = grades[i].instructorID;
        gradeObj["grade"] = grades[i].grade;
        j.push_back(gradeObj);
    }
    
    ofstream file(GRADES_FILE);
    file << j.dump(4);
    file.close();
}


// exists ?


bool userExists(int id) {
    int count;
    User* users = loadUsers(count);
    
    if (users == nullptr) return false;
    
    for (int i = 0; i < count; i++) {
        if (users[i].id == id) {
            delete[] users;
            return true;
        }
    }
    
    delete[] users;
    return false;
}


bool courseExists(const string& courseCode) {
    int count;
    Course* courses = loadCourses(count);
    
    if (courses == nullptr) return false;
    
    for (int i = 0; i < count; i++) {
        if (courses[i].courseCode == courseCode) {
            delete[] courses;
            return true;
        }
    }
    
    delete[] courses;
    return false;
}


//course name

string getCourseNameByCode(const string& courseCode) {
    int count;
    Course* courses = loadCourses(count);
    
    if (courses == nullptr) return "";
    
    for (int i = 0; i < count; i++) {
        if (courses[i].courseCode == courseCode) {
            string name = courses[i].courseName;
            delete[] courses;
            return name;
        }
    }
    
    delete[] courses;
    return "";
}


//registration and login

void registerUser() {
    User newUser;
    
    cout << "\n=== USER REGISTRATION ===\n";
    
    cout << "Enter first name: ";
    cin >> newUser.firstName;
    
    cout << "Enter last name: ";
    cin >> newUser.lastName;
    
    string password;
    do {
        cout << "Enter password (min 8 chars, must include numbers, letters, and special characters): ";
        cin >> password;
        
        if (!isValidPassword(password)) {
            cout << "Invalid password! Please try again.\n";
        } else {
            break;
        }
    } while (true);
    newUser.password = hashPassword(password);
    
    do {
        cout << "Enter email: ";
        cin >> newUser.email;
        
        if (!isValidEmail(newUser.email)) {
            cout << "Invalid email format! Please try again.\n";
        } else {
            break;
        }
    } while (true);
    
    cin.ignore();
    do {
        cout << "Enter phone number: ";
        getline(cin, newUser.phone);
        
        if (!isValidPhone(newUser.phone)) {
            cout << "Invalid phone format! Please try again.\n";
        } else {
            break;
        }
    } while (true);
    
    int roleChoice;
    cout << "\nSelect role:\n";
    cout << "1. Student\n";
    cout << "2. Instructor\n";
    cout << "3. Administrator\n";
    cout << "Choice: ";
    cin >> roleChoice;
    
    switch (roleChoice) {
        case 1: newUser.role = "student"; break;
        case 2: newUser.role = "instructor"; break;
        case 3: newUser.role = "admin"; break;
        default: newUser.role = "student";
    }
    
    newUser.id = genID();
    
    int count;
    User* users = loadUsers(count);
    
    User* newUsers = new User[count + 1];
    
    for (int i = 0; i < count; i++) {
        newUsers[i] = users[i];
    }
    
    newUsers[count] = newUser;
    
    saveUsers(newUsers, count + 1);
    
    if (users != nullptr) delete[] users;
    delete[] newUsers;
    
    currentUser = newUser;
    
    cout << "\n✓ Registration successful! Your ID is: " << newUser.id << "\n";
    cout << "Please remember this ID for future logins.\n";
}



bool loginUser() {
    int id;
    string password;
    
    cout << "\n=== USER LOGIN ===\n";
    cout << "Enter your ID: ";
    cin >> id;
    cout << "Enter your password: ";
    cin >> password;
    
    string hashedPassword = hashPassword(password);
    
    int count;
    User* users = loadUsers(count);
    
    if (users == nullptr) {
        cout << "No users found!\n";
        return false;
    }
    
    for (int i = 0; i < count; i++) {
        if (users[i].id == id && users[i].password == hashedPassword) {
            currentUser = users[i];
            delete[] users;
            cout << "\n✓ Login successful! Welcome " << currentUser.firstName << "!\n";
            return true;
        }
    }
    
    delete[] users;
    cout << "\n✗ Invalid ID or password!\n";
    return false;
}


//admin stuff

void addCourse() {
    Course newCourse;
    
    cout << "\n=== ADD COURSE ===\n";
    cout << "Enter course code: ";
    cin >> newCourse.courseCode;
    
    if (courseExists(newCourse.courseCode)) {
        cout << "✗ Course already exists!\n";
        return;
    }
    
    cin.ignore();
    cout << "Enter course name: ";
    getline(cin, newCourse.courseName);
    
    cout << "Enter number of instructors: ";
    cin >> newCourse.instructorCount;
    
    newCourse.instructorIDs = new int[newCourse.instructorCount];
    
    for (int i = 0; i < newCourse.instructorCount; i++) {
        cout << "Enter instructor ID " << (i + 1) << ": ";
        cin >> newCourse.instructorIDs[i];
        
        if (!userExists(newCourse.instructorIDs[i])) {
            cout << "Warning: Instructor ID " << newCourse.instructorIDs[i] << " does not exist!\n";
        }
    }
    
    int count;
    Course* courses = loadCourses(count);
    
    Course* newCourses = new Course[count + 1];
    
    for (int i = 0; i < count; i++) {
        newCourses[i].courseCode = courses[i].courseCode;
        newCourses[i].courseName = courses[i].courseName;
        newCourses[i].instructorCount = courses[i].instructorCount;
        
        if (courses[i].instructorCount > 0) {
            newCourses[i].instructorIDs = new int[courses[i].instructorCount];
            for (int k = 0; k < courses[i].instructorCount; k++) {
                newCourses[i].instructorIDs[k] = courses[i].instructorIDs[k];
            }
        }
    }
    
    newCourses[count].courseCode = newCourse.courseCode;
    newCourses[count].courseName = newCourse.courseName;
    newCourses[count].instructorCount = newCourse.instructorCount;
    newCourses[count].instructorIDs = new int[newCourse.instructorCount];
    for (int k = 0; k < newCourse.instructorCount; k++) {
        newCourses[count].instructorIDs[k] = newCourse.instructorIDs[k];
    }
    
    saveCourses(newCourses, count + 1);
    
    if (courses != nullptr) delete[] courses;
    delete[] newCourses;
    
    cout << "✓ Course added successfully!\n";
}


int main() {
    
}


