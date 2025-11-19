#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

QString LoginDialog::username() const
{
    return ui->usernameEdit->text().trimmed();
}

QString LoginDialog::password() const
{
    return ui->passwordEdit->text();
}

void LoginDialog::on_loginButton_clicked()
{
    if (username().isEmpty() || password().isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập đầy đủ!");
        return;
    }
    accept(); // OK
}

void LoginDialog::on_registerButton_clicked()
{
    on_loginButton_clicked(); // giống hệt, chỉ khác ý nghĩa khi xử lý bên ngoài
    accept();
}

void LoginDialog::on_guestButton_clicked()
{
    reject(); // Cancel = dùng khách
}
