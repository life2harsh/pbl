#pragma once
#include <QString>
#include <QVector>

struct User {
    QString username;
    QString password;
};

struct Game {
    QString owner;
    QString title;
    QString platform;
    int hoursPlayed;
    QString lastPlayed;
    QString execPath;
};

class DataStore {
public:
    static QVector<User> loadUsers(const QString& path = "users.json");
    static bool         saveUsers(const QVector<User>& users, const QString& path = "users.json");

    static QVector<Game> loadGames(const QString& user,const QString& path = "games.json");
    static bool          saveGames(const QVector<Game>& games, const QString& username, const QString& path = "games.json");
};
