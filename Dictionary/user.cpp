#include "user.h"
#include <QFile>
#include <QTextStream>
#include <QStringConverter>
#include <QMessageBox>
#include <QInputDialog>
#include <QDir>

bool User::login(const QString& user, const QString& pass)
{
    QFile f("users/accounts.txt");
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&f);
        in.setEncoding(QStringConverter::Utf8);
        while (!in.atEnd()) {
            QString line = in.readLine();
            auto parts = line.split(' ');
            if (parts.size() == 2 && parts[0] == user && parts[1] == pass) {
                username = user;
                loadWords();
                return true;
            }
        }
    }
    return false;
}

void User::registerUser(const QString& user, const QString& pass)
{
    QFile f("users/accounts.txt");
    if (f.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&f);
        out.setEncoding(QStringConverter::Utf8);
        out << user << " " << pass << "\n";
    }
    username = user;
}

void User::logout()
{
    username.clear();
    learnedWords.clear();
}

void User::saveWord(const QString& english)
{
    UserWord& w = learnedWords[english];
    w.exp += 10;
    while (w.level < 10 && w.exp >= 100) {
        w.exp -= 100;
        w.level++;
        if (w.level == 10) w.isMastered = true;
    }

    QFile f(getSaveFile());
    if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&f);
        out.setEncoding(QStringConverter::Utf8);
        for (auto it = learnedWords.constBegin(); it != learnedWords.constEnd(); ++it) {
            out << it.key() << "|" << it->level << "|" << it->exp << "|" << (it->isMastered ? "1" : "0") << "\n";
        }
    }
}

void User::loadWords()
{
    learnedWords.clear();
    QFile f(getSaveFile());
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&f);
    in.setEncoding(QStringConverter::Utf8);
    while (!in.atEnd()) {
        QString line = in.readLine();
        auto parts = line.split('|');
        if (parts.size() >= 4) {
            UserWord w;
            w.level = parts[1].toInt();
            w.exp = parts[2].toInt();
            w.isMastered = (parts[3] == "1");
            learnedWords[parts[0]] = w;
        }
    }
}

QString User::getSaveFile() const
{
    return "users/" + username + "_words.txt";
}
