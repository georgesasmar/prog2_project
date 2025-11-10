#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include "json.hpp" // Download from https://github.com/nlohmann/json

using namespace std;
using json = nlohmann::json;

// ==================== DATA STRUCTURES ====================
struct User {
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
    
    Course() : instructorIDs(nullptr), instructorCount(0) {}
    
    ~Course() {
        if (instructorIDs != nullptr) {
            delete[] instructorIDs;
        }
    }
};

struct Grade {
    string courseCode;
    int studentID;
    int instructorID;
    double grade;
};

// ==================== GLOBAL VARIABLES ====================
const string USERS_FILE = "data/users.json";
const string COURSES_FILE = "data/courses.json";
const string GRADES_FILE = "data/grades.json";

User currentUser;

// ==================== UTILITY FUNCTIONS ====================

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

bool isValidEmail(const string& email) {
    const regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return regex_match(email, pattern);
}

bool isValidPhone(const string& phone) {
    const regex pattern(R"(^[\d\s\+\-\(\)]{8,15}$)");
    return regex_match(phone, pattern);
}

int generateUniqueID() {
    return time(nullptr) % 1000000 + rand() % 10000;
}

void ensureDirectoryExists() {
    #ifdef _WIN32
        system("if not exist data mkdir data");
        system("if not exist reports mkdir reports");
    #else
        system("mkdir -p data");
        system("mkdir -p reports");
    #endif
}

// ==================== FILE OPERATIONS ====================

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
    try {
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
        j.push_back(userObj);
    }
    
    ofstream file(USERS_FILE);
    file << j.dump(4);
    file.close();
}

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

// ==================== HELPER FUNCTIONS ====================

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

// ==================== AUTHENTICATION ====================

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
    
    newUser.id = generateUniqueID();
    
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

// ==================== ADMIN FUNCTIONS ====================

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

void removeCourse() {
    string courseCode;
    
    cout << "\n=== REMOVE COURSE ===\n";
    cout << "Enter course code to remove: ";
    cin >> courseCode;
    
    int count;
    Course* courses = loadCourses(count);
    
    if (courses == nullptr) {
        cout << "No courses found!\n";
        return;
    }
    
    int foundIndex = -1;
    for (int i = 0; i < count; i++) {
        if (courses[i].courseCode == courseCode) {
            foundIndex = i;
            break;
        }
    }
    
    if (foundIndex == -1) {
        cout << "✗ Course not found!\n";
        delete[] courses;
        return;
    }
    
    Course* newCourses = new Course[count - 1];
    int newIndex = 0;
    
    for (int i = 0; i < count; i++) {
        if (i != foundIndex) {
            newCourses[newIndex].courseCode = courses[i].courseCode;
            newCourses[newIndex].courseName = courses[i].courseName;
            newCourses[newIndex].instructorCount = courses[i].instructorCount;
            
            if (courses[i].instructorCount > 0) {
                newCourses[newIndex].instructorIDs = new int[courses[i].instructorCount];
                for (int k = 0; k < courses[i].instructorCount; k++) {
                    newCourses[newIndex].instructorIDs[k] = courses[i].instructorIDs[k];
                }
            }
            newIndex++;
        }
    }
    
    saveCourses(newCourses, count - 1);
    
    delete[] courses;
    delete[] newCourses;
    
    cout << "✓ Course removed successfully!\n";
}

void modifyCourse() {
    string courseCode;
    
    cout << "\n=== MODIFY COURSE ===\n";
    cout << "Enter course code to modify: ";
    cin >> courseCode;
    
    int count;
    Course* courses = loadCourses(count);
    
    if (courses == nullptr) {
        cout << "No courses found!\n";
        return;
    }
    
    int foundIndex = -1;
    for (int i = 0; i < count; i++) {
        if (courses[i].courseCode == courseCode) {
            foundIndex = i;
            break;
        }
    }
    
    if (foundIndex == -1) {
        cout << "✗ Course not found!\n";
        delete[] courses;
        return;
    }
    
    cin.ignore();
    cout << "Enter new course name (current: " << courses[foundIndex].courseName << "): ";
    getline(cin, courses[foundIndex].courseName);
    
    cout << "Enter new number of instructors (current: " << courses[foundIndex].instructorCount << "): ";
    int newCount;
    cin >> newCount;
    
    delete[] courses[foundIndex].instructorIDs;
    courses[foundIndex].instructorCount = newCount;
    courses[foundIndex].instructorIDs = new int[newCount];
    
    for (int i = 0; i < newCount; i++) {
        cout << "Enter instructor ID " << (i + 1) << ": ";
        cin >> courses[foundIndex].instructorIDs[i];
    }
    
    saveCourses(courses, count);
    delete[] courses;
    
    cout << "✓ Course modified successfully!\n";
}

void generateCourseStats() {
    string courseCode;
    
    cout << "\n=== COURSE STATISTICS ===\n";
    cout << "Enter course code: ";
    cin >> courseCode;
    
    if (!courseExists(courseCode)) {
        cout << "✗ Course not found!\n";
        return;
    }
    
    int gradeCount;
    Grade* grades = loadGrades(gradeCount);
    
    if (grades == nullptr) {
        cout << "No grades found!\n";
        return;
    }
    
    int passed = 0, failed = 0, totalStudents = 0;
    double sum = 0, highest = -1, lowest = 101;
    
    for (int i = 0; i < gradeCount; i++) {
        if (grades[i].courseCode == courseCode) {
            totalStudents++;
            sum += grades[i].grade;
            
            if (grades[i].grade >= 50) passed++;
            else failed++;
            
            if (grades[i].grade > highest) highest = grades[i].grade;
            if (grades[i].grade < lowest) lowest = grades[i].grade;
        }
    }
    
    delete[] grades;
    
    if (totalStudents == 0) {
        cout << "No grades recorded for this course!\n";
        return;
    }
    
    double average = sum / totalStudents;
    
    cout << "\n--- Statistics for " << courseCode << " ---\n";
    cout << "Total students: " << totalStudents << "\n";
    cout << "Passed: " << passed << "\n";
    cout << "Failed: " << failed << "\n";
    cout << "Average grade: " << fixed << setprecision(2) << average << "\n";
    cout << "Highest grade: " << highest << "\n";
    cout << "Lowest grade: " << lowest << "\n";
    
    string courseName = getCourseNameByCode(courseCode);
    string filename = "reports/course_" + courseCode + "_stats.txt";
    ofstream report(filename);
    
    report << "COURSE STATISTICS REPORT\n";
    report << "========================\n\n";
    report << "Course Code: " << courseCode << "\n";
    report << "Course Name: " << courseName << "\n";
    time_t now = time(0);
    report << "Generated on: " << ctime(&now) << "\n";
    report << "Total Students: " << totalStudents << "\n";
    report << "Passed: " << passed << "\n";
    report << "Failed: " << failed << "\n";
    report << "Average Grade: " << fixed << setprecision(2) << average << "\n";
    report << "Highest Grade: " << highest << "\n";
    report << "Lowest Grade: " << lowest << "\n";
    
    report.close();
    
    cout << "\n✓ Report saved to: " << filename << "\n";
}

void adminMenu() {
    int choice;
    
    do {
        cout << "\n=== ADMINISTRATOR MENU ===\n";
        cout << "1. Add Course\n";
        cout << "2. Remove Course\n";
        cout << "3. Modify Course\n";
        cout << "4. Generate Course Statistics\n";
        cout << "5. Logout\n";
        cout << "Choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1: addCourse(); break;
            case 2: removeCourse(); break;
            case 3: modifyCourse(); break;
            case 4: generateCourseStats(); break;
            case 5: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 5);
}

// ==================== STUDENT FUNCTIONS ====================

void getStudentGrade() {
    string courseCode;
    
    cout << "\n=== GET GRADE ===\n";
    cout << "Enter course code: ";
    cin >> courseCode;
    
    int gradeCount;
    Grade* grades = loadGrades(gradeCount);
    
    if (grades == nullptr) {
        cout << "No grades found!\n";
        return;
    }
    
    bool found = false;
    for (int i = 0; i < gradeCount; i++) {
        if (grades[i].studentID == currentUser.id && grades[i].courseCode == courseCode) {
            string courseName = getCourseNameByCode(courseCode);
            cout << "\nCourse: " << courseCode << " - " << courseName << "\n";
            cout << "Your grade: " << grades[i].grade << "\n";
            found = true;
            break;
        }
    }
    
    delete[] grades;
    
    if (!found) {
        cout << "✗ No grade found for this course!\n";
    }
}

void generateStudentReport() {
    int gradeCount;
    Grade* grades = loadGrades(gradeCount);
    
    if (grades == nullptr) {
        cout << "No grades found!\n";
        return;
    }
    
    cout << "\n=== YOUR GRADES ===\n";
    
    double sum = 0;
    int count = 0;
    
    // Simple sorting by course code (bubble sort)
    for (int i = 0; i < gradeCount - 1; i++) {
        for (int j = 0; j < gradeCount - i - 1; j++) {
            if (grades[j].studentID == currentUser.id && 
                grades[j + 1].studentID == currentUser.id &&
                grades[j].courseCode > grades[j + 1].courseCode) {
                Grade temp = grades[j];
                grades[j] = grades[j + 1];
                grades[j + 1] = temp;
            }
        }
    }
    
    for (int i = 0; i < gradeCount; i++) {
        if (grades[i].studentID == currentUser.id) {
            string courseName = getCourseNameByCode(grades[i].courseCode);
            cout << grades[i].courseCode << " - " << courseName << ": " << grades[i].grade << "\n";
            sum += grades[i].grade;
            count++;
        }
    }
    
    if (count > 0) {
        double average = sum / count;
        cout << "\nOverall Average: " << fixed << setprecision(2) << average << "\n";
        
        string filename = "reports/student_" + to_string(currentUser.id) + "_report.txt";
        ofstream report(filename);
        
        report << "STUDENT GRADES REPORT\n";
        report << "=====================\n\n";
        report << "Student: " << currentUser.firstName << " " << currentUser.lastName << "\n";
        report << "ID: " << currentUser.id << "\n";
        time_t now = time(0);
        report << "Generated on: " << ctime(&now) << "\n";
        
        for (int i = 0; i < gradeCount; i++) {
            if (grades[i].studentID == currentUser.id) {
                string courseName = getCourseNameByCode(grades[i].courseCode);
                report << grades[i].courseCode << " - " << courseName << ": " << grades[i].grade << "\n";
            }
        }
        
        report << "\nOverall Average: " << fixed << setprecision(2) << average << "\n";
        report.close();
        
        cout << "\n✓ Report saved to: " << filename << "\n";
    } else {
        cout << "No grades found!\n";
    }
    
    delete[] grades;
}

void studentMenu() {
    int choice;
    
    do {
        cout << "\n=== STUDENT MENU ===\n";
        cout << "1. Get Grade for a Course\n";
        cout << "2. Generate Grade Report\n";
        cout << "3. Logout\n";
        cout << "Choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1: getStudentGrade(); break;
            case 2: generateStudentReport(); break;
            case 3: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 3);
}

// ==================== INSTRUCTOR FUNCTIONS ====================

void addGrade() {
    Grade newGrade;
    
    cout << "\n=== ADD GRADE ===\n";
    cout << "Enter student ID: ";
    cin >> newGrade.studentID;
    
    if (!userExists(newGrade.studentID)) {
        cout << "✗ Student not found!\n";
        return;
    }
    
    cout << "Enter course code: ";
    cin >> newGrade.courseCode;
    
    if (!courseExists(newGrade.courseCode)) {
        cout << "✗ Course not found!\n";
        return;
    }
    
    newGrade.instructorID = currentUser.id;
    
    cout << "Enter grade (0-100): ";
    cin >> newGrade.grade;
    
    if (newGrade.grade < 0 || newGrade.grade > 100) {
        cout << "✗ Invalid grade! Must be between 0 and 100.\n";
        return;
    }
    
    int gradeCount;
    Grade* grades = loadGrades(gradeCount);
    
    if (grades != nullptr) {
        for (int i = 0; i < gradeCount; i++) {
            if (grades[i].studentID == newGrade.studentID && 
                grades[i].courseCode == newGrade.courseCode) {
                cout << "✗ Grade already exists! Use modify option instead.\n";
                delete[] grades;
                return;
            }
        }
    }
    
    Grade* newGrades = new Grade[gradeCount + 1];
    
    for (int i = 0; i < gradeCount; i++) {
        newGrades[i] = grades[i];
    }
    
    newGrades[gradeCount] = newGrade;
    
    saveGrades(newGrades, gradeCount + 1);
    
    if (grades != nullptr) delete[] grades;
    delete[] newGrades;
    
    cout << "✓ Grade added successfully!\n";
}

void modifyGrade() {
    int studentID;
    string courseCode;
    
    cout << "\n=== MODIFY GRADE ===\n";
    cout << "Enter student ID: ";
    cin >> studentID;
    cout << "Enter course code: ";
    cin >> courseCode;
    
    int gradeCount;
    Grade* grades = loadGrades(gradeCount);
    
    if (grades == nullptr) {
        cout << "No grades found!\n";
        return;
    }
    
    int foundIndex = -1;
    for (int i = 0; i < gradeCount; i++) {
        if (grades[i].studentID == studentID && grades[i].courseCode == courseCode) {
            foundIndex = i;
            break;
        }
    }
    
    if (foundIndex == -1) {
        cout << "✗ Grade not found!\n";
        delete[] grades;
        return;
    }
    
    cout << "Current grade: " << grades[foundIndex].grade << "\n";
    cout << "Enter new grade (0-100): ";
    double newGrade;
    cin >> newGrade;
    
    if (newGrade < 0 || newGrade > 100) {
        cout << "✗ Invalid grade! Must be between 0 and 100.\n";
        delete[] grades;
        return;
    }
    
    grades[foundIndex].grade = newGrade;
    grades[foundIndex].instructorID = currentUser.id;
    
    saveGrades(grades, gradeCount);
    delete[] grades;
    
    cout << "✓ Grade modified successfully!\n";
}

void instructorMenu() {
    int choice;
    
    do {
        cout << "\n=== INSTRUCTOR MENU ===\n";
        cout << "1. Add Grade\n";
        cout << "2. Modify Grade\n";
        cout << "3. Logout\n";
        cout << "Choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1: addGrade(); break;
            case 2: modifyGrade(); break;
            case 3: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 3);
}

// ==================== MAIN PROGRAM ====================

int main() {
    srand(time(0));
    ensureDirectoryExists();
    
    cout << "========================================\n";
    cout << "  STUDENT GRADES MANAGEMENT SYSTEM\n";
    cout << "========================================\n";
    
    bool running = true;
    
    while (running) {
        cout << "\n=== MAIN MENU ===\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Choice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1:
                registerUser();
                // After registration, go to appropriate menu
                if (currentUser.role == "admin") {
                    adminMenu();
                } else if (currentUser.role == "student") {
                    studentMenu();
                } else if (currentUser.role == "instructor") {
                    instructorMenu();
                }
                break;
                
            case 2:
                if (loginUser()) {
                    // Direct to appropriate menu based on role
                    if (currentUser.role == "admin") {
                        adminMenu();
                    } else if (currentUser.role == "student") {
                        studentMenu();
                    } else if (currentUser.role == "instructor") {
                        instructorMenu();
                    }
                }
                break;
                
            case 3:
                cout << "\nThank you for using the system. Goodbye!\n";
                running = false;
                break;
                
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
    
    return 0;
}