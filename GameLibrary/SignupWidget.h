#pragma once
#include <QWidget>
#include <QString>

class QLineEdit;
class QPushButton;

class SignupWidget : public QWidget {
    Q_OBJECT
public:
    explicit SignupWidget(QWidget* parent = nullptr);

signals:
    void signupSuccess(const QString& username);
    void backToLogin();

private:
    QLineEdit*  userEdit;
    QLineEdit*  passEdit;
    QPushButton* okBtn;
    QPushButton* backBtn;

private slots:
    void doSignup();
};
