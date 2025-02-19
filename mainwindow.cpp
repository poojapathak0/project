#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<vector>
#include<string>
#include<QString>
#include<fstream>
#include<QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDate>
#include<QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     // Initialize the database
    initializeDatabase();
    // Assuming ui->stackedWidget is your QStackedWidget
    ui->stackedWidget->setCurrentIndex(0); // Set to the first page by default

    // Make sure the stacked widget fits the window
    setCentralWidget(ui->stackedWidget);
     
    // Get the current date
    QDate currentDate = QDate::currentDate();

     //Date setup
    // Set the date in the desired format
    ui->dateLabel->setText(currentDate.toString("yyyy/MM/dd"));

    //month setup
    ui->monthLabel->setText(currentDate.toString("MMM"));
    //year setup
    ui->yearLabel->setText(currentDate.toString("yyyy"));
    
    //----------------------------------------------------------------------------


// Connect signals to slots
    // Connect buttons to slots
    connect(ui->pushButtonAddIncome, &QPushButton::clicked, this, &MainWindow::on_pushButtonAddIncome_clicked);
    connect(ui->pushButtonAddExpense, &QPushButton::clicked, this, &MainWindow::on_pushButtonAddExpense_clicked);
    connect(ui->pushButtonConfirmIncome, &QPushButton::clicked, this, &MainWindow::on_pushButtonConfirmIncome_clicked);
    connect(ui->pushButtonConfirmExpense, &QPushButton::clicked, this, &MainWindow::on_pushButtonConfirmExpense_clicked);
}

//just created struct not in use right now
struct accountInfo{
    QString username;
    QString password;
};



MainWindow::~MainWindow()
{
    delete ui;
    db.close();
}

void MainWindow::initializeDatabase()
{
    // Set up the database connection
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");

    // Open the database
    if (!db.open()) {
        qDebug() << "Database connection failed!";
    } else {
        qDebug() << "Database connected successfully!";
    }

    // Create table if not exists
           QSqlQuery query;
        QString createTableQuery = R"(
            CREATE TABLE IF NOT EXISTS transactions (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                date TEXT,
                amount REAL,
                category TEXT,
                description TEXT
            )
        )";
        if (!query.exec(createTableQuery)) {
            qDebug() << "Error creating table: " << query.lastError();
        }

   // Create users table
      
    if (!query.exec("CREATE TABLE IF NOT EXISTS users ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "first_name TEXT, "
                    "middle_name TEXT, "
                    "last_name TEXT, "
                    "username TEXT UNIQUE, "
                    "mobile_number TEXT, "
                    "password TEXT, "

                    "security_answer1 TEXT, "
                   
                    "security_answer2 TEXT, "
                    //"security_question3 TEXT, "
                    "security_answer3 TEXT)")) {
        qDebug() << "Error creating users table: " << query.lastError();
    } else {
        qDebug() << "Users table created successfully.";
    }


}


//navigation
void MainWindow::on_pushButtonlogin_clicked()
{
   ui->stackedWidget->setCurrentIndex(3);
    QString userName = ui->lineEditusername->text();
    QString userPassword = ui->lineEditpassword->text();

    if (db.isOpen()) {
        QSqlQuery query;
        query.prepare("INSERT INTO users (id,  username, password) VALUES (:id,:name, :password)");
        query.bindValue(":name", userName);
        query.bindValue(":id", 2);
        query.bindValue(":password", userPassword);

        if (!query.exec()) {
            QMessageBox::critical(this, "Error", "Failed to add user: " + query.lastError().text());
        } else {
            QMessageBox::information(this, "Success", "User added successfully");
        }
    } else {
        QMessageBox::critical(this, "Error", "Database connection failed!");
    }

}

void MainWindow::on_pushButtonCreateAccount_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
     QString firstName = ui->lineEditFirstName->text();
    QString middleName = ui->lineEditMiddleName->text();
    QString lastName = ui->lineEditLastName->text();
    QString username = ui->lineEditUsername->text();
    QString mobileNumber = ui->lineEditMobileNumber->text();
    QString password = ui->lineEditPassword->text();

    //QString question1 = ui->lineEditSecurityQuestion1->text();
    QString answer1 = ui->lineEditSecurityAnswer1->text();
    //QString question2 = ui->lineEditSecurityQuestion2->text();
    QString answer2 = ui->lineEditSecurityAnswer2->text();
    //QString question3 = ui->lineEditSecurityQuestion3->text();
    QString answer3 = ui->lineEditSecurityAnswer3->text();

    createUser(firstName, middleName, lastName, username, mobileNumber, password, answer1,answer2,answer3);

}

void MainWindow::on_pushButtonsignup_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}

void MainWindow::on_pushButtonconfirm_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButtonback_e_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_pushButtonConfirmExpense_clicked()
{
    
    ui->stackedWidget->setCurrentIndex(3);
    double amount = ui->lineEditAmountExpense->text().toDouble(); // Adjust as per your UI
    QString description = ui->lineEditNoteExpense->text(); // Adjust as per your UI
    addTransaction(amount, "Expense", description);
}


void MainWindow::on_pushButtonadd_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_pushButtonAddExpense_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}


void MainWindow::on_pushButtonAddIncome_clicked()
{
     ui->stackedWidget->setCurrentIndex(6);
}


void MainWindow::on_pushButtoncancel_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_pushButtongback_in_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_pushButtonConfirmIncome_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    double amount = ui->lineEditAmountIncome->text().toDouble(); // Adjust as per your UI
    QString description = ui->lineEditNoteIncome->text(); // Adjust as per your UI
    addTransaction(amount, "Income", description);
}
void MainWindow::addTransaction(double amount, const QString& category, const QString& description)
{
   QSqlQuery query;
   QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    query.prepare("INSERT INTO transactions (date, amount, category, description) VALUES (:date, :amount, :category, :description)");
    query.bindValue(":date", currentDateTime);
    query.bindValue(":amount", amount);
    query.bindValue(":category", category);
    query.bindValue(":description", description);

    if (!query.exec()) {
        qDebug() << "Error adding transaction:" << query.lastError();
    } else {
        qDebug() << "Transaction added successfully.";
    }
}

void MainWindow::viewTransactions()
{
    QSqlQuery query("SELECT date, amount, category, description FROM transactions");
    while (query.next()) {
        QString date = query.value(0).toString();
        double amount = query.value(1).toDouble();
        QString category = query.value(2).toString();
        QString description = query.value(3).toString();

        // Display these values in your UI (e.g., add to a table or list widget)
        // This part should be customized to fit your UI design
    }
}


void MainWindow::createUser(const QString &firstName, const QString &middleName, const QString &lastName, const QString &username, const QString &mobileNumber, const QString &password, const QString &answer1, const QString &answer2, const QString &answer3)
{
    QSqlQuery query;
    query.prepare("INSERT INTO users (first_name, middle_name, last_name, username, mobile_number, password, security_question1, security_answer1, security_question2, security_answer2, security_question3, security_answer3) "
                  "VALUES (:firstName, :middleName, :lastName, :username, :mobileNumber, :password, :question1, :answer1, :question2, :answer2, :question3, :answer3)");
    query.bindValue(":firstName", firstName);
    query.bindValue(":middleName", middleName);
    query.bindValue(":lastName", lastName);
    query.bindValue(":username", username);
    query.bindValue(":mobileNumber", mobileNumber);
    query.bindValue(":password", password); // Note: For security, hash the password
    query.bindValue(":answer1", answer1);
    query.bindValue(":answer2", answer2);
    query.bindValue(":answer3", answer3);

    if (!query.exec()) {
        qDebug() << "Error creating user: " << query.lastError();
    } else {
        qDebug() << "User created successfully.";
    }
}



