#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include <QFile>
#include <QTextStream>
#include <QStringConverter>
#include <QMessageBox>
#include <QDir>
#include <QThread>
#include <QProcess>
#include <QInputDialog>
#include <algorithm>
#include <cmath>
#include <random>
#include <string>
#include <QRandomGenerator>
// Hàm normalize đơn giản (bỏ dấu, chuyển thường)
QString normalize(const QString& s)
{
    QString temp = s.normalized(QString::NormalizationForm_KD);
    temp = temp.toLower();
    QString result;
    for (QChar c : temp) {
        if (!c.isMark()) result += c;
    }
    return result;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("English Dictionary Pro+ 2025");

    loadAllData();

    // ==================== ĐĂNG NHẬP / ĐĂNG KÝ ====================
    LoginDialog loginDlg(this);
    if (loginDlg.exec() == QDialog::Accepted) {
        QString user = loginDlg.username().trimmed();
        QString pass = loginDlg.password();

        if (currentUser.login(user, pass)) {
            QMessageBox::information(this, "Chào mừng!", "Chào mừng trở lại, " + user + "!");
        } else {
            currentUser.registerUser(user, pass);
            QMessageBox::information(this, "Thành công", "Đăng ký thành công! Chào " + user + "!");
        }
    } else {
        QMessageBox::information(this, "Chế độ khách", "Bạn đang dùng chế độ khách\nTiến độ sẽ không được lưu.");
    }

    setWindowTitle("English Dictionary Pro+ - " + (currentUser.username.isEmpty() ? "Khách" : currentUser.username));
    updateUserStatus();

    // =================================================================
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::searchWord);
    connect(ui->searchEdit, &QLineEdit::returnPressed, this, &MainWindow::searchWord);
    connect(ui->topicList, &QListWidget::itemClicked, this, &MainWindow::showTopicWords);
    connect(ui->wordList, &QListWidget::itemDoubleClicked, this, &MainWindow::showWordDetail);
    connect(ui->resultList, &QListWidget::itemClicked, this, &MainWindow::showWordDetail);
    connect(ui->btnPlay, &QPushButton::clicked, this, &MainWindow::playSound);
    connect(ui->btnRepeat, &QPushButton::clicked, this, &MainWindow::repeatSound);
    connect(ui->btnSave, &QPushButton::clicked, this, &MainWindow::saveWord);

    // 3 NÚT MỚI
    connect(ui->btnShowSaved, &QPushButton::clicked, this, &MainWindow::showSavedWords);
    connect(ui->btnReviewSaved, &QPushButton::clicked, this, &MainWindow::reviewSavedWords);
    connect(ui->btnDeleteSaved, &QPushButton::clicked, this, &MainWindow::deleteSavedWord);

    // 2 NÚT GRAMMAR MỚI
    connect(ui->listTenses, &QListWidget::itemClicked, this, &MainWindow::showTenseDetail);
    connect(ui->btnStartGrammarPractice, &QPushButton::clicked, this, &MainWindow::startGrammarPractice);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::loadAllData()
{
    allWords.clear();
    topics.clear();

    QDir dir("Data");
    if (!dir.exists()) {
        statusBar()->showMessage("Không tìm thấy thư mục Data!");
        return;
    }

    QStringList filters = {"*.csv", "*.txt"};
    for (const QFileInfo& fileInfo : dir.entryInfoList(filters, QDir::Files)) {
        QFile file(fileInfo.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) continue;

        QTextStream in(&file);
        in.setEncoding(QStringConverter::Utf8);
        in.readLine(); // bỏ header

        QString topicName = fileInfo.baseName();

        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;

            QStringList cols = line.split(',');
            if (cols.size() < 4) continue;

            Word w;
            w.english    = cols[0].trimmed();
            w.vietnamese = cols[1].trimmed();
            w.ipa        = cols[2].trimmed();
            w.example    = cols.mid(3).join(",").trimmed();
            w.topic      = topicName;

            allWords.append(w);
            topics[topicName].append(w);
        }
        file.close();
    }

    ui->topicList->clear();
    for (auto it = topics.constBegin(); it != topics.constEnd(); ++it) {
        QString niceName = it.key();
        if (!niceName.isEmpty()) niceName[0] = niceName[0].toUpper();
        ui->topicList->addItem(niceName + QString(" (%1 từ)").arg(it.value().size()));
    }

    statusBar()->showMessage(QString("Đã tải %1 từ • %2 chủ đề").arg(allWords.size()).arg(topics.size()));
}

void MainWindow::showTopicWords()
{
    auto item = ui->topicList->currentItem();
    if (!item) return;

    QString topic = item->text().split('(').first().trimmed();
    ui->wordList->clear();

    for (const auto& w : topics[topic]) {
        ui->wordList->addItem(w.english + " → " + w.vietnamese);
    }
}

void MainWindow::showWordDetail(QListWidgetItem* item)
{
    if (!item) return;
    QString eng = item->text().split(" → ").first().trimmed();
    for (const auto& w : allWords) {
        if (w.english == eng) {
            currentWord = w.english;

            // Từ chính - to, đậm, màu xanh đẹp
            ui->detailEnglish->setText(
                "<h1 style='color:#4facfe; font-weight:bold; margin:10px 0'>" + w.english + "</h1>"
                );

            // Nghĩa - CHỮ ĐEN ĐẬM, FONT TO, RÕ RÀNG 100%
            ui->detailViet->setText(
                "<p style='color:black; font-size:18px; font-weight:bold; margin:5px 0'>Nghĩa:</p>"
                "<p style='color:black; font-size:19px; line-height:1.5; margin:5px 0'>" + w.vietnamese + "</p>"
                );

            // Phiên âm - màu đỏ nổi, font lớn
            ui->detailIPA->setText(
                "<p style='color:black; font-size:16px; font-weight:bold; margin:5px 0'>Phiên âm:</p>"
                "<p style='color:#e74c3c; font-size:22px; font-family:Arial; margin:5px 0'>" + w.ipa + "</p>"
                );

            // Ví dụ - CHỮ ĐEN ĐẬM, KHÔNG MỜ NỮA!
            ui->detailExample->setText(
                "<p style='color:black; font-size:16px; font-weight:bold; margin:5px 0'>Ví dụ:</p>"
                "<p style='color:black; font-size:17px; line-height:1.6; font-style:italic; margin:5px 0 15px 0'>"
                + w.example.toHtmlEscaped().replace("\n", "<br>") + "</p>"
                );

            // Chủ đề - màu đỏ, đậm
            ui->detailTopic->setText(
                "<p style='color:black; font-weight:bold; margin:5px 0'>Chủ đề:</p>"
                "<span style='color:#e74c3c; font-weight:bold'>" + w.topic.toUpper() + "</span>"
                );

            speak(w.english);
            return;
        }
    }
}

void MainWindow::searchWord()
{
    QString key = ui->searchEdit->text().trimmed().toLower();
    ui->resultList->clear();

    for (const auto& w : allWords) {
        if (w.english.toLower().contains(key) || w.vietnamese.toLower().contains(key)) {
            ui->resultList->addItem(w.english + " → " + w.vietnamese);
        }
    }

    if (ui->resultList->count() == 0) {
        ui->resultList->addItem("Không tìm thấy từ nào :(");
    } else {
        ui->resultList->setCurrentRow(0);
        showWordDetail(ui->resultList->currentItem());
    }
}

void MainWindow::speak(const QString& text)
{
#ifdef Q_OS_WIN
    QString safeText = text;
    safeText.replace("\"", "\\\"");
    QString cmd = QString("powershell -Command \"Add-Type -AssemblyName System.Speech; "
                          "$s=New-Object System.Speech.Synthesis.SpeechSynthesizer; "
                          "$s.SelectVoice('Microsoft Zira Desktop'); "
                          "$s.Speak('%1')\"").arg(safeText);
    QProcess::execute(cmd);
#endif
}

void MainWindow::playSound() { if (!currentWord.isEmpty()) speak(currentWord); }

void MainWindow::repeatSound()
{
    if (currentWord.isEmpty()) return;
    for (int i = 0; i < 20; ++i) {
        speak(currentWord);
        QThread::msleep(1200);
        qApp->processEvents();
    }
}

void MainWindow::saveWord()
{
    if (currentWord.isEmpty()) return;
    if (currentUser.username.isEmpty()) {
        QMessageBox::warning(this, "Chưa đăng nhập", "Vui lòng đăng nhập để lưu từ!");
        return;
    }

    const Word* fullWord = nullptr;
    for (const auto& w : allWords) {
        if (w.english == currentWord) {
            fullWord = &w;
            break;
        }
    }
    if (!fullWord) return;

    if (currentUser.learnedWords.contains(currentWord)) {
        UserWord& uw = currentUser.learnedWords[currentWord];
        uw.exp += 10;

        while (uw.level < 10 && uw.exp >= (uw.level < 5 ? 100 : static_cast<int>(100 * std::pow(1.2, uw.level - 4)))) {
            uw.exp -= (uw.level < 5 ? 100 : static_cast<int>(100 * std::pow(1.2, uw.level - 4)));
            uw.level++;
            if (uw.level == 10) {
                uw.isMastered = true;
                QMessageBox::information(this, "THÔNG THÁO!", QString("CHÚC MỪNG!\nTừ <b>%1</b> đã THÔNG THÁO! ★").arg(currentWord));
            }
        }
        QMessageBox::information(this, "Cập nhật", QString("Ôn lại \"%1\"\n+10 EXP → Level %2").arg(currentWord).arg(uw.level));
    } else {
        UserWord uw;
        uw.level = 1;
        uw.exp = 10;
        uw.isMastered = false;
        currentUser.learnedWords[currentWord] = uw;
        QMessageBox::information(this, "Thành công", QString("Đã lưu từ mới \"%1\"\n+10 EXP → Level 1").arg(currentWord));
    }

    currentUser.saveWord(currentWord);
    updateUserStatus();
}

void MainWindow::updateUserStatus()
{
    if (currentUser.username.isEmpty()) {
        statusBar()->showMessage("Chế độ khách");
        return;
    }

    int totalEXP = currentUser.getTotalEXP();
    int mastered = 0;
    for (auto it = currentUser.learnedWords.constBegin(); it != currentUser.learnedWords.constEnd(); ++it) {
        if (it.value().isMastered) mastered++;
    }

    statusBar()->showMessage(QString("Người dùng: %1 • EXP: %2 • Thông thạo: %3 từ")
                                 .arg(currentUser.username).arg(totalEXP).arg(mastered));
}

// ==================== XEM TỪ ĐÃ LƯU ====================
void MainWindow::showSavedWords()
{
    if (currentUser.username.isEmpty()) {
        QMessageBox::information(this, "Từ đã lưu", "Bạn đang ở chế độ khách - không có từ đã lưu!");
        return;
    }

    if (currentUser.learnedWords.isEmpty()) {
        QMessageBox::information(this, "Từ đã lưu", "Bạn chưa lưu từ nào!");
        return;
    }

    QString info = QString("<h2 style='color:#4facfe'>TỪ ĐÃ LƯU CỦA %1</h2>").arg(currentUser.username);
    info += "<table border='1' style='width:100%; border-collapse:collapse;'><tr style='background:#333; color:white;'>"
            "<th>STT</th><th>Từ</th><th>Nghĩa</th><th>Level</th><th>EXP</th><th>Trạng thái</th></tr>";

    int stt = 1;
    for (auto it = currentUser.learnedWords.constBegin(); it != currentUser.learnedWords.constEnd(); ++it) {
        const QString& eng = it.key();
        const UserWord& uw = it.value();

        QString meaning = "???";
        for (const auto& w : allWords) {
            if (w.english == eng) {
                meaning = w.vietnamese;
                break;
            }
        }

        QString status;
        if (uw.isMastered) status = "<b style='color:gold'>THÔNG THÁO ★</b>";
        else if (uw.level >= 6) status = "<span style='color:#00ff9d'>Nâng cao</span>";
        else status = "<span style='color:#ff6b6b'>Cơ bản</span>";

        int nextEXP = (uw.level < 10) ? (uw.level < 5 ? 100 : static_cast<int>(100 * std::pow(1.2, uw.level - 4))) : 0;

        info += QString("<tr><td align='center'>%1</td><td><b>%2</b></td><td>%3</td><td align='center'>%4</td><td>%5/%6</td><td>%7</td></tr>")
                    .arg(stt++)
                    .arg(eng)
                    .arg(meaning)
                    .arg(uw.level)
                    .arg(uw.exp)
                    .arg(nextEXP)
                    .arg(status);
    }
    info += "</table>";

    QMessageBox msg(this);
    msg.setWindowTitle("Từ đã lưu - " + currentUser.username);
    msg.setTextFormat(Qt::RichText);
    msg.setText(info);
    msg.setMinimumWidth(900);
    msg.setMinimumHeight(600);
    msg.exec();
}

// ==================== ÔN TẬP TỪ ĐÃ LƯU – SCORE ĐÚNG 100% ====================
void MainWindow::reviewSavedWords()
{
    if (currentUser.username.isEmpty()) {
        QMessageBox::warning(this, "Chưa đăng nhập", "Vui lòng đăng nhập để ôn từ đã lưu!");
        return;
    }

    if (currentUser.learnedWords.isEmpty()) {
        QMessageBox::information(this, "Trống", "Bạn chưa lưu từ nào để ôn!");
        return;
    }

    QVector<QString> savedEng;
    for (auto it = currentUser.learnedWords.constBegin(); it != currentUser.learnedWords.constEnd(); ++it) {
        if (!it.value().isMastered) {
            savedEng.append(it.key());
        }
    }

    if (savedEng.isEmpty()) {
        QMessageBox::information(this, "Tuyệt vời!", "Bạn đã THÔNG THÁO hết từ rồi!\nGiỏi quá! ★★★");
        return;
    }

    std::sort(savedEng.begin(), savedEng.end(), [this](const QString& a, const QString& b) {
        return currentUser.learnedWords[a].level < currentUser.learnedWords[b].level;
    });

    if (savedEng.size() > 10) savedEng.resize(10);

    // Hỏi kiểu ôn tập
    QMessageBox modeBox(this);
    modeBox.setWindowTitle("Chọn kiểu ôn tập");
    modeBox.setText("Chọn kiểu ôn tập từ đã lưu:");
    QPushButton* btnFlash = modeBox.addButton("1. Flashcard (Anh → Việt)", QMessageBox::YesRole);
    QPushButton* btnMCQ = modeBox.addButton("2. Trắc nghiệm (4 đáp án)", QMessageBox::NoRole);
    QPushButton* btnFill = modeBox.addButton("3. Điền vào chỗ trống", QMessageBox::RejectRole);
    QPushButton* btnListen = modeBox.addButton("4. Nghe và viết chính tả", QMessageBox::HelpRole);
    QPushButton* btnExit = modeBox.addButton("0. Thoát", QMessageBox::RejectRole);

    modeBox.exec();
    QPushButton* clicked = qobject_cast<QPushButton*>(modeBox.clickedButton());
    if (!clicked || clicked == btnExit) return;

    int mode = 0;
    if (clicked == btnFlash) mode = 1;
    else if (clicked == btnMCQ) mode = 2;
    else if (clicked == btnFill) mode = 3;
    else if (clicked == btnListen) mode = 4;

    int score = 0;  // ← ĐÃ ĐƯA RA NGOÀI VÒNG LẶP → SCORE TĂNG ĐÚNG!!!

    for (int i = 0; i < savedEng.size(); ++i) {
        QString eng = savedEng[i];
        const Word* wordPtr = nullptr;
        for (const auto& w : allWords) {
            if (w.english == eng) {
                wordPtr = &w;
                break;
            }
        }
        if (!wordPtr) continue;

        const Word& w = *wordPtr;
        currentWord = w.english;

        bool correct = false;

        if (mode == 1) { // Flashcard
            QMessageBox msg(this);
            msg.setWindowTitle(QString("Ôn tập [%1/%2] - Flashcard").arg(i+1).arg(savedEng.size()));
            msg.setText(QString("<h2>%1</h2>").arg(w.english));
            msg.addButton("Hiện nghĩa", QMessageBox::YesRole);
            msg.exec();

            QMessageBox msg2(this);
            msg2.setText(QString("<h2>%1</h2><p><b>Nghĩa:</b> %2</p><p><b>IPA:</b> %3</p><p><b>Ví dụ:</b> %4</p>")
                             .arg(w.english).arg(w.vietnamese).arg(w.ipa).arg(w.example));

            QPushButton* btnRemember = msg2.addButton("Đã nhớ", QMessageBox::YesRole);
            QPushButton* btnForget = msg2.addButton("Còn quên", QMessageBox::NoRole);
            speak(w.english);

            msg2.exec();
            QPushButton* clicked2 = qobject_cast<QPushButton*>(msg2.clickedButton());
            correct = (clicked2 == btnRemember);
            if(correct)score ++;
        }
        else if (mode == 2) { // Trắc nghiệm
            QVector<QString> options = {w.vietnamese};
            QVector<Word> others = allWords;
            others.erase(std::remove_if(others.begin(), others.end(), [&w](const Word& ww){ return ww.english == w.english; }), others.end());
            std::shuffle(others.begin(), others.end(), std::mt19937(std::random_device()()));

            for (int j = 0; j < 3 && j < others.size(); ++j) options.append(others[j].vietnamese);
            std::shuffle(options.begin(), options.end(), std::mt19937(std::random_device()()));

            QMessageBox msg(this);
            msg.setWindowTitle(QString("Ôn tập [%1/%2] - Trắc nghiệm").arg(i+1).arg(savedEng.size()));
            QString text = QString("Nghĩa của <b>%1</b> là gì?\n\n").arg(w.english);
            for (int j = 0; j < options.size(); ++j) text += QString("%1. %2\n").arg(j+1).arg(options[j]);
            msg.setText(text);

            QPushButton* btn1 = msg.addButton("1", QMessageBox::ActionRole);
            QPushButton* btn2 = msg.addButton("2", QMessageBox::ActionRole);
            QPushButton* btn3 = msg.addButton("3", QMessageBox::ActionRole);
            QPushButton* btn4 = msg.addButton("4", QMessageBox::ActionRole);
            msg.addButton("Bỏ qua", QMessageBox::RejectRole);

            msg.exec();
            QPushButton* clicked = qobject_cast<QPushButton*>(msg.clickedButton());

            int ans = -1;
            if (clicked == btn1) ans = 1;
            else if (clicked == btn2) ans = 2;
            else if (clicked == btn3) ans = 3;
            else if (clicked == btn4) ans = 4;

            int correctIdx = options.indexOf(w.vietnamese) + 1;
            correct = (ans == correctIdx);

            if (correct) {
                QMessageBox::information(this, "Đúng!", "Giỏi quá!");
                score++;
            } else if (ans != -1) {
                QMessageBox::information(this, "Sai!", QString("Đáp án đúng: %1. %2").arg(correctIdx).arg(w.vietnamese));
            }
        }
        else if (mode == 3) { // Điền từ
            QString sentence = w.example;
            size_t pos = sentence.toStdString().find(w.english.toStdString());
            if (pos != std::string::npos) {
                sentence.replace(pos, w.english.length(), "_____");
            } else {
                sentence = "I _____ every day.";
            }

            bool ok;
            QString ans = QInputDialog::getText(this, QString("Ôn tập [%1/%2]").arg(i+1).arg(savedEng.size()),
                                                sentence + "\n→ Điền từ:", QLineEdit::Normal, "", &ok);
            if (!ok) continue;

            correct = (normalize(ans) == normalize(w.english));
            if (correct) {
                QMessageBox::information(this, "Đúng!", "Tuyệt vời!");
                score++;
            } else {
                QMessageBox::information(this, "Sai!", "Đáp án: " + w.english);
            }
        }
        else if (mode == 4) { // Nghe viết
            QMessageBox msg(this);
            msg.setText("Nghe kỹ và viết lại từ:\n\n(Nhấn OK để nghe 2 lần)");
            msg.addButton(QMessageBox::Ok);
            msg.exec();

            speak(w.english);
            QThread::msleep(800);
            speak(w.english);

            bool ok;
            QString ans = QInputDialog::getText(this, "Viết từ bạn nghe được", "→ Từ là:", QLineEdit::Normal, "", &ok);
            if (!ok) continue;

            correct = (normalize(ans) == normalize(w.english));
            if (correct) {
                QMessageBox::information(this, "Đúng!", "Tai nghe siêu đấy!");
                score++;
            } else {
                QMessageBox::information(this, "Sai!", "Từ đúng là: " + w.english);
            }
        }

        // Cập nhật EXP
        if (correct) {
            currentUser.learnedWords[eng].exp += 5;
        } else {
            currentUser.learnedWords[eng].exp += 2;
        }

        // Cập nhật level và kiểm tra thông thạo
        UserWord& uw = currentUser.learnedWords[eng];
        while (uw.level < 10 && uw.exp >= (uw.level < 5 ? 100 : static_cast<int>(100 * std::pow(1.2, uw.level - 4)))) {
            uw.exp -= (uw.level < 5 ? 100 : static_cast<int>(100 * std::pow(1.2, uw.level - 4)));
            uw.level++;
            if (uw.level == 10) {
                uw.isMastered = true;
                QMessageBox::information(this, "THÔNG THÁO!", QString("CHÚC MỪNG!\nTừ <b>%1</b> đã đạt THÔNG THÁO! ★").arg(w.english));
            }
        }
        currentUser.saveWord(eng);
    }

    QMessageBox::information(this, "Hoàn thành!", QString("Ôn xong %1 từ!\nBạn nhớ đúng: %2/%3\nTiếp tục cố lên nhé!").arg(savedEng.size()).arg(score).arg(savedEng.size()));
    updateUserStatus();
}

// ==================== XÓA TỪ ĐÃ LƯU ====================
void MainWindow::deleteSavedWord()
{
    if (currentUser.username.isEmpty()) {
        QMessageBox::warning(this, "Chưa đăng nhập", "Vui lòng đăng nhập để xóa từ!");
        return;
    }

    bool ok;
    QString eng = QInputDialog::getText(this, "Xóa từ", "Nhập từ tiếng Anh muốn xóa:", QLineEdit::Normal, "", &ok);
    if (!ok || eng.isEmpty()) return;

    if (currentUser.learnedWords.remove(eng)) {
        QFile f(currentUser.getSaveFile());
        if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&f);
            out.setEncoding(QStringConverter::Utf8);
            for (auto it = currentUser.learnedWords.constBegin(); it != currentUser.learnedWords.constEnd(); ++it) {
                out << it.key() << "|" << it.value().level << "|" << it.value().exp << "|" << (it.value().isMastered ? "1" : "0") << "\n";
            }
        }
        QMessageBox::information(this, "Thành công", "Đã xóa từ \"" + eng + "\" khỏi danh sách!");
        updateUserStatus();
    } else {
        QMessageBox::warning(this, "Không tìm thấy", "Không tìm thấy từ \"" + eng + "\" trong danh sách đã lưu!");
    }
}
// ==================== HIỂN THỊ LÝ THUYẾT – CHỮ ĐEN ĐẬM, ĐỌC ĐẸP ====================
void MainWindow::showTenseDetail(QListWidgetItem* item)
{
    if (!item) return;

    QString text = item->text().trimmed();
    int num = text.left(text.indexOf('.')).toInt();
    if (num < 1 || num > 12) return;

    static const QStringList tenseNames = {
        "PRESENT SIMPLE", "PRESENT CONTINUOUS", "PRESENT PERFECT", "PRESENT PERFECT CONTINUOUS",
        "PAST SIMPLE", "PAST CONTINUOUS", "PAST PERFECT", "PAST PERFECT CONTINUOUS",
        "FUTURE SIMPLE", "FUTURE CONTINUOUS", "FUTURE PERFECT", "FUTURE PERFECT CONTINUOUS"
    };
    QString target = tenseNames[num - 1];

    QFile file("grammar_data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->textGrammarDetail->setHtml("<h2 style='color:red'>Không tìm thấy grammar_data.txt!</h2>");
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    QString html = "<h1 style='color:#4facfe; text-align:center'>" + target.replace("_", " ") + "</h1><hr>";
    html += "<div style='color:black; font-size:16px; line-height:1.6;'>";  // ← CHỮ ĐEN ĐẬM, DỄ ĐỌC

    bool found = false;
    bool inTarget = false;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QString trimmed = line.trimmed();

        if (trimmed.startsWith('[') && trimmed.endsWith(']')) {
            QString current = trimmed.mid(1, trimmed.size()-2).trimmed().toUpper();
            inTarget = (current == target);
            if (inTarget) found = true;
            if (!inTarget && found) break;
            continue;
        }

        if (!inTarget) continue;

        QString content = line.mid(line.indexOf(':') + 1).trimmed();
        QString safe = content.toHtmlEscaped();

        if (trimmed.toLower().startsWith("formula_aff:") || trimmed.contains("khẳng định", Qt::CaseInsensitive))
            html += "<p><b style='color:#0066cc'>Khẳng định:</b> " + safe + "</p>";
        else if (trimmed.toLower().startsWith("formula_neg:") || trimmed.contains("phủ định", Qt::CaseInsensitive))
            html += "<p><b style='color:#cc0000'>Phủ định:</b> " + safe + "</p>";
        else if (trimmed.toLower().startsWith("formula_q:") || trimmed.contains("nghi vấn", Qt::CaseInsensitive))
            html += "<p><b style='color:#ff9900'>Nghi vấn:</b> " + safe + "</p>";
        else if (trimmed.toLower().startsWith("signals:") || trimmed.contains("dấu hiệu", Qt::CaseInsensitive))
            html += "<p><b style='color:#009900'>Dấu hiệu:</b> " + safe + "</p>";
        else if (trimmed.toLower().startsWith("example:") || trimmed.contains("ví dụ", Qt::CaseInsensitive)) {
            QString ex = content.replace("|", "<br>");
            html += "<p><b style='color:#660099'>Ví dụ:</b><br>" + ex.toHtmlEscaped() + "</p>";
        }
    }

    html += "</div>";
    file.close();

    if (!found) html += "<p style='color:red; font-weight:bold'>Chưa có dữ liệu cho thì này!</p>";

    ui->textGrammarDetail->setHtml(html);
}
void MainWindow::startGrammarPractice()
{
    QStringList choices = {"Ôn riêng từng thì", "Làm bài thi tổng hợp practice_data.txt"};
    bool ok;
    QString choice = QInputDialog::getItem(this, "Ôn tập Grammar", "Chọn kiểu ôn tập:", choices, 0, false, &ok);
    if (!ok) return;

    if (choice.contains("riêng")) {
        QFile file("grammar_data.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Lỗi", "Không tìm thấy grammar_data.txt!");
            return;
        }

        QTextStream in(&file);
        in.setEncoding(QStringConverter::Utf8);

        struct Ques {
            QString tense;
            QString question;
            QStringList options;
            int correct = 0;
        };

        QMap<QString, QVector<Ques>> questions;
        QString currentTense;
        Ques temp;
        bool inQuestion = false;

        while (!in.atEnd()) {
            QString raw = in.readLine();
            QString line = raw.trimmed();
            if (line.isEmpty()) continue;

            if (line.startsWith('[') && line.endsWith(']')) {
                QString tag = line.mid(1, line.size()-2).trimmed().toUpper();

                if (tag == "QUESTION") {
                    if (temp.correct > 0 && !currentTense.isEmpty()) {
                        temp.tense = currentTense;
                        questions[currentTense].append(temp);
                    }
                    temp = Ques();
                    temp.options.clear();
                    temp.correct = 0;
                    inQuestion = true;
                    continue;
                }
                if (tag == "END") { inQuestion = false; continue; }

                currentTense = tag;
                inQuestion = false;
                continue;
            }

            if (!inQuestion) continue;

            if (temp.question.isEmpty()) {
                temp.question = raw;
                continue;
            }

            if (line[0].isDigit() && line.contains('.')) {
                temp.options << raw.mid(raw.indexOf('.') + 1).trimmed();
                continue;
            }

            if (line.startsWith("ANSWER:", Qt::CaseInsensitive)) {
                temp.correct = line.mid(7).trimmed().toInt();
                if (temp.correct > 0 && !currentTense.isEmpty()) {
                    temp.tense = currentTense;
                    questions[currentTense].append(temp);
                }
                temp = Ques();
                inQuestion = false;
            }
        }
        file.close();

        const QStringList tenses = {
            "PRESENT SIMPLE", "PRESENT CONTINUOUS", "PRESENT PERFECT", "PRESENT PERFECT CONTINUOUS",
            "PAST SIMPLE", "PAST CONTINUOUS", "PAST PERFECT", "PAST PERFECT CONTINUOUS",
            "FUTURE SIMPLE", "FUTURE CONTINUOUS", "FUTURE PERFECT", "FUTURE PERFECT CONTINUOUS"
        };

        QStringList menu;
        for (const auto& t : tenses) {
            int n = questions[t].size();
            QString name = t;
            name.replace('_', ' ');  // replace() chuẩn Qt từ xưa tới giờ
            menu << QString("%1 (%2 câu)").arg(name).arg(n);
        }

        bool loop = true;
        while (loop) {
            QString pick = QInputDialog::getItem(this, "Chọn thì", "Bạn muốn ôn thì nào?", menu, 0, false, &ok);
            if (!ok) break;

            int idx = menu.indexOf(pick);
            QString tense = tenses[idx];
            QVector<Ques> list = questions[tense];

            if (list.isEmpty()) {
                QMessageBox::information(this, "", "Chưa có câu hỏi cho thì này!");
                continue;
            }

            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(list.begin(), list.end(), g);
            int total = qMin(10, list.size());
            list.resize(total);

            int score = 0;
            for (int i = 0; i < total; ++i) {
                const Ques& q = list[i];

                // Cách thay _ bằng space an toàn 100% trên mọi Qt 6
                QString tenseName = tense;
                tenseName.replace('_', ' ');

                QString html = QString("<h2 style='color:#4facfe'>%1</h2>"
                                       "<h3>Câu %2/%3</h3>"
                                       "<p style='font-size:17px'>%4</p><br>")
                                   .arg(tenseName)
                                   .arg(i+1).arg(total)
                                   .arg(q.question.toHtmlEscaped().replace("\n", "<br>"));

                for (int j = 0; j < q.options.size(); ++j)
                    html += QString("<b style='color:#0066cc'>%1.</b> %2<br>").arg(j+1).arg(q.options[j].toHtmlEscaped());

                QDialog dlg(this);
                dlg.setWindowTitle(QString("Câu %1/%2").arg(i+1).arg(total));
                QVBoxLayout lay(&dlg);
                QLabel* lbl = new QLabel(html);
                lbl->setWordWrap(true);
                lay.addWidget(lbl);

                QLineEdit* edit = new QLineEdit(&dlg);
                edit->setPlaceholderText("1-4");
                edit->setMaximumWidth(150);
                edit->setAlignment(Qt::AlignCenter);
                edit->setFocus();
                lay.addWidget(edit);

                QDialogButtonBox* bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
                lay.addWidget(bb);
                connect(bb, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
                connect(bb, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

                if (dlg.exec() != QDialog::Accepted) {
                    QMessageBox::information(this, "", "Đã dừng ôn tập.");
                    loop = false;
                    break;
                }

                bool valid;
                int ans = edit->text().toInt(&valid);
                if (!valid || ans < 1 || ans > 4) {
                    QMessageBox::warning(this, "Lỗi", "Nhập số từ 1-4 thôi!");
                    i--;
                    continue;
                }

                if (ans == q.correct) {
                    QMessageBox::information(this, "Đúng!", "Chính xác!");
                    score++;
                } else {
                    QMessageBox::warning(this, "Sai", QString("Đáp án đúng: %1. %2")
                                                          .arg(q.correct).arg(q.options[q.correct-1]));
                }
            }

            if (loop) {
                QString tenseName = tense;
                tenseName.replace('_', ' ');
                QMessageBox::information(this, "Hoàn thành",
                                         QString("Bạn đã làm xong thì <b>%1</b>!\nĐúng <b>%2/%3</b> câu.")
                                             .arg(tenseName).arg(score).arg(total));
            }
        }
    }
    else {
        // ======================= BÀI THI TỔNG HỢP - DỰA HOÀN TOÀN VÀO CLASS C++ CỦA BẠN =======================
        QFile file("practice_data.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Lỗi", "Không tìm thấy file practice_data.txt!\nĐặt cùng thư mục với chương trình nhé.");
            return;
        }

        QTextStream in(&file);
        in.setEncoding(QStringConverter::Utf8);

        struct MCQ { QString question; QStringList options; int answer = 0; };
        struct GapFill { QString question, answer, hint, baseVerb; };
        struct Rearrange { QString jumbled, correct; };
        struct PracticeTest { QVector<MCQ> mcqs; QVector<GapFill> gapfills; QVector<Rearrange> rearranges; };

        QVector<PracticeTest> tests;
        PracticeTest current;
        bool inMCQ = false, inGap = false, inRearr = false;
        MCQ curMCQ;
        int mcqStep = 0;

        auto qtrim = [](QString s) -> QString {
            s = s.simplified();
            if (s.endsWith('\r')) s.chop(1);
            return s;
        };

        while (!in.atEnd()) {
            QString rawLine = in.readLine();
            QString line = rawLine.trimmed();
            if (line.isEmpty()) continue;

            if (line.startsWith('[') && line.endsWith(']')) {
                QString tag = line.mid(1, line.size()-2).trimmed().toUpper();

                if (tag == "END") {
                    tests.append(current);
                    current = PracticeTest();
                    inMCQ = inGap = inRearr = false;
                    mcqStep = 0;
                    continue;
                }
                if (tag == "MCQ") { inMCQ = true; inGap = inRearr = false; mcqStep = 0; continue; }
                if (tag == "GAPFILL") { inGap = true; inMCQ = inRearr = false; continue; }
                if (tag == "REARRANGE") { inRearr = true; inMCQ = inGap = false; continue; }
                continue;
            }

            if (inMCQ) {
                if (mcqStep == 0) {
                    curMCQ = MCQ();
                    curMCQ.question = rawLine;
                    mcqStep = 1;
                }
                else if (mcqStep >= 1 && mcqStep <= 4) {
                    QString opt = rawLine;
                    int dot = opt.indexOf('.');
                    if (dot != -1) opt = opt.mid(dot + 1);
                    curMCQ.options.append(qtrim(opt));
                    mcqStep++;
                }
                else if (line.startsWith("ANSWER:")) {
                    curMCQ.answer = line.mid(7).trimmed().toInt();
                    current.mcqs.append(curMCQ);
                    mcqStep = 0;
                }
            }
            else if (inGap) {
                int eq = rawLine.indexOf('=');
                if (eq != -1) {
                    GapFill g;
                    QString left = rawLine.left(eq).trimmed();
                    QString right = rawLine.mid(eq + 1);

                    // Xử lý baseVerb nếu có (verb)
                    int verbStart = left.indexOf(" (");
                    if (verbStart != -1 && left.endsWith(')')) {
                        g.baseVerb = left.mid(verbStart + 2, left.size() - verbStart - 3);
                        g.question = left.left(verbStart) + " _____";
                    } else {
                        g.baseVerb = "";
                        g.question = left;
                    }

                    // Xử lý đáp án + hint
                    int hintStart = right.indexOf(" (");
                    if (hintStart != -1) {
                        g.answer = right.left(hintStart).trimmed();
                        g.hint = right.mid(hintStart + 2, right.size() - hintStart - 3);
                    } else {
                        int bar = right.indexOf('|');
                        if (bar != -1) {
                            g.answer = right.left(bar).trimmed();
                            g.hint = right.mid(bar + 1).trimmed();
                        } else {
                            g.answer = right.trimmed();
                            g.hint = "";
                        }
                    }
                    current.gapfills.append(g);
                }
            }
            else if (inRearr) {
                int eq = rawLine.indexOf('=');
                if (eq != -1) {
                    Rearrange r;
                    r.jumbled = rawLine.left(eq).trimmed();
                    r.correct = rawLine.mid(eq + 1).trimmed();
                    current.rearranges.append(r);
                }
            }
        }
        if (!current.mcqs.isEmpty() || !current.gapfills.isEmpty() || !current.rearranges.isEmpty())
            tests.append(current);

        file.close();

        if (tests.isEmpty()) {
            QMessageBox::information(this, "Thông báo", "Chưa có bộ đề nào trong practice_data.txt!");
            return;
        }

        // Chọn ngẫu nhiên 1 bộ
        int testIdx = QRandomGenerator::global()->bounded(tests.size());
        const PracticeTest& test = tests[testIdx];

        int score = 0, total = test.mcqs.size() + test.gapfills.size() + test.rearranges.size();

        // Hàm so sánh đáp án không phân biệt hoa thường, khoảng trắng thừa
        auto compare = [](const QString& a, const QString& b) -> bool {
            QString x = a.simplified().toLower();
            QString y = b.simplified().toLower();
            return x == y;
        };

        // 1. MCQ
        for (int i = 0; i < test.mcqs.size(); ++i) {
            const auto& q = test.mcqs[i];
            QString html = QString("<h3>Câu %1 (Trắc nghiệm)</h3><p style='font-size:17px'>%2</p><br>")
                               .arg(i+1).arg(q.question.toHtmlEscaped());

            for (int j = 0; j < q.options.size(); ++j)
                html += QString("<b style='color:#0066cc'>%1.</b> %2<br>").arg(j+1).arg(q.options[j].toHtmlEscaped());

            QDialog dlg(this);
            dlg.setWindowTitle(QString("Bộ %1 - Câu %2/%3").arg(testIdx+1).arg(i+1).arg(total));
            QVBoxLayout lay(&dlg);
            QLabel* lbl = new QLabel(html); lbl->setWordWrap(true); lay.addWidget(lbl);
            QLineEdit* edit = new QLineEdit; edit->setPlaceholderText("Nhập 1-4"); edit->setMaximumWidth(150); edit->setAlignment(Qt::AlignCenter);
            lay.addWidget(edit);
            QDialogButtonBox* bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            lay.addWidget(bb);
            connect(bb, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
            connect(bb, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

            if (dlg.exec() != QDialog::Accepted) { QMessageBox::information(this, "", "Đã dừng bài thi."); return; }

            bool okNum; int ans = edit->text().toInt(&okNum);
            if (!200 || ans < 1 || ans > 4) {
                QMessageBox::warning(this, "Lỗi", "Nhập số từ 1 đến 4!");
                i--; continue;
            }
            if (ans == q.answer) { QMessageBox::information(this, "Đúng!", "Tuyệt vời!"); score++; }
            else QMessageBox::warning(this, "Sai", "Đáp án đúng: " + q.options[q.answer-1]);
        }

        // 2. Gap Fill
        int globalIdx = test.mcqs.size();
        for (const auto& g : test.gapfills) {
            globalIdx++;
            QString hint = g.baseVerb.isEmpty() ? "" : QString(" (gợi ý: %1 → đổi thì phù hợp)").arg(g.baseVerb);
            QString html = QString("<h3>Câu %1 (Điền động từ)</h3><p style='font-size:17px'>%2%3</p>")
                               .arg(globalIdx).arg(g.question.toHtmlEscaped()).arg(hint);

            QDialog dlg(this);
            dlg.setWindowTitle(QString("Bộ %1 - Câu %2/%3").arg(testIdx+1).arg(globalIdx).arg(total));
            QVBoxLayout lay(&dlg);
            QLabel* lbl = new QLabel(html); lbl->setWordWrap(true); lay.addWidget(lbl);
            QLineEdit* edit = new QLineEdit; edit->setPlaceholderText("Nhập đáp án..."); edit->setFocus();
            lay.addWidget(edit);
            QDialogButtonBox* bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            lay.addWidget(bb);
            connect(bb, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
            connect(bb, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

            if (dlg.exec() != QDialog::Accepted) { QMessageBox::information(this, "", "Đã dừng bài thi."); return; }

            if (compare(edit->text(), g.answer)) {
                QMessageBox::information(this, "Đúng!", "Chính xác!");
                score++;
            } else {
                QString msg = "Sai rồi!\nĐáp án đúng: <b>" + g.answer + "</b>";
                if (!g.hint.isEmpty()) msg += "\nGợi ý: " + g.hint;
                QMessageBox::warning(this, "Sai", msg);
            }
        }

        // 3. Rearrange
        for (const auto& r : test.rearranges) {
            globalIdx++;
            QString html = QString("<h3>Câu %1 (Sắp xếp lại câu)</h3><p style='font-size:17px'>%2</p>")
                               .arg(globalIdx).arg(r.jumbled.toHtmlEscaped());

            QDialog dlg(this);
            dlg.setWindowTitle(QString("Bộ %1 - Câu %2/%3").arg(testIdx+1).arg(globalIdx).arg(total));
            QVBoxLayout lay(&dlg);
            QLabel* lbl = new QLabel(html); lbl->setWordWrap(true); lay.addWidget(lbl);
            QLineEdit* edit = new QLineEdit; edit->setPlaceholderText("Nhập câu hoàn chỉnh..."); edit->setFocus();
            lay.addWidget(edit);
            QDialogButtonBox* bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            lay.addWidget(bb);
            connect(bb, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
            connect(bb, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

            if (dlg.exec() != QDialog::Accepted) { QMessageBox::information(this, "", "Đã dừng bài thi."); return; }

            if (compare(edit->text(), r.correct)) {
                QMessageBox::information(this, "Đúng!", "Hoàn hảo!");
                score++;
            } else {
                QMessageBox::warning(this, "Sai", "Câu đúng: <b>" + r.correct + "</b>");
            }
        }

        // Kết quả
        QMessageBox::information(this, "HOÀN THÀNH!",
                                 QString("<h2>Bộ đề %1</h2>"
                                         "<p>Bạn đúng <b>%2/%3</b> câu!</p>"
                                         "<p>Giỏi lắm!</p>").arg(testIdx+1).arg(score).arg(total));
    }
}
