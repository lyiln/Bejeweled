#include "startscene.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QCoreApplication>
#include <QtSql>
#include <QMessageBox>
#include <QDebug>

startscene::startscene(QWidget* parent):QWidget(parent) {
    createDatabase();  // 创建数据库并连接
    connectToDatabase();

    setFixedSize(740, 480);

    // 创建输入框
    usernameLineEdit = new QLineEdit(this);
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    // 设置输入框宽度
    int textBoxWidth = 300; // 设置文本框宽度
    usernameLineEdit->setFixedWidth(textBoxWidth);
    passwordLineEdit->setFixedWidth(textBoxWidth);

    // 创建按钮
    QPushButton *loginButton = new QPushButton("登录", this);
    QPushButton *registerButton = new QPushButton("注册", this);

    // 设置按钮宽度与文本框相等
    loginButton->setFixedWidth(textBoxWidth);
    registerButton->setFixedWidth(textBoxWidth);

    // 创建布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建水平间距，使输入框和按钮居中
    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    // 添加输入框到主布局，并居中
    QHBoxLayout *usernameLayout = new QHBoxLayout();
    usernameLayout->addItem(horizontalSpacer);
    usernameLayout->addWidget(usernameLineEdit);
    usernameLayout->addItem(horizontalSpacer);
    mainLayout->addLayout(usernameLayout);

    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->addItem(horizontalSpacer);
    passwordLayout->addWidget(passwordLineEdit);
    passwordLayout->addItem(horizontalSpacer);
    mainLayout->addLayout(passwordLayout);

    // 创建按钮布局，并使按钮居中
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addItem(horizontalSpacer);
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);
    buttonLayout->addItem(horizontalSpacer);

    // 将按钮布局添加到主布局
    mainLayout->addLayout(buttonLayout);

    // 连接信号和槽
    connect(loginButton, &QPushButton::clicked, this, &startscene::login);
    connect(registerButton, &QPushButton::clicked, this, &startscene::registerAcc);
}

// 登录逻辑
void startscene::login() {
    // 获取输入框内容
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();

    // 登录逻辑：验证用户信息
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = ? AND password = ?");
    query.addBindValue(username);
    query.addBindValue(password);

    if (query.exec() && query.next()) {
        // 登录成功
        QMessageBox::information(this, "登录成功", "欢迎 " + username);

        // 发出信号，通知主程序跳转到 MenuWindow
        emit showMenuWindow();
        this->close();  // 关闭当前窗口
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误！");
    }
}

// 注册逻辑
void startscene::registerAcc() {
    // 获取输入框内容
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();

    // 注册
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "用户名和密码不能为空！");
        return;
    }

    // 检查用户名是否已存在
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE username = ?");
    query.addBindValue(username);

    if (query.exec() && query.next()) {
        // 用户名已存在，注册失败
        QMessageBox::critical(this, "注册失败", "用户名已存在，注册失败！");
    } else {
        // 用户名不存在，执行插入操作
        query.prepare("INSERT INTO users (username, password, score) VALUES (?, ?, ?)");
        query.addBindValue(username);
        query.addBindValue(password);
        query.addBindValue(0);

        if (query.exec()) {
            QMessageBox::information(this, "注册成功", "用户注册成功！");
        } else {
            QMessageBox::critical(this, "注册失败", "注册失败");
        }
    }
}

// 创建数据库
void startscene::createDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("player_data.db");  // 使用当前目录中的 player_data.db

    if (!db.open()) {
        qDebug() << "Error: failed to connect to database!";
    }

    // 创建表（如果不存在）
    QSqlQuery query;
    bool success = query.exec("CREATE TABLE IF NOT EXISTS users ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "username TEXT UNIQUE NOT NULL, "
                              "password TEXT NOT NULL, "
                              "score INTEGER)");
    if (success) {
        qDebug() << "Database and table created successfully!";
    } else {
        qDebug() << "Error: failed to create table!";
    }
}

// 数据库连接
void startscene::connectToDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("player_data.db");  // 使用当前目录中的 player_data.db

    if (!db.open()) {
        QMessageBox::critical(this, "Error", "Failed to open database.");
    }
}
