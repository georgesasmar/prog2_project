#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QTextEdit>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QGroupBox>
#include <string>
#include <QInputDialog>  
#include "json.hpp"

using namespace std;

// Forward declarations of structures
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
            instructorIDs = nullptr;
    }
}
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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Auth slots
    void onLoginClicked();
    void onRegisterClicked();
    void showRegisterPage();
    void showLoginPage();
    void onLogoutClicked();
    
    // Admin slots
    void onAddCourseClicked();
    void onRemoveCourseClicked();
    void onModifyCourseClicked();
    void onGenerateStatsClicked();
    
    // Student slots
    void onGetGradeClicked();
    void onGenerateReportClicked();
    
    // Instructor slots
    void onAddGradeClicked();
    void onModifyGradeClicked();

private:
    // Core functions
    void setupUI();
    void createLoginPage();
    void createRegisterPage();
    void createAdminPage();
    void createStudentPage();
    void createInstructorPage();
    
    // Helper functions
    bool loginUser(int id, const string& password);
    bool registerUser(const User& user);
    User* loadUsers(int& count);
    void saveUsers(User* users, int count);
    Course* loadCourses(int& count);
    void saveCourses(Course* courses, int count);
    Grade* loadGrades(int& count);
    void saveGrades(Grade* grades, int count);
    bool userExists(int id);
    bool courseExists(const string& courseCode);
    string getCourseNameByCode(const string& courseCode);
    string hashPassword(const string& password);
    bool isValidPassword(const string& password);
    bool isValidEmail(const string& email);
    bool isValidPhone(const string& phone);
    int genID();
    int findlastid();
    void generatePDF_StudentReport(int studentID, const string& studentName, 
                                   const string& email, const string& phone,
                                   StudentGradeInfo* grades, int gradeCount,
                                   int passed, int failed, double average);
    void generatePDF_CourseStats(const string& courseCode, const string& courseName,
                                int totalStudents, int passed, int failed,
                                double average, double highest, double lowest);
    
    // UI Components
    QStackedWidget* stackedWidget;
    
    // Login page widgets
    QWidget* loginPage;
    QLineEdit* loginIdEdit;
    QLineEdit* loginPasswordEdit;
    
    // Register page widgets
    QWidget* registerPage;
    QLineEdit* regFirstNameEdit;
    QLineEdit* regLastNameEdit;
    QLineEdit* regPasswordEdit;
    QLineEdit* regEmailEdit;
    QLineEdit* regPhoneEdit;
    QComboBox* regRoleCombo;
    
    // Admin page widgets
    QWidget* adminPage;
    QLineEdit* adminCourseCodeEdit;
    QLineEdit* adminCourseNameEdit;
    QLineEdit* adminInstructorIDsEdit;
    QLineEdit* adminStatsCodeEdit;
    
    // Student page widgets
    QWidget* studentPage;
    QLineEdit* studentCourseCodeEdit;
    QTextEdit* studentResultDisplay;
    
    // Instructor page widgets
    QWidget* instructorPage;
    QLineEdit* instrStudentIDEdit;
    QLineEdit* instrCourseCodeEdit;
    QLineEdit* instrGradeEdit;
    
    // Current user
    User currentUser;
    
    // File paths
    const string USERS_FILE = "data/users.json";
    const string COURSES_FILE = "data/courses.json";
    const string GRADES_FILE = "data/grades.json";
};

#endif // MAINWINDOW_H