#include <QApplication>
#include <QStackedWidget>
#include "LoginWidget.h"
#include "SignupWidget.h"
#include "LibraryWidget.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    auto* stack  = new QStackedWidget;
    auto* login  = new LoginWidget;
    auto* signup = new SignupWidget;
    auto* lib    = new LibraryWidget;

    stack->addWidget(login);
    stack->addWidget(signup);
    stack->addWidget(lib);

    // Wire transitions
    QObject::connect(login,  &LoginWidget::signupRequested, [=](){
        stack->setCurrentWidget(signup);
    });
    QObject::connect(signup, &SignupWidget::backToLogin,   [=](){
        stack->setCurrentWidget(login);
    });
    QObject::connect(signup, &SignupWidget::signupSuccess, [=](auto u){
        lib->load(u);
        stack->setCurrentWidget(lib);
    });
    QObject::connect(login,  &LoginWidget::loginSuccess,  [=](auto u){
        lib->load(u);
        stack->setCurrentWidget(lib);
    });
    QObject::connect(lib,    &LibraryWidget::logoutRequested, [=](){
        stack->setCurrentWidget(login);
    });
    stack->setMinimumSize(400, 300);
    stack->setMaximumSize(1200, 900);
    stack->show();
    return app.exec();
}
