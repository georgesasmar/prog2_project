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

struct StudentGradeInfo {
    string courseCode;
    string courseName;
    double grade;
};


//global var

const string USERS_FILE = "data/users.json";
const string COURSES_FILE = "data/courses.json";
const string GRADES_FILE = "data/grades.json";
User currentUser;

//prototype cause ma badde enza3 l function structure
User *loadUsers(int& count);

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
    const regex pattern(R"(^\+961\d{7,8}$)");
    return regex_match(phone, pattern);
}

//id gen

int findLastId() {
    int count;
    User* users = loadUsers(count);

    if (users == nullptr || count == 0) return 0;

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
  
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    int year = localTime->tm_year + 1900;

    int lastId = findLastId();

   
    if (lastId / 10000 != year) {
        return year * 10000 + 1;
    }

    
    return lastId + 1;
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
    } while (true);  //+96178804389
    
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

//pdf gen

void generatePDF_StudentReport(int studentID, const string& studentName, const string& email, 
                               const string& phone, StudentGradeInfo* grades, int gradeCount, 
                               int passed, int failed, double average) {
    string filename = "reports/student_" + to_string(studentID) + "_report.pdf";
    ofstream pdf(filename, ios::binary);
    
    pdf << "%PDF-1.4\n";
    pdf << "1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n";
    pdf << "2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n";
    pdf << "3 0 obj\n<< /Type /Page /Parent 2 0 R /Resources 4 0 R /MediaBox [0 0 612 792] /Contents 5 0 R >>\nendobj\n";
    pdf << "4 0 obj\n<< /Font << /F1 << /Type /Font /Subtype /Type1 /BaseFont /Helvetica >> ";
    pdf << "/F2 << /Type /Font /Subtype /Type1 /BaseFont /Helvetica-Bold >> >> >>\nendobj\n";
    
    stringstream content;
    content << "BT\n/F2 18 Tf\n50 750 Td\n(STUDENT GRADES REPORT) Tj\n0 -30 Td\n/F1 11 Tf\n";
    content << "(Student: " << studentName << ") Tj\n0 -15 Td\n";
    content << "(ID: " << studentID << ") Tj\n0 -15 Td\n";
    content << "(Email: " << email << ") Tj\n0 -15 Td\n";
    content << "(Phone: " << phone << ") Tj\n0 -30 Td\n";
    content << "/F2 14 Tf\n(Course Grades) Tj\n0 -25 Td\n/F1 10 Tf\n";
    
    for (int i = 0; i < gradeCount; i++) {
        content << "(" << grades[i].courseCode << " - " << grades[i].courseName << ": " 
                << fixed << setprecision(2) << grades[i].grade;
        if (grades[i].grade >= 50) content << " PASSED";
        else content << " FAILED";
        content << ") Tj\n0 -15 Td\n";
    }
    
    content << "0 -20 Td\n/F2 12 Tf\n(Summary) Tj\n0 -20 Td\n/F1 11 Tf\n";
    content << "(Total: " << gradeCount << ") Tj\n0 -15 Td\n";
    content << "(Passed: " << passed << ") Tj\n0 -15 Td\n";
    content << "(Failed: " << failed << ") Tj\n0 -15 Td\n/F2 13 Tf\n";
    content << "(Average: " << fixed << setprecision(2) << average << ") Tj\nET\n";
    
    string contentStr = content.str();
    pdf << "5 0 obj\n<< /Length " << contentStr.length() << " >>\nstream\n";
    pdf << contentStr << "endstream\nendobj\n";
    pdf << "xref\n0 6\n0000000000 65535 f \n0000000009 00000 n \n0000000058 00000 n \n";
    pdf << "0000000115 00000 n \n0000000214 00000 n \n0000000373 00000 n \n";
    pdf << "trailer\n<< /Size 6 /Root 1 0 R >>\nstartxref\n" << (contentStr.length() + 450) << "\n%%EOF\n";
    
    pdf.close();
    cout << "✓ PDF saved: " << filename << "\n";
}

void generatePDF_CourseStats(const string& courseCode, const string& courseName,
                             int totalStudents, int passed, int failed,
                             double average, double highest, double lowest) {
    string filename = "reports/course_" + courseCode + "_stats.pdf";
    ofstream pdf(filename, ios::binary);
    
    pdf << "%PDF-1.4\n";
    pdf << "1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n";
    pdf << "2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n";
    pdf << "3 0 obj\n<< /Type /Page /Parent 2 0 R /Resources 4 0 R /MediaBox [0 0 612 792] /Contents 5 0 R >>\nendobj\n";
    pdf << "4 0 obj\n<< /Font << /F1 << /Type /Font /Subtype /Type1 /BaseFont /Helvetica >> ";
    pdf << "/F2 << /Type /Font /Subtype /Type1 /BaseFont /Helvetica-Bold >> >> >>\nendobj\n";
    
    stringstream content;
    content << "BT\n/F2 18 Tf\n50 750 Td\n(COURSE STATISTICS) Tj\n0 -30 Td\n/F1 12 Tf\n";
    content << "(Course: " << courseCode << " - " << courseName << ") Tj\n0 -30 Td\n";
    content << "/F2 14 Tf\n(Statistics) Tj\n0 -25 Td\n/F1 11 Tf\n";
    content << "(Total Students: " << totalStudents << ") Tj\n0 -20 Td\n";
    content << "(Passed: " << passed << ") Tj\n0 -20 Td\n";
    content << "(Failed: " << failed << ") Tj\n0 -20 Td\n/F2 12 Tf\n";
    content << "(Average: " << fixed << setprecision(2) << average << ") Tj\n0 -20 Td\n/F1 11 Tf\n";
    content << "(Highest: " << highest << ") Tj\n0 -20 Td\n";
    content << "(Lowest: " << lowest << ") Tj\nET\n";
    
    string contentStr = content.str();
    pdf << "5 0 obj\n<< /Length " << contentStr.length() << " >>\nstream\n";
    pdf << contentStr << "endstream\nendobj\n";
    pdf << "xref\n0 6\n0000000000 65535 f \n0000000009 00000 n \n0000000058 00000 n \n";
    pdf << "0000000115 00000 n \n0000000214 00000 n \n0000000373 00000 n \n";
    pdf << "trailer\n<< /Size 6 /Root 1 0 R >>\nstartxref\n" << (contentStr.length() + 450) << "\n%%EOF\n";
    
    pdf.close();
    cout << "✓ PDF saved: " << filename << "\n";
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
    
    // Count unique students and calculate statistics for THIS COURSE
    int* uniqueStudents = new int[gradeCount];
    int uniqueCount = 0;
    int passed = 0, failed = 0;
    double sum = 0, highest = -1, lowest = 101;
    
    for (int i = 0; i < gradeCount; i++) {
        if (grades[i].courseCode == courseCode) {
            // Check if student already counted
            bool alreadyCounted = false;
            for (int j = 0; j < uniqueCount; j++) {
                if (uniqueStudents[j] == grades[i].studentID) {
                    alreadyCounted = true;
                    break;
                }
            }
            
            if (!alreadyCounted) {
                uniqueStudents[uniqueCount++] = grades[i].studentID;
                
                sum += grades[i].grade;
                
                if (grades[i].grade >= 50) passed++;
                else failed++;
                
                if (grades[i].grade > highest) highest = grades[i].grade;
                if (grades[i].grade < lowest) lowest = grades[i].grade;
            }
        }
    }
    
    delete[] uniqueStudents;
    delete[] grades;
    
    if (uniqueCount == 0) {
        cout << "No grades recorded for this course!\n";
        return;
    }
    
    double average = sum / uniqueCount;
    
    cout << "\n--- Statistics for " << courseCode << " ---\n";
    cout << "Total Students: " << uniqueCount << "\n";
    cout << "Passed: " << passed << " | Failed: " << failed << "\n";
    cout << "Average: " << fixed << setprecision(2) << average << "\n";
    cout << "Highest: " << highest << " | Lowest: " << lowest << "\n";
    
    string courseName = getCourseNameByCode(courseCode);
    generatePDF_CourseStats(courseCode, courseName, uniqueCount, passed, failed, average, highest, lowest);
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

// students


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
    
    StudentGradeInfo* studentGrades = new StudentGradeInfo[gradeCount];
    int studentGradeCount = 0;
    
    for (int i = 0; i < gradeCount; i++) {
        if (grades[i].studentID == currentUser.id) {
            studentGrades[studentGradeCount].courseCode = grades[i].courseCode;
            studentGrades[studentGradeCount].courseName = getCourseNameByCode(grades[i].courseCode);
            studentGrades[studentGradeCount].grade = grades[i].grade;
            studentGradeCount++;
        }
    }
    
    if (studentGradeCount == 0) {
        cout << "No grades found!\n";
        delete[] studentGrades;
        delete[] grades;
        return;
    }
    
    
    for (int i = 0; i < studentGradeCount - 1; i++) {
        for (int j = 0; j < studentGradeCount - i - 1; j++) {
            if (studentGrades[j].courseName > studentGrades[j + 1].courseName) {
                StudentGradeInfo temp = studentGrades[j];
                studentGrades[j] = studentGrades[j + 1];
                studentGrades[j + 1] = temp;
            }
        }
    }
    
    double sum = 0;
    int passed = 0, failed = 0;
    for (int i = 0; i < studentGradeCount; i++) {
        sum += studentGrades[i].grade;
        if (studentGrades[i].grade >= 50) passed++;
        else failed++;
    }
    double average = sum / studentGradeCount;
    
    cout << "\n=== YOUR GRADES ===\n";
    for (int i = 0; i < studentGradeCount; i++) {
        cout << studentGrades[i].courseCode << " - " << studentGrades[i].courseName 
             << ": " << studentGrades[i].grade;
        if (studentGrades[i].grade >= 50) cout << " (PASSED)";
        else cout << " (FAILED)";
        cout << "\n";
    }
    cout << "\nTotal: " << studentGradeCount << " | Passed: " << passed 
         << " | Failed: " << failed << " | Average: " << fixed << setprecision(2) << average << "\n";
    
    string fullName = currentUser.firstName + " " + currentUser.lastName;
    generatePDF_StudentReport(currentUser.id, fullName, currentUser.email, currentUser.phone,
                             studentGrades, studentGradeCount, passed, failed, average);
    
    delete[] studentGrades;
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

// instructor 

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


int main() {
    srand(time(0));
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


