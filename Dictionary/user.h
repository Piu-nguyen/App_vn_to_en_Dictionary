#ifndef USER_H
#define USER_H

#include <QString>
#include <QMap>

struct UserWord {
    int level = 0;
    int exp = 0;
    bool isMastered = false;
};

class User
{
public:
    QString username;
    QMap<QString, UserWord> learnedWords;

    bool login(const QString& user, const QString& pass);
    void registerUser(const QString& user, const QString& pass);
    void logout();
    void saveWord(const QString& english);
    void loadWords();
    QString getSaveFile() const;

    // THÊM HÀM NÀY ĐỂ TÍNH TỔNG EXP
    int getTotalEXP() const {
        int total = 0;
        for (const auto& pair : learnedWords) {
            total += pair.exp + pair.level * 100;
        }
        return total;
    }
};

#endif
