/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tabSearch;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLineEdit *searchEdit;
    QPushButton *searchButton;
    QListWidget *resultList;
    QWidget *tabTopic;
    QHBoxLayout *hboxLayout1;
    QListWidget *topicList;
    QListWidget *wordList;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout1;
    QLabel *detailEnglish;
    QLabel *detailViet;
    QLabel *detailIPA;
    QLabel *detailExample;
    QLabel *detailTopic;
    QHBoxLayout *hboxLayout2;
    QPushButton *btnPlay;
    QPushButton *btnRepeat;
    QPushButton *btnSave;
    QWidget *tabGrammar;
    QHBoxLayout *hboxLayout3;
    QListWidget *listTenses;
    QTextEdit *textGrammarDetail;
    QWidget *tabPracticeGrammar;
    QVBoxLayout *vboxLayout2;
    QPushButton *btnStartGrammarPractice;
    QHBoxLayout *hboxLayout4;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnShowSaved;
    QPushButton *btnReviewSaved;
    QPushButton *btnDeleteSaved;
    QSpacerItem *horizontalSpacer_2;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1280, 720);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabSearch = new QWidget();
        tabSearch->setObjectName("tabSearch");
        vboxLayout = new QVBoxLayout(tabSearch);
        vboxLayout->setObjectName("vboxLayout");
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName("hboxLayout");
        searchEdit = new QLineEdit(tabSearch);
        searchEdit->setObjectName("searchEdit");

        hboxLayout->addWidget(searchEdit);

        searchButton = new QPushButton(tabSearch);
        searchButton->setObjectName("searchButton");

        hboxLayout->addWidget(searchButton);


        vboxLayout->addLayout(hboxLayout);

        resultList = new QListWidget(tabSearch);
        resultList->setObjectName("resultList");

        vboxLayout->addWidget(resultList);

        tabWidget->addTab(tabSearch, QString());
        tabTopic = new QWidget();
        tabTopic->setObjectName("tabTopic");
        hboxLayout1 = new QHBoxLayout(tabTopic);
        hboxLayout1->setObjectName("hboxLayout1");
        topicList = new QListWidget(tabTopic);
        topicList->setObjectName("topicList");
        topicList->setMaximumWidth(300);

        hboxLayout1->addWidget(topicList);

        wordList = new QListWidget(tabTopic);
        wordList->setObjectName("wordList");

        hboxLayout1->addWidget(wordList);

        groupBox = new QGroupBox(tabTopic);
        groupBox->setObjectName("groupBox");
        vboxLayout1 = new QVBoxLayout(groupBox);
        vboxLayout1->setObjectName("vboxLayout1");
        detailEnglish = new QLabel(groupBox);
        detailEnglish->setObjectName("detailEnglish");

        vboxLayout1->addWidget(detailEnglish);

        detailViet = new QLabel(groupBox);
        detailViet->setObjectName("detailViet");

        vboxLayout1->addWidget(detailViet);

        detailIPA = new QLabel(groupBox);
        detailIPA->setObjectName("detailIPA");

        vboxLayout1->addWidget(detailIPA);

        detailExample = new QLabel(groupBox);
        detailExample->setObjectName("detailExample");

        vboxLayout1->addWidget(detailExample);

        detailTopic = new QLabel(groupBox);
        detailTopic->setObjectName("detailTopic");

        vboxLayout1->addWidget(detailTopic);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setObjectName("hboxLayout2");
        btnPlay = new QPushButton(groupBox);
        btnPlay->setObjectName("btnPlay");

        hboxLayout2->addWidget(btnPlay);

        btnRepeat = new QPushButton(groupBox);
        btnRepeat->setObjectName("btnRepeat");

        hboxLayout2->addWidget(btnRepeat);

        btnSave = new QPushButton(groupBox);
        btnSave->setObjectName("btnSave");

        hboxLayout2->addWidget(btnSave);


        vboxLayout1->addLayout(hboxLayout2);


        hboxLayout1->addWidget(groupBox);

        tabWidget->addTab(tabTopic, QString());
        tabGrammar = new QWidget();
        tabGrammar->setObjectName("tabGrammar");
        hboxLayout3 = new QHBoxLayout(tabGrammar);
        hboxLayout3->setObjectName("hboxLayout3");
        listTenses = new QListWidget(tabGrammar);
        new QListWidgetItem(listTenses);
        new QListWidgetItem(listTenses);
        new QListWidgetItem(listTenses);
        new QListWidgetItem(listTenses);
        new QListWidgetItem(listTenses);
        new QListWidgetItem(listTenses);
        new QListWidgetItem(listTenses);
        new QListWidgetItem(listTenses);
        new QListWidgetItem(listTenses);
        new QListWidgetItem(listTenses);
        new QListWidgetItem(listTenses);
        new QListWidgetItem(listTenses);
        listTenses->setObjectName("listTenses");
        listTenses->setMaximumWidth(350);

        hboxLayout3->addWidget(listTenses);

        textGrammarDetail = new QTextEdit(tabGrammar);
        textGrammarDetail->setObjectName("textGrammarDetail");
        textGrammarDetail->setReadOnly(true);

        hboxLayout3->addWidget(textGrammarDetail);

        tabWidget->addTab(tabGrammar, QString());
        tabPracticeGrammar = new QWidget();
        tabPracticeGrammar->setObjectName("tabPracticeGrammar");
        vboxLayout2 = new QVBoxLayout(tabPracticeGrammar);
        vboxLayout2->setObjectName("vboxLayout2");
        btnStartGrammarPractice = new QPushButton(tabPracticeGrammar);
        btnStartGrammarPractice->setObjectName("btnStartGrammarPractice");
        btnStartGrammarPractice->setMinimumSize(QSize(600, 120));
        QFont font;
        font.setPointSize(28);
        font.setBold(true);
        btnStartGrammarPractice->setFont(font);
        btnStartGrammarPractice->setStyleSheet(QString::fromUtf8("background:#ff6b6b; color:white; border-radius:40px; padding:30px;"));

        vboxLayout2->addWidget(btnStartGrammarPractice, 0, Qt::AlignCenter);

        tabWidget->addTab(tabPracticeGrammar, QString());

        verticalLayout->addWidget(tabWidget);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setObjectName("hboxLayout4");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout4->addItem(horizontalSpacer);

        btnShowSaved = new QPushButton(centralwidget);
        btnShowSaved->setObjectName("btnShowSaved");
        btnShowSaved->setStyleSheet(QString::fromUtf8("background:#4facfe; color:white; padding:15px; font-size:18px; font-weight:bold; border-radius:15px;"));

        hboxLayout4->addWidget(btnShowSaved);

        btnReviewSaved = new QPushButton(centralwidget);
        btnReviewSaved->setObjectName("btnReviewSaved");
        btnReviewSaved->setStyleSheet(QString::fromUtf8("background:#00ff9d; color:white; padding:15px; font-size:18px; font-weight:bold; border-radius:15px;"));

        hboxLayout4->addWidget(btnReviewSaved);

        btnDeleteSaved = new QPushButton(centralwidget);
        btnDeleteSaved->setObjectName("btnDeleteSaved");
        btnDeleteSaved->setStyleSheet(QString::fromUtf8("background:#ff6b6b; color:white; padding:15px; font-size:18px; font-weight:bold; border-radius:15px;"));

        hboxLayout4->addWidget(btnDeleteSaved);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout4->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(hboxLayout4);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "English Dictionary Pro+ 2025", nullptr));
        searchEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Nh\341\272\255p t\341\273\253 ti\341\272\277ng Anh ho\341\272\267c Vi\341\273\207t...", nullptr));
        searchButton->setText(QCoreApplication::translate("MainWindow", "T\303\254m", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabSearch), QCoreApplication::translate("MainWindow", "Tra t\341\273\253 nhanh", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Chi ti\341\272\277t t\341\273\253", nullptr));
        btnPlay->setText(QCoreApplication::translate("MainWindow", "Ph\303\241t \303\242m", nullptr));
        btnRepeat->setText(QCoreApplication::translate("MainWindow", "L\341\272\267p l\341\272\241i", nullptr));
        btnSave->setText(QCoreApplication::translate("MainWindow", "L\306\260u t\341\273\253", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabTopic), QCoreApplication::translate("MainWindow", "Ch\341\273\247 \304\221\341\273\201 t\341\273\253 v\341\273\261ng", nullptr));

        const bool __sortingEnabled = listTenses->isSortingEnabled();
        listTenses->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listTenses->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("MainWindow", "1. Present Simple", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = listTenses->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("MainWindow", "2. Present Continuous", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = listTenses->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("MainWindow", "3. Present Perfect", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = listTenses->item(3);
        ___qlistwidgetitem3->setText(QCoreApplication::translate("MainWindow", "4. Present Perfect Continuous", nullptr));
        QListWidgetItem *___qlistwidgetitem4 = listTenses->item(4);
        ___qlistwidgetitem4->setText(QCoreApplication::translate("MainWindow", "5. Past Simple", nullptr));
        QListWidgetItem *___qlistwidgetitem5 = listTenses->item(5);
        ___qlistwidgetitem5->setText(QCoreApplication::translate("MainWindow", "6. Past Continuous", nullptr));
        QListWidgetItem *___qlistwidgetitem6 = listTenses->item(6);
        ___qlistwidgetitem6->setText(QCoreApplication::translate("MainWindow", "7. Past Perfect", nullptr));
        QListWidgetItem *___qlistwidgetitem7 = listTenses->item(7);
        ___qlistwidgetitem7->setText(QCoreApplication::translate("MainWindow", "8. Past Perfect Continuous", nullptr));
        QListWidgetItem *___qlistwidgetitem8 = listTenses->item(8);
        ___qlistwidgetitem8->setText(QCoreApplication::translate("MainWindow", "9. Future Simple", nullptr));
        QListWidgetItem *___qlistwidgetitem9 = listTenses->item(9);
        ___qlistwidgetitem9->setText(QCoreApplication::translate("MainWindow", "10. Future Continuous", nullptr));
        QListWidgetItem *___qlistwidgetitem10 = listTenses->item(10);
        ___qlistwidgetitem10->setText(QCoreApplication::translate("MainWindow", "11. Future Perfect", nullptr));
        QListWidgetItem *___qlistwidgetitem11 = listTenses->item(11);
        ___qlistwidgetitem11->setText(QCoreApplication::translate("MainWindow", "12. Future Perfect Continuous", nullptr));
        listTenses->setSortingEnabled(__sortingEnabled);

        textGrammarDetail->setHtml(QCoreApplication::translate("MainWindow", "<h2 style=\"color:#4facfe\">Ch\341\273\215n m\341\273\231t th\303\254 \304\221\341\273\203 h\341\273\215c</h2><p style=\"font-size:20px\">Click v\303\240o danh s\303\241ch b\303\252n tr\303\241i \304\221\341\273\203 xem chi ti\341\272\277t!</p>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabGrammar), QCoreApplication::translate("MainWindow", "H\341\273\215c 12 th\303\254", nullptr));
        btnStartGrammarPractice->setText(QCoreApplication::translate("MainWindow", "B\341\272\256T \304\220\341\272\246U \303\224N T\341\272\254P T\341\273\224NG H\341\273\242P", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabPracticeGrammar), QCoreApplication::translate("MainWindow", "\303\224n t\341\272\255p Grammar", nullptr));
        btnShowSaved->setText(QCoreApplication::translate("MainWindow", "Xem t\341\273\253 \304\221\303\243 l\306\260u", nullptr));
        btnReviewSaved->setText(QCoreApplication::translate("MainWindow", "\303\224n t\341\272\255p t\341\273\253 \304\221\303\243 l\306\260u", nullptr));
        btnDeleteSaved->setText(QCoreApplication::translate("MainWindow", "X\303\263a t\341\273\253 \304\221\303\243 l\306\260u", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
