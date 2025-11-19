/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QHBoxLayout *hboxLayout;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QPushButton *guestButton;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName("LoginDialog");
        LoginDialog->resize(400, 300);
        LoginDialog->setStyleSheet(QString::fromUtf8("\n"
"    QDialog { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #667eea, stop:1 #764ba2); }\n"
"    QLabel { color: white; font-size: 16px; }\n"
"    QLineEdit { padding: 12px; border-radius: 10px; font-size: 16px; }\n"
"    QPushButton { background: #ff6b6b; color: white; padding: 12px; border-radius: 10px; font-weight: bold; font-size: 16px; }\n"
"    QPushButton:hover { background: #ee5a52; }\n"
"   "));
        verticalLayout = new QVBoxLayout(LoginDialog);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(LoginDialog);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        usernameEdit = new QLineEdit(LoginDialog);
        usernameEdit->setObjectName("usernameEdit");

        verticalLayout->addWidget(usernameEdit);

        passwordEdit = new QLineEdit(LoginDialog);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(passwordEdit);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName("hboxLayout");
        loginButton = new QPushButton(LoginDialog);
        loginButton->setObjectName("loginButton");

        hboxLayout->addWidget(loginButton);

        registerButton = new QPushButton(LoginDialog);
        registerButton->setObjectName("registerButton");

        hboxLayout->addWidget(registerButton);


        verticalLayout->addLayout(hboxLayout);

        guestButton = new QPushButton(LoginDialog);
        guestButton->setObjectName("guestButton");
        guestButton->setStyleSheet(QString::fromUtf8("background: transparent; color: #a0a0a0;"));

        verticalLayout->addWidget(guestButton);


        retranslateUi(LoginDialog);

        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QCoreApplication::translate("LoginDialog", "\304\220\304\203ng nh\341\272\255p / \304\220\304\203ng k\303\275", nullptr));
        label->setText(QCoreApplication::translate("LoginDialog", "<h1>English Dictionary Pro+</h1>", nullptr));
        usernameEdit->setPlaceholderText(QCoreApplication::translate("LoginDialog", "T\303\252n ng\306\260\341\273\235i d\303\271ng", nullptr));
        passwordEdit->setPlaceholderText(QCoreApplication::translate("LoginDialog", "M\341\272\255t kh\341\272\251u", nullptr));
        loginButton->setText(QCoreApplication::translate("LoginDialog", "\304\220\304\203ng nh\341\272\255p", nullptr));
        registerButton->setText(QCoreApplication::translate("LoginDialog", "\304\220\304\203ng k\303\275", nullptr));
        guestButton->setText(QCoreApplication::translate("LoginDialog", "D\303\271ng v\341\273\233i t\306\260 c\303\241ch kh\303\241ch (kh\303\264ng l\306\260u)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
