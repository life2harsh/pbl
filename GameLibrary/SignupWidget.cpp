#include "SignupWidget.h"
#include "DataStore.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

SignupWidget::SignupWidget(QWidget* parent) : QWidget(parent) {
    userEdit = new QLineEdit;  userEdit->setPlaceholderText("Choose username");
    passEdit = new QLineEdit;  passEdit->setPlaceholderText("Choose password");
    passEdit->setEchoMode(QLineEdit::Password);
    okBtn   = new QPushButton("Sign Up");
    backBtn = new QPushButton("Back");

    auto* layout = new QVBoxLayout;
    layout->addWidget(userEdit);
    layout->addWidget(passEdit);
    layout->addWidget(okBtn);
    layout->addWidget(backBtn);
    setLayout(layout);

    connect(okBtn,   &QPushButton::clicked, this, &SignupWidget::doSignup);
    connect(backBtn, &QPushButton::clicked, this, &SignupWidget::backToLogin);
}

void SignupWidget::doSignup() {
    auto users = DataStore::loadUsers();
    for (auto& u : users) {
        if (u.username == userEdit->text()) {
            QMessageBox::warning(this, "Exists", "Username already taken");
            return;
        }
    }
    users.append({ userEdit->text(), passEdit->text() });
    if (!DataStore::saveUsers(users)) {
        QMessageBox::critical(this, "Error", "Cannot write user file");
        return;
    }
    emit signupSuccess(userEdit->text());
}
