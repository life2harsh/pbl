#pragma once

#include <QWidget>
#include <QString>
#include <vector>

struct UserRecord {
    QString username;
    QString password;
    // … any other fields …
};

class QLineEdit;
class QPushButton;

class LoginWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoginWidget(QWidget* parent = nullptr);

signals:
    void loginSuccess(const QString& username);
    void signupRequested();

private slots:
    void tryLogin();

private:
    QLineEdit*   userEdit;
    QLineEdit*   passEdit;
    QPushButton* loginBtn;
    QPushButton* signupBtn;
};
