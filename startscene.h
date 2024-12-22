#ifndef STARTSCENE_H
#define STARTSCENE_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QCoreApplication>
#include <QtSql>
#include <QLabel>


class startscene: public QWidget{
    Q_OBJECT

public:
    explicit startscene(QWidget *parent = nullptr);
signals:
    void showMenuWindow();

public slots:
    void login();
    void registerAcc();
    int getScore(const QString &name);
    void updateScore(const QString &name, int score);
    void setBackgroundImage(const QString &imagePath);

private:
    void createDatabase();
    void connectToDatabase();
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    // 在类成员变量中声明 QLabel
    QLabel *userIconLabel;
    QLabel *passwordIconLabel;
};

#endif // STARTSCENE_H
