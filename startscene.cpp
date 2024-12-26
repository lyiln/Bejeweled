#include "startscene.h"
#include "high_score.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QCoreApplication>
#include <QtSql>
#include <QMessageBox>
#include <QDebug>
#include <QPalette>
#include <QPixmap>

startscene::startscene(QWidget* parent) : QWidget(parent) {
    db.connectToDatabase();  // 连接数据库

    setFixedSize(740, 480);

    // 设置背景图片
    QString backgroundImagePath = ":/images/loginBackground.png";
    setBackgroundImage(backgroundImagePath);

    // 创建用户和密码图片标签
    userIconLabel = new QLabel(this);
    passwordIconLabel = new QLabel(this);

    // 加载图片并设置到标签
    QPixmap userPixmap(":/images/account.png");
    QPixmap passwordPixmap(":/images/password.png");
    userIconLabel->setPixmap(userPixmap);
    passwordIconLabel->setPixmap(passwordPixmap);

    // 设置图片标签为透明背景
    userIconLabel->setAttribute(Qt::WA_TranslucentBackground);
    passwordIconLabel->setAttribute(Qt::WA_TranslucentBackground);

    // 创建输入框
    usernameLineEdit = new QLineEdit(this);
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    // 设置输入框宽度
    int textBoxWidth = 300;
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
    int iconLeftMargin = 100;
    int iconTopMargin = 220;

    userIconLabel->move(iconLeftMargin, 100);
    passwordIconLabel->move(iconLeftMargin, 220);

    userIconLabel->show();
    passwordIconLabel->show();

    // 创建水平间距，使输入框和按钮居中
    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    // 添加输入框到主布局
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

    // 创建按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addItem(horizontalSpacer);
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);
    buttonLayout->addItem(horizontalSpacer);

    mainLayout->addLayout(buttonLayout);

    // 连接信号和槽
    connect(loginButton, &QPushButton::clicked, this, &startscene::login);
    connect(registerButton, &QPushButton::clicked, this, &startscene::registerAcc);
    //界面创建完毕
}

// 登录逻辑
void startscene::login() {
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();

    userAccName = username;

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = ? AND password = ?");
    query.addBindValue(username);
    query.addBindValue(password);

    if (query.exec() && query.next()) {
        QMessageBox::information(this, "登录成功", "欢迎 " + username);
        emit showMenuWindow();
        this->close();
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误！");
    }
}

// 注册逻辑
void startscene::registerAcc() {
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "用户名和密码不能为空！");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE username = ?");
    query.addBindValue(username);

    if (query.exec() && query.next()) {
        QMessageBox::critical(this, "注册失败", "用户名已存在，注册失败！");
    } else {
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

// 设置背景图片
void startscene::setBackgroundImage(const QString &imagePath) {
    QPixmap backgroundImage(imagePath);
    if (backgroundImage.isNull()) {
        qDebug() << "Error: Failed to load background image!";
        return;
    }

    backgroundImage = backgroundImage.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Window, backgroundImage);
    this->setPalette(palette);
}





