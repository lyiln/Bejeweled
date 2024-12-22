#include "startscene.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QCoreApplication>
#include <QtSql>
#include <QMessageBox>
#include <QDebug>
#include <QPalette>  // 添加 QPalette 头文件
#include <QPixmap>   // 添加 QPixmap 头文件

startscene::startscene(QWidget* parent):QWidget(parent) {
    createDatabase();  // 创建数据库并连接
    connectToDatabase();

    setFixedSize(740, 480);

    // 设置背景图片
    QString backgroundImagePath = ":/images/loginBackground.png";  // 替换为你的背景图片路径
    setBackgroundImage(backgroundImagePath);

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

    // 登录
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
        // 用户名不存在
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
    db.setDatabaseName("player_data.db");

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

int startscene::getScore(const QString &name){
    QSqlQuery query;
    query.prepare("SELECT score FROM users WHERE username = ?");
    query.addBindValue(name);

    if (query.exec() && query.next()) {
        return query.value(0).toInt(); // 返回分数
    } else {
        qDebug() << "Error: Unable to fetch score for user " << name;
        return -1;  // 如果出错返回 -1
    }
}

// 注意这里没有比较数据库与输入分数大小，自行比对
void startscene::updateScore(const QString &name, int score){
    // 更新用户的分数
    QSqlQuery query;
    query.prepare("UPDATE users SET score = ? WHERE username = ?");
    query.addBindValue(score);
    query.addBindValue(name);

    if (query.exec()) {
        qDebug() << "update success: " << name;
    } else {
        qDebug() << "Error: update failed" << name;
    }
}

// 设置背景图片
void startscene::setBackgroundImage(const QString &imagePath) {
    QPixmap backgroundImage(imagePath);
    if (backgroundImage.isNull()) {
        qDebug() << "Error: Failed to load background image!";
        return;
    }

    // 调整图片大小以适应窗口
    backgroundImage = backgroundImage.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // 设置背景图片
    QPalette palette;
    palette.setBrush(QPalette::Window, backgroundImage);
    this->setPalette(palette);
}







