#include "DataStore.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QVector<User> DataStore::loadUsers(const QString& path) {
    QVector<User> out;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) return out;
    auto doc = QJsonDocument::fromJson(f.readAll());
    for (auto v : doc.object()["users"].toArray()) {
        auto o = v.toObject();
        out.append({
            o["username"].toString(),
            o["password"].toString()
        });
    }
    return out;
}

bool DataStore::saveUsers(const QVector<User>& users, const QString& path) {
    QJsonArray arr;
    for (auto& u : users) {
        QJsonObject o;
        o["username"] = u.username;
        o["password"] = u.password;
        arr.append(o);
    }
    QJsonObject root;
    root["users"] = arr;
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly)) return false;
    f.write(QJsonDocument(root).toJson());
    return true;
}

QVector<Game> DataStore::loadGames(const QString& user, const QString& path) {
    QVector<Game> out;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) return out;

    auto doc = QJsonDocument::fromJson(f.readAll());
    QJsonArray arr;

    // If top-level object with "games" key
    if (doc.isObject() && doc.object().contains("games")) {
        arr = doc.object()["games"].toArray();
    }
    // Else if it’s directly an array
    else if (doc.isArray()) {
        arr = doc.array();
    }
    else {
        // unrecognized format
        return out;
    }

    for (auto v : arr) {
        auto o = v.toObject();
        if (o["owner"].toString() != user)
            continue;

        Game g;
        g.owner       = o["owner"].toString();
        g.title       = o["title"].toString();
        g.platform    = o["platform"].toString();
        g.hoursPlayed = o["hoursPlayed"].toInt();
        g.lastPlayed  = o["lastPlayed"].toString();
        g.execPath    = o["execPath"].toString();
        out.append(g);
    }

    return out;
}

bool DataStore::saveGames(const QVector<Game>& games, const QString& username, const QString& path) {
    QJsonArray arr;
    for (auto& g : games) {
        QJsonObject o;
        o["owner"]       = username;
        o["title"]       = g.title;
        o["platform"]    = g.platform;
        o["hoursPlayed"] = g.hoursPlayed;
        o["lastPlayed"]  = g.lastPlayed;
        o["execPath"]    = g.execPath;
        arr.append(o);
    }
    QJsonObject root;
    root["games"] = arr;

    QFile f(path);
    if (!f.open(QIODevice::WriteOnly)) return false;
    f.write(QJsonDocument(root).toJson());
    return true;
}
