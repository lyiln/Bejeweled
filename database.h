#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>

class Database {
public:
    Database();
    ~Database();

    bool connectToDatabase();
    bool createDatabase();
    int getScore(const QString &name);
    void updateScore(const QString &name, int score);

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
