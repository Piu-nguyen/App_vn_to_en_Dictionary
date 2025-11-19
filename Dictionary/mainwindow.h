#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QMap>
#include <QListWidgetItem>
#include "user.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Word {
    QString english;
    QString vietnamese;
    QString ipa;
    QString example;
    QString topic;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void searchWord();
    void playSound();
    void repeatSound();
    void saveWord();
    void showTopicWords();
    void showWordDetail(QListWidgetItem* item);

    // ← 3 HÀM MỚI ĐÃ THÊM VÀO ĐÂY!!!
    void showSavedWords();
    void reviewSavedWords();
    void deleteSavedWord();
private slots:
    void showTenseDetail(QListWidgetItem* item);
    void startGrammarPractice();
private:
    Ui::MainWindow *ui;
    QString currentWord;
    QVector<Word> allWords;
    QMap<QString, QVector<Word>> topics;
    User currentUser;

    void loadAllData();
    void speak(const QString& text);
    void updateUserStatus();
};

#endif // MAINWINDOW_H
