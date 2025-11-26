#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include <QScrollArea>
#include <regex>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <ctime>

using json = nlohmann::json;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    this->resize(900, 700);
    this->setWindowTitle("Student Grades Management System");
    
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);
    
    createLoginPage();
    createRegisterPage();
    createAdminPage();
    createStudentPage();
    createInstructorPage();
    
    stackedWidget->setCurrentWidget(loginPage);
}

// ==================== LOGIN PAGE ====================
void MainWindow::createLoginPage()
{
    loginPage = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(loginPage);
    mainLayout->setAlignment(Qt::AlignCenter);
    
    QLabel* titleLabel = new QLabel("STUDENT GRADES MANAGEMENT SYSTEM");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #2c3e50; margin: 20px;");
    
    QGroupBox* loginBox = new QGroupBox("Login");
    loginBox->setStyleSheet("QGroupBox { font-size: 16px; font-weight: bold; padding: 20px; }");
    QFormLayout* formLayout = new QFormLayout();
    
    loginIdEdit = new QLineEdit();
    loginIdEdit->setPlaceholderText("Enter your ID");
    loginIdEdit->setMinimumHeight(35);
    
    loginPasswordEdit = new QLineEdit();
    loginPasswordEdit->setEchoMode(QLineEdit::Password);
    loginPasswordEdit->setPlaceholderText("Enter your password");
    loginPasswordEdit->setMinimumHeight(35);
    
    formLayout->addRow("User ID:", loginIdEdit);
    formLayout->addRow("Password:", loginPasswordEdit);
    
    QPushButton* loginBtn = new QPushButton("Login");
    loginBtn->setMinimumHeight(40);
    loginBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 10px; font-size: 14px; border-radius: 5px; } QPushButton:hover { background-color: #45a049; }");
    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    
    QPushButton* registerBtn = new QPushButton("Register New Account");
    registerBtn->setMinimumHeight(40);
    registerBtn->setStyleSheet("QPushButton { background-color: #2196F3; color: white; padding: 10px; font-size: 14px; border-radius: 5px; } QPushButton:hover { background-color: #0b7dda; }");
    connect(registerBtn, &QPushButton::clicked, this, &MainWindow::showRegisterPage);
    
    formLayout->addRow(loginBtn);
    formLayout->addRow(registerBtn);
    
    loginBox->setLayout(formLayout);
    loginBox->setMaximumWidth(450);
    
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(loginBox, 0, Qt::AlignCenter);
    
    stackedWidget->addWidget(loginPage);
}

// ==================== REGISTER PAGE ====================
void MainWindow::createRegisterPage()
{
    registerPage = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(registerPage);
    
    QLabel* titleLabel = new QLabel("USER REGISTRATION");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #2c3e50; margin: 20px;");
    
    QScrollArea* scrollArea = new QScrollArea();
    QWidget* scrollWidget = new QWidget();
    QFormLayout* formLayout = new QFormLayout(scrollWidget);
    formLayout->setSpacing(15);
    
    regFirstNameEdit = new QLineEdit();
    regFirstNameEdit->setPlaceholderText("First name");
    regFirstNameEdit->setMinimumHeight(35);
    
    regLastNameEdit = new QLineEdit();
    regLastNameEdit->setPlaceholderText("Last name");
    regLastNameEdit->setMinimumHeight(35);
    
    regPasswordEdit = new QLineEdit();
    regPasswordEdit->setEchoMode(QLineEdit::Password);
    regPasswordEdit->setPlaceholderText("Min 8 chars, with numbers, letters, special chars");
    regPasswordEdit->setMinimumHeight(35);
    
    regEmailEdit = new QLineEdit();
    regEmailEdit->setPlaceholderText("example@email.com");
    regEmailEdit->setMinimumHeight(35);
    
    regPhoneEdit = new QLineEdit();
    regPhoneEdit->setPlaceholderText("+1234567890");
    regPhoneEdit->setMinimumHeight(35);
    
    regRoleCombo = new QComboBox();
    regRoleCombo->addItem("Student", "student");
    regRoleCombo->addItem("Instructor", "instructor");
    regRoleCombo->addItem("Administrator", "admin");
    regRoleCombo->setMinimumHeight(35);
    
    formLayout->addRow("First Name:", regFirstNameEdit);
    formLayout->addRow("Last Name:", regLastNameEdit);
    formLayout->addRow("Password:", regPasswordEdit);
    formLayout->addRow("Email:", regEmailEdit);
    formLayout->addRow("Phone:", regPhoneEdit);
    formLayout->addRow("Role:", regRoleCombo);
    
    QPushButton* submitBtn = new QPushButton("Register");
    submitBtn->setMinimumHeight(40);
    submitBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 10px; font-size: 14px; border-radius: 5px; } QPushButton:hover { background-color: #45a049; }");
    connect(submitBtn, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
    
    QPushButton* backBtn = new QPushButton("Back to Login");
    backBtn->setMinimumHeight(40);
    backBtn->setStyleSheet("QPushButton { background-color: #757575; color: white; padding: 10px; font-size: 14px; border-radius: 5px; } QPushButton:hover { background-color: #616161; }");
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::showLoginPage);
    
    formLayout->addRow(submitBtn);
    formLayout->addRow(backBtn);
    
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(scrollArea);
    
    stackedWidget->addWidget(registerPage);
}

// ==================== ADMIN PAGE ====================
void MainWindow::createAdminPage()
{
    adminPage = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(adminPage);
    
    QLabel* titleLabel = new QLabel("ADMINISTRATOR PANEL");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #2c3e50; margin: 10px;");
    
    QScrollArea* scrollArea = new QScrollArea();
    QWidget* scrollWidget = new QWidget();
    QVBoxLayout* scrollLayout = new QVBoxLayout(scrollWidget);
    
    // Add Course Section
    QGroupBox* addCourseBox = new QGroupBox("Add Course");
    QFormLayout* addCourseLayout = new QFormLayout();
    
    adminCourseCodeEdit = new QLineEdit();
    adminCourseCodeEdit->setPlaceholderText("e.g., CS101");
    
    adminCourseNameEdit = new QLineEdit();
    adminCourseNameEdit->setPlaceholderText("e.g., Introduction to Programming");
    
    adminInstructorIDsEdit = new QLineEdit();
    adminInstructorIDsEdit->setPlaceholderText("e.g., 1001,1002 (comma-separated)");
    
    addCourseLayout->addRow("Course Code:", adminCourseCodeEdit);
    addCourseLayout->addRow("Course Name:", adminCourseNameEdit);
    addCourseLayout->addRow("Instructor IDs:", adminInstructorIDsEdit);
    
    QPushButton* addCourseBtn = new QPushButton("Add Course");
    addCourseBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 8px; }");
    connect(addCourseBtn, &QPushButton::clicked, this, &MainWindow::onAddCourseClicked);
    addCourseLayout->addRow(addCourseBtn);
    
    addCourseBox->setLayout(addCourseLayout);
    scrollLayout->addWidget(addCourseBox);
    
    // Remove/Modify Course Section
    QGroupBox* modifyCourseBox = new QGroupBox("Modify/Remove Course");
    QVBoxLayout* modifyLayout = new QVBoxLayout();
    
    QPushButton* removeCourseBtn = new QPushButton("Remove Course");
    removeCourseBtn->setStyleSheet("QPushButton { background-color: #f44336; color: white; padding: 8px; }");
    connect(removeCourseBtn, &QPushButton::clicked, this, &MainWindow::onRemoveCourseClicked);
    
    QPushButton* modifyCourseBtn = new QPushButton("Modify Course");
    modifyCourseBtn->setStyleSheet("QPushButton { background-color: #FF9800; color: white; padding: 8px; }");
    connect(modifyCourseBtn, &QPushButton::clicked, this, &MainWindow::onModifyCourseClicked);
    
    modifyLayout->addWidget(removeCourseBtn);
    modifyLayout->addWidget(modifyCourseBtn);
    modifyCourseBox->setLayout(modifyLayout);
    scrollLayout->addWidget(modifyCourseBox);
    
    // Statistics Section
    QGroupBox* statsBox = new QGroupBox("Generate Course Statistics");
    QFormLayout* statsLayout = new QFormLayout();
    
    adminStatsCodeEdit = new QLineEdit();
    adminStatsCodeEdit->setPlaceholderText("Enter course code");
    
    statsLayout->addRow("Course Code:", adminStatsCodeEdit);
    
    QPushButton* generateStatsBtn = new QPushButton("Generate Statistics PDF");
    generateStatsBtn->setStyleSheet("QPushButton { background-color: #2196F3; color: white; padding: 8px; }");
    connect(generateStatsBtn, &QPushButton::clicked, this, &MainWindow::onGenerateStatsClicked);
    statsLayout->addRow(generateStatsBtn);
    
    statsBox->setLayout(statsLayout);
    scrollLayout->addWidget(statsBox);
    
    // Logout Button
    QPushButton* logoutBtn = new QPushButton("Logout");
    logoutBtn->setStyleSheet("QPushButton { background-color: #757575; color: white; padding: 10px; margin-top: 20px; }");
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
    scrollLayout->addWidget(logoutBtn);
    
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(scrollArea);
    
    stackedWidget->addWidget(adminPage);
}

// ==================== STUDENT PAGE ====================
void MainWindow::createStudentPage()
{
    studentPage = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(studentPage);
    
    QLabel* titleLabel = new QLabel("STUDENT PANEL");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #2c3e50; margin: 10px;");
    
    // Get Grade Section
    QGroupBox* getGradeBox = new QGroupBox("Get Grade for Course");
    QFormLayout* getGradeLayout = new QFormLayout();
    
    studentCourseCodeEdit = new QLineEdit();
    studentCourseCodeEdit->setPlaceholderText("Enter course code");
    
    getGradeLayout->addRow("Course Code:", studentCourseCodeEdit);
    
    QPushButton* getGradeBtn = new QPushButton("Get Grade");
    getGradeBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 8px; }");
    connect(getGradeBtn, &QPushButton::clicked, this, &MainWindow::onGetGradeClicked);
    getGradeLayout->addRow(getGradeBtn);
    
    getGradeBox->setLayout(getGradeLayout);
    
    // Generate Report Section
    QGroupBox* reportBox = new QGroupBox("Generate Grade Report");
    QVBoxLayout* reportLayout = new QVBoxLayout();
    
    QPushButton* generateReportBtn = new QPushButton("Generate Full Report (PDF)");
    generateReportBtn->setStyleSheet("QPushButton { background-color: #2196F3; color: white; padding: 10px; }");
    connect(generateReportBtn, &QPushButton::clicked, this, &MainWindow::onGenerateReportClicked);
    reportLayout->addWidget(generateReportBtn);
    
    reportBox->setLayout(reportLayout);
    
    // Result Display
    studentResultDisplay = new QTextEdit();
    studentResultDisplay->setReadOnly(true);
    studentResultDisplay->setPlaceholderText("Results will appear here...");
    studentResultDisplay->setMinimumHeight(200);
    
    // Logout Button
    QPushButton* logoutBtn = new QPushButton("Logout");
    logoutBtn->setStyleSheet("QPushButton { background-color: #757575; color: white; padding: 10px; }");
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
    
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(getGradeBox);
    mainLayout->addWidget(reportBox);
    mainLayout->addWidget(new QLabel("Results:"));
    mainLayout->addWidget(studentResultDisplay);
    mainLayout->addWidget(logoutBtn);
    
    stackedWidget->addWidget(studentPage);
}

// ==================== INSTRUCTOR PAGE ====================
void MainWindow::createInstructorPage()
{
    instructorPage = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(instructorPage);
    
    QLabel* titleLabel = new QLabel("INSTRUCTOR PANEL");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #2c3e50; margin: 10px;");
    
    QScrollArea* scrollArea = new QScrollArea();
    QWidget* scrollWidget = new QWidget();
    QVBoxLayout* scrollLayout = new QVBoxLayout(scrollWidget);
    
    // Add Grade Section
    QGroupBox* addGradeBox = new QGroupBox("Add Grade");
    QFormLayout* addGradeLayout = new QFormLayout();
    
    instrStudentIDEdit = new QLineEdit();
    instrStudentIDEdit->setPlaceholderText("Student ID");
    
    instrCourseCodeEdit = new QLineEdit();
    instrCourseCodeEdit->setPlaceholderText("Course Code");
    
    instrGradeEdit = new QLineEdit();
    instrGradeEdit->setPlaceholderText("Grade (0-100)");
    
    addGradeLayout->addRow("Student ID:", instrStudentIDEdit);
    addGradeLayout->addRow("Course Code:", instrCourseCodeEdit);
    addGradeLayout->addRow("Grade:", instrGradeEdit);
    
    QPushButton* addGradeBtn = new QPushButton("Add Grade");
    addGradeBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 8px; }");
    connect(addGradeBtn, &QPushButton::clicked, this, &MainWindow::onAddGradeClicked);
    addGradeLayout->addRow(addGradeBtn);
    
    addGradeBox->setLayout(addGradeLayout);
    scrollLayout->addWidget(addGradeBox);
    
    // Modify Grade Section
    QGroupBox* modifyGradeBox = new QGroupBox("Modify Existing Grade");
    QVBoxLayout* modifyLayout = new QVBoxLayout();
    
    QPushButton* modifyGradeBtn = new QPushButton("Modify Grade");
    modifyGradeBtn->setStyleSheet("QPushButton { background-color: #FF9800; color: white; padding: 8px; }");
    connect(modifyGradeBtn, &QPushButton::clicked, this, &MainWindow::onModifyGradeClicked);
    modifyLayout->addWidget(modifyGradeBtn);
    
    modifyGradeBox->setLayout(modifyLayout);
    scrollLayout->addWidget(modifyGradeBox);
    
    // Logout Button
    QPushButton* logoutBtn = new QPushButton("Logout");
    logoutBtn->setStyleSheet("QPushButton { background-color: #757575; color: white; padding: 10px; margin-top: 20px; }");
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
    scrollLayout->addWidget(logoutBtn);
    
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(scrollArea);
    
    stackedWidget->addWidget(instructorPage);
}

// ==================== AUTHENTICATION SLOTS ====================
void MainWindow::onLoginClicked()
{
    bool ok;
    int id = loginIdEdit->text().toInt(&ok);
    
    if (!ok) {
        QMessageBox::warning(this, "Error", "Invalid ID format!");
        return;
    }
    
    string password = loginPasswordEdit->text().toStdString();
    
    if (loginUser(id, password)) {
        loginIdEdit->clear();
        loginPasswordEdit->clear();
        
        if (currentUser.role == "admin") {
            stackedWidget->setCurrentWidget(adminPage);
        } else if (currentUser.role == "student") {
            stackedWidget->setCurrentWidget(studentPage);
            studentResultDisplay->clear();
        } else if (currentUser.role == "instructor") {
            stackedWidget->setCurrentWidget(instructorPage);
        }
        
        QMessageBox::information(this, "Success", 
            QString("Welcome %1 %2!").arg(QString::fromStdString(currentUser.firstName))
                                     .arg(QString::fromStdString(currentUser.lastName)));
    } else {
        QMessageBox::warning(this, "Error", "Invalid ID or password!");
    }
}

void MainWindow::onRegisterClicked()
{
    User newUser;
    newUser.firstName = regFirstNameEdit->text().toStdString();
    newUser.lastName = regLastNameEdit->text().toStdString();
    string password = regPasswordEdit->text().toStdString();
    newUser.email = regEmailEdit->text().toStdString();
    newUser.phone = regPhoneEdit->text().toStdString();
    newUser.role = regRoleCombo->currentData().toString().toStdString();
    
    // Validation
    if (newUser.firstName.empty() || newUser.lastName.empty()) {
        QMessageBox::warning(this, "Error", "Name fields cannot be empty!");
        return;
    }
    
    if (!isValidPassword(password)) {
        QMessageBox::warning(this, "Error", "Password must be at least 8 characters with numbers, letters, and special characters!");
        return;
    }
    
    if (!isValidEmail(newUser.email)) {
        QMessageBox::warning(this, "Error", "Invalid email format!");
        return;
    }
    
    if (!isValidPhone(newUser.phone)) {
        QMessageBox::warning(this, "Error", "Invalid phone format!");
        return;
    }
    
    newUser.password = hashPassword(password);
    newUser.id = genID();
    
    if (registerUser(newUser)) {
        QMessageBox::information(this, "Success", 
            QString("Registration successful!\nYour ID is: %1\nPlease remember this ID for login.").arg(newUser.id));
        
        // Clear fields
        regFirstNameEdit->clear();
        regLastNameEdit->clear();
        regPasswordEdit->clear();
        regEmailEdit->clear();
        regPhoneEdit->clear();
        
        currentUser = newUser;
        
        // Go to appropriate page
        if (currentUser.role == "admin") {
            stackedWidget->setCurrentWidget(adminPage);
        } else if (currentUser.role == "student") {
            stackedWidget->setCurrentWidget(studentPage);
        } else if (currentUser.role == "instructor") {
            stackedWidget->setCurrentWidget(instructorPage);
        }
    } else {
        QMessageBox::warning(this, "Error", "Registration failed!");
    }
}

void MainWindow::showRegisterPage()
{
    stackedWidget->setCurrentWidget(registerPage);
}

void MainWindow::showLoginPage()
{
    stackedWidget->setCurrentWidget(loginPage);
}

void MainWindow::onLogoutClicked()
{
    currentUser = User();
    loginIdEdit->clear();
    loginPasswordEdit->clear();
    if (studentResultDisplay) studentResultDisplay->clear();
    stackedWidget->setCurrentWidget(loginPage);
    QMessageBox::information(this, "Logout", "You have been logged out successfully.");
}

// ==================== ADMIN SLOTS ====================
void MainWindow::onAddCourseClicked()
{
    string courseCode = adminCourseCodeEdit->text().toStdString();
    string courseName = adminCourseNameEdit->text().toStdString();
    string instructorIDsStr = adminInstructorIDsEdit->text().toStdString();
    
    if (courseCode.empty() || courseName.empty()) {
        QMessageBox::warning(this, "Error", "Course code and name are required!");
        return;
    }
    
    if (courseExists(courseCode)) {
        QMessageBox::warning(this, "Error", "Course already exists!");
        return;
    }
    
    Course newCourse;
    newCourse.courseCode = courseCode;
    newCourse.courseName = courseName;
    
    // Parse instructor IDs
    stringstream ss(instructorIDsStr);
    string token;
    int tempIDs[100];
    int count = 0;
    
    while (getline(ss, token, ',')) {
        if (!token.empty()) {
            tempIDs[count++] = stoi(token);
        }
    }
    
    newCourse.instructorCount = count;
    newCourse.instructorIDs = new int[count];
    for (int i = 0; i < count; i++) {
        newCourse.instructorIDs[i] = tempIDs[i];
    }
    
    // Load and save
    int courseCount;
    Course* courses = loadCourses(courseCount);
    Course* newCourses = new Course[courseCount + 1];
    
    for (int i = 0; i < courseCount; i++) {
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
    
    newCourses[courseCount] = newCourse;
    saveCourses(newCourses, courseCount + 1);
    
    if (courses) delete[] courses;
    delete[] newCourses;
    
    adminCourseCodeEdit->clear();
    adminCourseNameEdit->clear();
    adminInstructorIDsEdit->clear();
    
    QMessageBox::information(this, "Success", "Course added successfully!");
}

void MainWindow::onRemoveCourseClicked()
{
    bool ok;
    QString courseCode = QInputDialog::getText(this, "Remove Course", 
                                               "Enter course code to remove:", 
                                               QLineEdit::Normal, "", &ok);
    if (!ok || courseCode.isEmpty()) return;
    
    string code = courseCode.toStdString();
    
    if (!courseExists(code)) {
        QMessageBox::warning(this, "Error", "Course not found!");
        return;
    }
    
    int count;
    Course* courses = loadCourses(count);
    
    int foundIndex = -1;
    for (int i = 0; i < count; i++) {
        if (courses[i].courseCode == code) {
            foundIndex = i;
            break;
        }
    }
    
    if (foundIndex == -1) {
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
    
    QMessageBox::information(this, "Success", "Course removed successfully!");
}

void MainWindow::onModifyCourseClicked()
{
    bool ok;
    QString courseCode = QInputDialog::getText(this, "Modify Course", 
                                               "Enter course code to modify:", 
                                               QLineEdit::Normal, "", &ok);
    if (!ok || courseCode.isEmpty()) return;
    
    string code = courseCode.toStdString();
    
    if (!courseExists(code)) {
        QMessageBox::warning(this, "Error", "Course not found!");
        return;
    }
    
    QString newName = QInputDialog::getText(this, "Modify Course", 
                                           "Enter new course name:", 
                                           QLineEdit::Normal, "", &ok);
    if (!ok || newName.isEmpty()) return;
    
    QString instructorIDs = QInputDialog::getText(this, "Modify Course", 
                                                  "Enter instructor IDs (comma-separated):", 
                                                  QLineEdit::Normal, "", &ok);
    if (!ok) return;
    
    int count;
    Course* courses = loadCourses(count);
    
    for (int i = 0; i < count; i++) {
        if (courses[i].courseCode == code) {
            courses[i].courseName = newName.toStdString();
            
            // Parse new instructor IDs
            stringstream ss(instructorIDs.toStdString());
            string token;
            int tempIDs[100];
            int idCount = 0;
            
            while (getline(ss, token, ',')) {
                if (!token.empty()) {
                    tempIDs[idCount++] = stoi(token);
                }
            }
            
            delete[] courses[i].instructorIDs;
            courses[i].instructorCount = idCount;
            courses[i].instructorIDs = new int[idCount];
            for (int j = 0; j < idCount; j++) {
                courses[i].instructorIDs[j] = tempIDs[j];
            }
            break;
        }
    }
    
    saveCourses(courses, count);
    delete[] courses;
    
    QMessageBox::information(this, "Success", "Course modified successfully!");
}

void MainWindow::onGenerateStatsClicked()
{
    string courseCode = adminStatsCodeEdit->text().toStdString();
    
    if (courseCode.empty()) {
        QMessageBox::warning(this, "Error", "Please enter a course code!");
        return;
    }
    
    if (!courseExists(courseCode)) {
        QMessageBox::warning(this, "Error", "Course not found!");
        return;
    }
    
    int gradeCount;
    Grade* grades = loadGrades(gradeCount);
    
    if (!grades) {
        QMessageBox::warning(this, "Error", "No grades found!");
        return;
    }
    
    int* uniqueStudents = new int[gradeCount];
    int uniqueCount = 0;
    int passed = 0, failed = 0;
    double sum = 0, highest = -1, lowest = 101;
    
    for (int i = 0; i < gradeCount; i++) {
        if (grades[i].courseCode == courseCode) {
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
    QMessageBox::warning(this, "Error", "No grades for this course!");
    return;
}

double average = sum / uniqueCount;
string courseName = getCourseNameByCode(courseCode);

generatePDF_CourseStats(courseCode, courseName, uniqueCount, passed, failed, average, highest, lowest);

adminStatsCodeEdit->clear();

QMessageBox::information(this, "Success", 
    QString("PDF generated!\n\nTotal: %1\nPassed: %2\nFailed: %3\nAverage: %4")
    .arg(uniqueCount).arg(passed).arg(failed).arg(average, 0, 'f', 2));
}
// ==================== STUDENT SLOTS ====================
void MainWindow::onGetGradeClicked()
{
string courseCode = studentCourseCodeEdit->text().toStdString();
if (courseCode.empty()) {
    QMessageBox::warning(this, "Error", "Please enter a course code!");
    return;
}

int gradeCount;
Grade* grades = loadGrades(gradeCount);

if (!grades) {
    QMessageBox::warning(this, "Error", "No grades found!");
    return;
}

bool found = false;
for (int i = 0; i < gradeCount; i++) {
    if (grades[i].studentID == currentUser.id && grades[i].courseCode == courseCode) {
        string courseName = getCourseNameByCode(courseCode);
        QString result = QString("Course: %1 - %2\nYour Grade: %3\nStatus: %4")
            .arg(QString::fromStdString(courseCode))
            .arg(QString::fromStdString(courseName))
            .arg(grades[i].grade)
            .arg(grades[i].grade >= 50 ? "PASSED" : "FAILED");
        
        studentResultDisplay->setText(result);
        found = true;
        break;
    }
}

delete[] grades;

if (!found) {
    studentResultDisplay->setText("No grade found for this course.");
}

studentCourseCodeEdit->clear();
}
void MainWindow::onGenerateReportClicked()
{
int gradeCount;
Grade* grades = loadGrades(gradeCount);
if (!grades) {
    QMessageBox::warning(this, "Error", "No grades found!");
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
    delete[] studentGrades;
    delete[] grades;
    QMessageBox::warning(this, "Error", "No grades found!");
    return;
}

// Sort by course name
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
QString displayText = "YOUR GRADES:\n\n";

for (int i = 0; i < studentGradeCount; i++) {
    sum += studentGrades[i].grade;
    if (studentGrades[i].grade >= 50) passed++;
    else failed++;
    
    displayText += QString("%1 - %2: %3 (%4)\n")
        .arg(QString::fromStdString(studentGrades[i].courseCode))
        .arg(QString::fromStdString(studentGrades[i].courseName))
        .arg(studentGrades[i].grade)
        .arg(studentGrades[i].grade >= 50 ? "PASSED" : "FAILED");
}

double average = sum / studentGradeCount;

displayText += QString("\n=== SUMMARY ===\nTotal: %1 | Passed: %2 | Failed: %3 | Average: %4")
    .arg(studentGradeCount).arg(passed).arg(failed).arg(average, 0, 'f', 2);

studentResultDisplay->setText(displayText);

string fullName = currentUser.firstName + " " + currentUser.lastName;
generatePDF_StudentReport(currentUser.id, fullName, currentUser.email, currentUser.phone,
                         studentGrades, studentGradeCount, passed, failed, average);

delete[] studentGrades;
delete[] grades;

QMessageBox::information(this, "Success", "PDF report generated successfully!");
}
// ==================== INSTRUCTOR SLOTS ====================
void MainWindow::onAddGradeClicked()
{
bool ok;
int studentID = instrStudentIDEdit->text().toInt(&ok);
if (!ok) {
QMessageBox::warning(this, "Error", "Invalid student ID!");
return;
}
string courseCode = instrCourseCodeEdit->text().toStdString();
double grade = instrGradeEdit->text().toDouble(&ok);

if (!ok || grade < 0 || grade > 100) {
    QMessageBox::warning(this, "Error", "Grade must be between 0 and 100!");
    return;
}

if (!userExists(studentID)) {
    QMessageBox::warning(this, "Error", "Student not found!");
    return;
}

if (!courseExists(courseCode)) {
    QMessageBox::warning(this, "Error", "Course not found!");
    return;
}

Grade newGrade;
newGrade.studentID = studentID;
newGrade.courseCode = courseCode;
newGrade.instructorID = currentUser.id;
newGrade.grade = grade;

int gradeCount;
Grade* grades = loadGrades(gradeCount);

if (grades) {
    for (int i = 0; i < gradeCount; i++) {
        if (grades[i].studentID == studentID && grades[i].courseCode == courseCode) {
            delete[] grades;
            QMessageBox::warning(this, "Error", "Grade already exists! Use modify option.");
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

if (grades) delete[] grades;
delete[] newGrades;

instrStudentIDEdit->clear();
instrCourseCodeEdit->clear();
instrGradeEdit->clear();

QMessageBox::information(this, "Success", "Grade added successfully!");
}
void MainWindow::onModifyGradeClicked()
{
bool ok;
int studentID = QInputDialog::getInt(this, "Modify Grade", "Enter student ID:", 0, 0, 999999, 1, &ok);
if (!ok) return;
QString courseCode = QInputDialog::getText(this, "Modify Grade", "Enter course code:", QLineEdit::Normal, "", &ok);
if (!ok || courseCode.isEmpty()) return;

double newGrade = QInputDialog::getDouble(this, "Modify Grade", "Enter new grade (0-100):", 0, 0, 100, 2, &ok);
if (!ok) return;

int gradeCount;
Grade* grades = loadGrades(gradeCount);

if (!grades) {
    QMessageBox::warning(this, "Error", "No grades found!");
    return;
}

bool found = false;
for (int i = 0; i < gradeCount; i++) {
    if (grades[i].studentID == studentID && grades[i].courseCode == courseCode.toStdString()) {
        grades[i].grade = newGrade;
        grades[i].instructorID = currentUser.id;
        found = true;
        break;
    }
}

if (found) {
    saveGrades(grades, gradeCount);
    QMessageBox::information(this, "Success", "Grade modified successfully!");
} else {
    QMessageBox::warning(this, "Error", "Grade not found!");
}

delete[] grades;
}
// ==================== HELPER FUNCTIONS ====================
bool MainWindow::loginUser(int id, const string& password)
{
string hashedPassword = hashPassword(password);
int count;
User* users = loadUsers(count);
if (!users) return false;

for (int i = 0; i < count; i++) {
    if (users[i].id == id && users[i].password == hashedPassword) {
        currentUser = users[i];
        delete[] users;
        return true;
    }
}

delete[] users;
return false;
}
bool MainWindow::registerUser(const User& user)
{
int count;
User* users = loadUsers(count);
User* newUsers = new User[count + 1];
for (int i = 0; i < count; i++) {
    newUsers[i] = users[i];
}
newUsers[count] = user;

saveUsers(newUsers, count + 1);

if (users) delete[] users;
delete[] newUsers;
return true;
}
User* MainWindow::loadUsers(int& count)
{
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
void MainWindow::saveUsers(User* users, int count)
{
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
Course* MainWindow::loadCourses(int& count)
{
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
void MainWindow::saveCourses(Course* courses, int count)
{
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
Grade* MainWindow::loadGrades(int& count)
{
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
void MainWindow::saveGrades(Grade* grades, int count)
{
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
bool MainWindow::userExists(int id)
{
int count;
User* users = loadUsers(count);
if (!users) return false;
for (int i = 0; i < count; i++) {
    if (users[i].id == id) {
        delete[] users;
        return true;
    }
}
delete[] users;
return false;
}
bool MainWindow::courseExists(const string& courseCode)
{
int count;
Course* courses = loadCourses(count);
if (!courses) return false;
for (int i = 0; i < count; i++) {
    if (courses[i].courseCode == courseCode) {
        delete[] courses;
        return true;
    }
}
delete[] courses;
return false;
}
string MainWindow::getCourseNameByCode(const string& courseCode)
{
int count;
Course* courses = loadCourses(count);
if (!courses) return "";
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
string MainWindow::hashPassword(const string& password)
{
hash<string> hasher;
size_t hashValue = hasher(password);
stringstream ss;
ss << hex << hashValue;
return ss.str();
}
bool MainWindow::isValidPassword(const string& password)
{
if (password.length() < 8) return false;
bool hasDigit = false, hasLetter = false, hasSpecial = false;
for (char c : password) {
if (isdigit(c)) hasDigit = true;
else if (isalpha(c)) hasLetter = true;
else if (!isspace(c)) hasSpecial = true;
}
return hasDigit && hasLetter && hasSpecial;
}
bool MainWindow::isValidEmail(const string& email)
{
const regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+.[a-zA-Z]{2,})");
return regex_match(email, pattern);
}
bool MainWindow::isValidPhone(const string& phone)
{
const regex pattern(R"(^[\d\s+-()]{8,15}$)");
return regex_match(phone, pattern);
}
int MainWindow::findlastid()
{
int count;
User* users = loadUsers(count);
if (!users) return 1000;
int maxId = 0;
for (int i = 0; i < count; i++) {
    if (users[i].id > maxId) maxId = users[i].id;
}
delete[] users;
return maxId;
}
int MainWindow::genID()
{
return findlastid() + 1;
}
void MainWindow::generatePDF_StudentReport(int studentID, const string& studentName,
const string& email, const string& phone,
StudentGradeInfo* grades, int gradeCount,
int passed, int failed, double average)
{
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
}
void MainWindow::generatePDF_CourseStats(const string& courseCode, const string& courseName,
int totalStudents, int passed, int failed,
double average, double highest, double lowest)
{
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
}