#include "database.h"
#include <QDebug>

Database::Database() {
    db = QSqlDatabase::addDatabase("QSQLITE");
}

Database::~Database() {
    if (db.isOpen()) {
        db.close();
    }
}

bool Database::connectToDatabase() {
    db.setDatabaseName("player_data.db");

    if (!db.open()) {
        qDebug() << "Error: failed to connect to database!";
        return false;
    }
    return true;
}

bool Database::createDatabase() {
    QSqlQuery query;
    bool success = query.exec("CREATE TABLE IF NOT EXISTS users ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "username TEXT UNIQUE NOT NULL, "
                              "password TEXT NOT NULL, "
                              "score INTEGER)");
    if (!success) {
        qDebug() << "Error: failed to create table!";
    }
    return success;
}

int Database::getScore(const QString &name) {
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

void Database::updateScore(const QString &name, int score) {
    QSqlQuery query;
    query.prepare("UPDATE users SET score = ? WHERE username = ?");
    query.addBindValue(score);
    query.addBindValue(name);

    if (!query.exec()) {
        qDebug() << "Error: update failed for " << name;
    } else {
        qDebug() << "Update success for " << name;
    }
}
