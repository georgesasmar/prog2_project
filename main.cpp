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

int generateUniqueID() {
    return time(nullptr) % 1000000 + rand() % 10000;
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
 


int main() {
    
}


