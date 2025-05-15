#include "LoginWidget.h"
#include "DataStore.h"       // must declare: std::vector<UserRecord> DataStore::loadUsers();
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

LoginWidget::LoginWidget(QWidget* parent)
    : QWidget(parent)
    , userEdit  (new QLineEdit(this))
    , passEdit  (new QLineEdit(this))
    , loginBtn  (new QPushButton("Login", this))
    , signupBtn (new QPushButton("Sign Up", this))
{
    userEdit->setPlaceholderText("Username");
    passEdit->setPlaceholderText("Password");
    passEdit->setEchoMode(QLineEdit::Password);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(userEdit);
    layout->addWidget(passEdit);
    layout->addWidget(loginBtn);
    layout->addWidget(signupBtn);

    connect(loginBtn,  &QPushButton::clicked, this, &LoginWidget::tryLogin);
    connect(signupBtn, &QPushButton::clicked, this, &LoginWidget::signupRequested);
}

void LoginWidget::tryLogin() {
    QString u = userEdit->text();
    QString p = passEdit->text();
    if (u.isEmpty() || p.isEmpty()) {
        QMessageBox::warning(this, "Login", "Please enter both username and password.");
        return;
    }

    auto users = DataStore::loadUsers();
    for (const auto& rec : users) {
        if (rec.username == u && rec.password == p) {
            emit loginSuccess(u);
            return;
        }
    }

    QMessageBox::warning(this, "Login Failed", "Invalid credentials.");
}
