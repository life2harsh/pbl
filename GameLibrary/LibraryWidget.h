#pragma once

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QString>

class LibraryWidget : public QWidget {
    Q_OBJECT

public:
    /// Creates the library view.  Call load(username) after login.
    explicit LibraryWidget(QWidget *parent = nullptr);

    /// Populates the table with all games owned by `username`.
    void load(const QString &username);

signals:
    /// Emitted when the user clicks “Logout”
    void logoutRequested();

private:
    QTableView*          view;   // the table view
    QStandardItemModel*  model;  // the backing model (5 columns)
};
