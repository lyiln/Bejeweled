#ifndef STARTSCENE_H
#define STARTSCENE_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QCoreApplication>
#include <QtSql>
#include <QLabel>
#include "database.h"  // 引入数据库类

class startscene : public QWidget {
    Q_OBJECT

public:
    explicit startscene(QWidget *parent = nullptr);

signals:
    void showMenuWindow();

public slots:
    void login();
    void registerAcc();
    void setBackgroundImage(const QString &imagePath);

private:
    Database db;  // 创建 Database 类的对象
    void createDatabase();
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QLabel *userIconLabel;
    QLabel *passwordIconLabel;
};

#endif // STARTSCENE_H
