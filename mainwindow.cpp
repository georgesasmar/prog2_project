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
    this->resize(800, 600);
    this->setWindowTitle("Student Grades Management System");
    
    // Center window
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

void MainWindow::createLoginPage()
{
    loginPage = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(loginPage);
    mainLayout->setAlignment(Qt::AlignCenter);
    
    // Title
    QLabel* titleLabel = new QLabel("STUDENT GRADES MANAGEMENT SYSTEM");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    
    // Login form
    QGroupBox* loginBox = new QGroupBox("Login");
    QFormLayout* formLayout = new QFormLayout();
    
    loginIdEdit = new QLineEdit();
    loginIdEdit->setPlaceholderText("Enter your ID");
    
    loginPasswordEdit = new QLineEdit();
    loginPasswordEdit->setEchoMode(QLineEdit::Password);
    loginPasswordEdit->setPlaceholderText("Enter your password");
    
    formLayout->addRow("User ID:", loginIdEdit);
    formLayout->addRow("Password:", loginPasswordEdit);
    
    QPushButton* loginBtn = new QPushButton("Login");
    loginBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 10px; font-size: 14px; }");
    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    
    QPushButton* registerBtn = new QPushButton("Register New Account");
    registerBtn->setStyleSheet("QPushButton { background-color: #2196F3; color: white; padding: 10px; font-size: 14px; }");
    connect(registerBtn, &QPushButton::clicked, this, &MainWindow::showRegisterPage);
    
    formLayout->addRow(loginBtn);
    formLayout->addRow(registerBtn);
    
    loginBox->setLayout(formLayout);
    loginBox->setMaximumWidth(400);
    
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(loginBox, 0, Qt::AlignCenter);
    
    stackedWidget->addWidget(loginPage);
}

void MainWindow::createRegisterPage()
{
    registerPage = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(registerPage);
    
    QLabel* titleLabel = new QLabel("USER REGISTRATION");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont->setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    
    QScrollArea* scrollArea = new QScrollArea();
    QWidget* scrollWidget = new QWidget();
    QFormLayout* formLayout = new QFormLayout(scrollWidget);
    
    regFirstNameEdit = new QLineEdit();
    regFirstNameEdit->setPlaceholderText("First name");
    
    regLastNameEdit = new QLineEdit();
    regLastNameEdit->setPlaceholderText("Last name");
    
    regPasswordEdit = new QLineEdit();
    regPasswordEdit->setEchoMode(QLineEdit::Password);
    regPasswordEdit->setPlaceholderText("Min 8 chars, with numbers, letters, special chars");
    
    regEmailEdit = new QLineEdit();
    regEmailEdit->setPlaceholderText("example@email.com");
    
    regPhoneEdit = new QLineEdit();
    regPhoneEdit->setPlaceholderText("+1234567890");
    
    regRoleCombo = new QComboBox();
    regRoleCombo->addItem("Student", "student");
    regRoleCombo->addItem("Instructor", "instructor");
    regRoleCombo->addItem("Administrator", "admin");
    
    formLayout->addRow("First Name:", regFirstNameEdit);
    formLayout->addRow("Last Name:", regLastNameEdit);
    formLayout->addRow("Password:", regPasswordEdit);
    formLayout->addRow("Email:", regEmailEdit);
    formLayout->addRow("Phone:", regPhoneEdit);
    formLayout->addRow("Role:", regRoleCombo);
    
    QPushButton* submitBtn = new QPushButton("Register");
    submitBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 10px; }");
    connect(submitBtn, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
    
    QPushButton* backBtn = new QPushButton("Back to Login");
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::showLoginPage);
    
    formLayout->addRow(submitBtn);
    formLayout->addRow(backBtn);
    
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(scrollArea);
    
    stackedWidget->addWidget(registerPage);
}