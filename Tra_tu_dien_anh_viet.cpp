#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#include <limits>
#include <filesystem>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <random>
#include <thread>
#include <chrono>
#include <memory>
#include <set>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif
using namespace std;
namespace fs = std::filesystem;

// ==============================
// UTF-8 & Safe Input
// ==============================
void enableUTF8() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    system("chcp 65001 > nul");
#endif
}

string safeInput(const string& prompt = "") {
    string line;
    cout << prompt;
    cout.flush();
    if (!getline(cin, line)) { cin.clear(); return ""; }
    if (!line.empty() && line.back() == '\r') line.pop_back();
    return line;
}

// ==============================
// Utility
// ==============================
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

bool isNumber(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

string normalize(const string& s) {
    static const map<char, char> m = {
        // vietnamese lowercase
        {char(0xE0),'a'},{char(0xE1),'a'},{char(0x1EA3),'a'},{char(0xE3),'a'},{char(0x1EA1),'a'},
        {char(0x103),'a'},{char(0x1EAF),'a'},{char(0x1EB1),'a'},{char(0x1EB3),'a'},{char(0x1EB5),'a'},{char(0x1EB7),'a'},
        {char(0xE2),'a'},{char(0x1EA7),'a'},{char(0x1EA9),'a'},{char(0x1EAB),'a'},{char(0x1EAD),'a'},
        {char(0xE8),'e'},{char(0xE9),'e'},{char(0x1EBB),'e'},{char(0x1EBD),'e'},{char(0x1EB9),'e'},
        {char(0xEA),'e'},{char(0x1EC1),'e'},{char(0x1EBF),'e'},{char(0x1EC3),'e'},{char(0x1EC5),'e'},{char(0x1EC7),'e'},
        {char(0xEC),'i'},{char(0xED),'i'},{char(0x1EC9),'i'},{char(0x129),'i'},{char(0x1ECB),'i'},
        {char(0xF2),'o'},{char(0xF3),'o'},{char(0x1ECF),'o'},{char(0xF5),'o'},{char(0x1ECD),'o'},
        {char(0xF4),'o'},{char(0x1ED3),'o'},{char(0x1ED1),'o'},{char(0x1ED5),'o'},{char(0x1ED7),'o'},{char(0x1ED9),'o'},
        {char(0x1A1),'o'},{char(0x1EDD),'o'},{char(0x1EDB),'o'},{char(0x1EDF),'o'},{char(0x1EE1),'o'},{char(0x1EE3),'o'},
        {char(0xF9),'u'},{char(0xFA),'u'},{char(0x1EE7),'u'},{char(0x169),'u'},{char(0x1EE5),'u'},
        {char(0x1B0),'u'},{char(0x1EEB),'u'},{char(0x1EED),'u'},{char(0x1EEF),'u'},{char(0x1EF1),'u'},
        {char(0x1EF3),'y'},{char(0xFD),'y'},{char(0x1EF7),'y'},{char(0x1EF9),'y'},{char(0x1EF5),'y'},
        {char(0x111),'d'},
        // uppercase
        {char(0xC0),'A'},{char(0xC1),'A'},{char(0x1EA2),'A'},{char(0xC3),'A'},{char(0x1EA0),'A'},
        {char(0x102),'A'},{char(0x1EAE),'A'},{char(0x1EB0),'A'},{char(0x1EB2),'A'},{char(0x1EB4),'A'},{char(0x1EB6),'A'},
        {char(0xC2),'A'},{char(0x1EA6),'A'},{char(0x1EA8),'A'},{char(0x1EAA),'A'},{char(0x1EAC),'A'},
        {char(0xC8),'E'},{char(0xC9),'E'},{char(0x1EBA),'E'},{char(0x1EBC),'E'},{char(0x1EB8),'E'},
        {char(0xCA),'E'},{char(0x1EC0),'E'},{char(0x1EBE),'E'},{char(0x1EC2),'E'},{char(0x1EC4),'E'},{char(0x1EC6),'E'},
        {char(0xCC),'I'},{char(0xCD),'I'},{char(0x1EC8),'I'},{char(0x128),'I'},{char(0x1ECA),'I'},
        {char(0xD2),'O'},{char(0xD3),'O'},{char(0x1ECE),'O'},{char(0xD5),'O'},{char(0x1ECC),'O'},
        {char(0xD4),'O'},{char(0x1ED2),'O'},{char(0x1ED0),'O'},{char(0x1ED4),'O'},{char(0x1ED6),'O'},{char(0x1ED8),'O'},
        {char(0x1A0),'O'},{char(0x1EDC),'O'},{char(0x1EDA),'O'},{char(0x1EDE),'O'},{char(0x1EE0),'O'},{char(0x1EE2),'O'},
        {char(0xD9),'U'},{char(0xDA),'U'},{char(0x1EE6),'U'},{char(0x168),'U'},{char(0x1EE4),'U'},
        {char(0x1AF),'U'},{char(0x1EEA),'U'},{char(0x1EEC),'U'},{char(0x1EEE),'U'},{char(0x1EF0),'U'},
        {char(0x1EF2),'Y'},{char(0xDD),'Y'},{char(0x1EF6),'Y'},{char(0x1EF8),'Y'},{char(0x1EF4),'Y'},
        {char(0x110),'D'}
    };
    string res;
    for (char c : s) {
        unsigned char uc = static_cast<unsigned char>(c);
        auto it = m.find(uc);
        res += (it != m.end()) ? it->second : tolower(uc);
    }
    return res;
}

// ==============================
// Phát âm (Windows)
// ==============================
#ifdef _WIN32
void speak(const string& text) {
    string escaped;
    for (char c : text) {
        if (c == '"' || c == '\\') escaped += '\\';
        escaped += c;
    }
    string cmd = "powershell -Command \"Add-Type -AssemblyName System.Speech; "
                 "$synth = New-Object System.Speech.Synthesis.SpeechSynthesizer; "
                 "$synth.SelectVoice('Microsoft Zira Desktop'); "
                 "$synth.Speak('" + escaped + "');\"";
    system(cmd.c_str());
}

void speakRepeat(const string& text) {
    cout << "\nĐang phát lại liên tục... (Nhấn Enter để dừng)\n";
    thread([text]() {
        while (true) {
            this_thread::sleep_for(chrono::milliseconds(100));
            if (_kbhit() && _getch() == '\r') break;
            speak(text);
        }
    }).detach();
    safeInput("");
}
#endif

// ==============================
// Lịch sử tra từ
// ==============================
void saveHistory(const string& word) {
    ofstream f("history.txt", ios::app);
    if (f.is_open()) { f << word << "\n"; f.close(); }
}

vector<string> loadHistory(int limit = 5) {
    vector<string> all, res;
    ifstream f("history.txt");
    if (!f.is_open()) return res;
    string line;
    while (getline(f, line)) {
        line = trim(line);
        if (!line.empty()) all.push_back(line);
    }
    f.close();
    int start = max(0, (int)all.size() - limit);
    for (int i = start; i < (int)all.size(); ++i) res.push_back(all[i]);
    return res;
}

// ==============================
// Word (CẬP NHẬT: lastReview + isMastered)
// ==============================
struct Word {
    string english, vietnamese, ipaText, example, topic;
    bool learned = false;
    int exp = 0;
    bool isNew = true;
    time_t lastReview = 0;     // Thời gian ôn lần cuối
    bool isMastered = false;   // Đã thành thạo

    Word() = default;
    Word(const string& e, const string& v, const string& ipaTxt, const string& ex, const string& t = "")
        : english(e), vietnamese(v), ipaText(ipaTxt), example(ex), topic(t) {}
};

// ==============================
// BST Dictionary
// ==============================
struct Node { Word data; Node *left = nullptr, *right = nullptr; Node(const Word& w) : data(w) {} };
class Dictionary {
protected:
    Node *rootEng = nullptr, *rootVie = nullptr;
    Node* insert(Node* node, const Word& w, bool eng) {
        if (!node) return new Node(w);
        string k1 = normalize(eng ? w.english : w.vietnamese);
        string k2 = normalize(eng ? node->data.english : node->data.vietnamese);
        if (k1 < k2) node->left = insert(node->left, w, eng);
        else if (k1 > k2) node->right = insert(node->right, w, eng);
        return node;
    }
    Word* find(Node* node, const string& key, bool eng) {
        if (!node) return nullptr;
        string k = normalize(eng ? node->data.english : node->data.vietnamese);
        if (key == k) return &node->data;
        return (key < k) ? find(node->left, key, eng) : find(node->right, key, eng);
    }
    void suggest(Node* node, const string& key, bool eng, vector<string>& res, int limit) {
        if (!node || (int)res.size() >= limit) return;
        string w = normalize(eng ? node->data.english : node->data.vietnamese);
        if (w.find(key) != string::npos) res.push_back(eng ? node->data.english : node->data.vietnamese);
        suggest(node->left, key, eng, res, limit);
        suggest(node->right, key, eng, res, limit);
    }
    void clear(Node* n) { if (n) { clear(n->left); clear(n->right); delete n; } }
public:
    ~Dictionary() { clear(rootEng); clear(rootVie); }
    void add(const Word& w) { rootEng = insert(rootEng, w, true); rootVie = insert(rootVie, w, false); }
    Word* findEng(const string& k) { return find(rootEng, normalize(k), true); }
    Word* findVie(const string& k) { return find(rootVie, normalize(k), false); }
    vector<string> suggestEng(const string& k) { vector<string> r; suggest(rootEng, normalize(k), true, r, 8); return r; }
    vector<string> suggestVie(const string& k) { vector<string> r; suggest(rootVie, normalize(k), false, r, 8); return r; }
};

// ==============================
// SavedWords (CẬP NHẬT: lastReview + isMastered)
// ==============================
class SavedWords : public Dictionary {
private:
    string username;
    vector<Word> words;
    string saveFile;
    void saveToFile() const {
        ofstream f(saveFile);
        if (!f) return;
        for (const auto& w : words)
            f << w.english << "|" << w.vietnamese << "|" << w.ipaText << "|" << w.example << "|"
              << w.exp << "|" << (w.isNew ? "1" : "0") << "|" << w.topic << "|" 
              << (w.learned ? "1" : "0") << "|" << w.lastReview << "|" << (w.isMastered ? "1" : "0") << "\n";
    }
    void loadFromFile() {
        words.clear();
        clear(rootEng); clear(rootVie); rootEng = rootVie = nullptr;
        ifstream f(saveFile);
        if (!f) return;
        string line;
        while (getline(f, line)) {
            stringstream ss(line);
            vector<string> parts;
            string part;
            while (getline(ss, part, '|')) parts.push_back(trim(part));
            if (parts.size() < 10) continue;
            Word w(parts[0], parts[1], parts[2], parts[3], parts[6]);
            try { w.exp = stoi(parts[4]); } catch(...) { w.exp = 0; }
            w.isNew = (parts[5] == "1");
            w.learned = (parts[7] == "1");
            w.lastReview = parts[8].empty() ? 0 : stoll(parts[8]);
            w.isMastered = (parts[9] == "1");
            words.push_back(w);
            Dictionary::add(w);
        }
    }
public:
    SavedWords(const string& user) : username(user), saveFile(user + "_words.txt") {
        loadFromFile();
    }
    void addOrUpdate(const Word& word) {
        auto it = find_if(words.begin(), words.end(),
                          [&](const Word& w) { return w.english == word.english; });
        if (it != words.end()) {
            it->exp += 10;
            it->isNew = false;
            it->learned = true;
            it->lastReview = time(nullptr);
            cout << "Cập nhật: " << word.english << " (+10 EXP) [" << word.topic << "]\n";
        } else {
            Word newWord = word;
            newWord.exp = 10;
            newWord.isNew = true;
            newWord.learned = true;
            newWord.lastReview = time(nullptr);
            words.push_back(newWord);
            Dictionary::add(newWord);
            cout << "Đã lưu: " << word.english << " (MỚI +10 EXP) [" << word.topic << "]\n";
        }
        saveToFile();
    }
    void autoLearn(const string& eng) {
        auto it = find_if(words.begin(), words.end(),
                          [&](const Word& w) { return w.english == eng; });
        if (it != words.end()) {
            if (!it->learned) {
                it->learned = true;
                it->exp += 10;
                cout << "Đã học: " << eng << " (+10 EXP)\n";
            } else {
                it->exp += 5;
                cout << "Ôn lại: " << eng << " (+5 EXP)\n";
            }
            it->isNew = false;
            it->lastReview = time(nullptr);
        } else {
            Word temp(eng, "", "", "", "");
            temp.learned = true;
            temp.exp = 10;
            temp.isNew = false;
            temp.lastReview = time(nullptr);
            words.push_back(temp);
            Dictionary::add(temp);
            cout << "Tự động ghi nhận: " << eng << " là đã học (+10 EXP)\n";
        }
        saveToFile();
    }
    bool remove(const string& eng) {
        auto it = remove_if(words.begin(), words.end(),
                            [&](const Word& w) { return w.english == eng; });
        if (it == words.end()) return false;
        words.erase(it, words.end());
        loadFromFile();  // rebuild BST
        saveToFile();
        return true;
    }
    void display() const {
        cout << "\n===== TỪ CỦA " << username << " =====\n";
        if (words.empty()) { cout << "(Trống)\n"; return; }
        for (const auto& w : words) {
            cout << w.english << " - " << w.vietnamese
                 << " | IPA: " << w.ipaText
                 << " | EXP: " << w.exp
                 << " | " << (w.isNew ? "MỚI" : "CŨ")
                 << " | Chủ đề: " << w.topic
                 << (w.isMastered ? " [THÀNH THẠO]" : "") << "\n";
        }
    }
    bool contains(const string& eng) const {
        return any_of(words.begin(), words.end(),
                      [&](const Word& w) { return w.english == eng; });
    }
    const vector<Word>& getWords() const { return words; }
    vector<Word>& getWords() { return words; }
};

// ==============================
// Account
// ==============================
class Account {
    string username, password;
    bool isLoggedIn = false;
    unique_ptr<SavedWords> savedWords;
    const string accountFile = "accounts.txt";
public:
    bool signUp() {
        cout << "\n=== ĐĂNG KÝ ===\nTên: "; getline(cin, username);
        cout << "Mật khẩu: "; getline(cin, password);
        if (username.empty() || password.empty()) return cout << "Không để trống!\n", false;
        ifstream in(accountFile);
        string u, p; while (in >> u >> p) if (u == username) return cout << "Tên đã tồn tại!\n", false;
        in.close();
        ofstream out(accountFile, ios::app);
        if (out) { out << username << " " << password << "\n"; cout << "Đăng ký thành công!\n"; return true; }
        return cout << "Lỗi file!\n", false;
    }
    bool login() {
        cout << "\n=== ĐĂNG NHẬP ===\nTên: "; getline(cin, username);
        cout << "Mật khẩu: "; getline(cin, password);
        ifstream in(accountFile);
        if (!in) return cout << "Chưa có tài khoản!\n", false;
        string u, p;
        while (in >> u >> p)
            if (u == username && p == password) {
                isLoggedIn = true;
                savedWords = make_unique<SavedWords>(username);
                cout << "Chào, " << username << "!\n";
                return true;
            }
        return cout << "Sai thông tin!\n", false;
    }
    void logout() {
        if (isLoggedIn) {
            savedWords.reset();
            isLoggedIn = false;
            cout << "Đăng xuất!\n";
        } else cout << "Chưa đăng nhập.\n";
    }
    void saveWord(Word& w) {
        if (!isLoggedIn || !savedWords) return cout << "Đăng nhập trước!\n", void();
        savedWords->addOrUpdate(w);
    }
    void deleteSavedWord(const string& eng) {
        if (!isLoggedIn || !savedWords) { cout << "Cần đăng nhập!\n"; return; }
        if (!savedWords->remove(eng)) cout << "Không tìm thấy: " << eng << "\n";
        else cout << "Đã xóa: " << eng << "\n";
    }
    void showSavedWords() const {
        if (!isLoggedIn || !savedWords) return cout << "Cần đăng nhập!\n", void();
        savedWords->display();
    }
    bool isLogin() const { return isLoggedIn; }
    string getUsername() const { return username; }
    SavedWords* getSavedWords() const { return savedWords.get(); }
    bool isWordSaved(const string& eng) const {
        return isLoggedIn && savedWords && savedWords->contains(eng);
    }
};

// ==============================
// Topic
// ==============================
class Topic : public Dictionary {
private:
    string name;
    vector<Word> words;
public:
    Topic(const string& topicName) : name(topicName) {}
    const string& getName() const { return name; }
    void addWord(const Word& w) { words.push_back(w); Dictionary::add(w); }
    const vector<Word>& getWords() const { return words; }
    size_t size() const { return words.size(); }
};

// ==============================
// GrammarTrainer (giữ nguyên)
// ==============================
class GrammarTrainer { /* ... giữ nguyên như cũ ... */ };

// ==============================
// VocabularyApp (CẬP NHẬT viewTopicByIndex)
// ==============================
class VocabularyApp {
private:
    vector<Word> allWords;
    map<string, Topic> topicMap;
    vector<string> topicList;
    Dictionary globalDict;
    Account account;
    GrammarTrainer grammar;

    void refreshLearnedStatus() {
        set<string> learnedEng;
        if (account.isLogin() && account.getSavedWords()) {
            for (const auto& w : account.getSavedWords()->getWords()) {
                if (w.learned) learnedEng.insert(normalize(w.english));
            }
        }
        for (auto& w : allWords) {
            w.learned = learnedEng.count(normalize(w.english));
        }
    }

    bool needsReview(const Word& w) {
        if (w.isMastered || w.isNew || !w.learned) return false;
        time_t now = time(nullptr);
        return (now - w.lastReview) >= (7 * 24 * 3600); // 7 ngày
    }

    void moveToMastered(const Word& w) {
        ofstream f("mastered_words.txt", ios::app);
        if (f) f << w.english << "|" << w.vietnamese << "|" << w.ipaText << "|" << w.example << "|" << w.topic << "\n";
    }

    int getChoice(int max, const string& prompt) {
        while (true) {
            string input = safeInput(prompt);
            input = trim(input);
            if (input == "0") return 0;
            if (isNumber(input)) {
                int n = stoi(input);
                if (n >= 1 && n <= max) return n;
            }
            cout << "Nhập số từ 1 đến " << max << " (0 để thoát).\n";
        }
    }

    void viewTopicByIndex(int idx) {
        string selected = topicList[idx - 1];
        const auto& topic = topicMap.at(selected);
        const auto& wordList = topic.getWords();

        // === ÔN TẬP TỪ CŨ ===
        vector<const Word*> reviewWords;
        if (account.isLogin() && account.getSavedWords()) {
            for (const auto& w : wordList) {
                auto saved = find_if(account.getSavedWords()->getWords().begin(),
                                     account.getSavedWords()->getWords().end(),
                                     [&](const Word& sw) { return sw.english == w.english; });
                if (saved != account.getSavedWords()->getWords().end() && needsReview(*saved)) {
                    reviewWords.push_back(&(*saved));
                }
            }
        }

        if (!reviewWords.empty()) {
            cout << "\nÔN TẬP LẠI (" << reviewWords.size() << " từ cũ)\n";
            cout << "Nhập nghĩa tiếng Việt của từ sau:\n";

            random_device rd; mt19937 g(rd());
            shuffle(reviewWords.begin(), reviewWords.end(), g);

            for (const Word* pw : reviewWords) {
                Word& w = const_cast<Word&>(*pw);
                cout << "\nTừ: " << w.english << " (IPA: " << w.ipaText << ")\n";
                cout << "→ Nghĩa: ";
                string answer = safeInput();

                bool correct = normalize(answer) == normalize(w.vietnamese);
                if (correct) {
                    w.exp += 1;
                    cout << "Đúng! (+1 EXP) → " << w.exp << "/100\n";
                } else {
                    w.exp = max(0, w.exp - 3);
                    cout << "Sai! (-3 EXP) → " << w.exp << "/100\n";
                    cout << "Đáp án: " << w.vietnamese << "\n";
                }
                w.lastReview = time(nullptr);

                if (w.exp >= 100 && !w.isMastered) {
                    w.isMastered = true;
                    moveToMastered(w);
                    account.getSavedWords()->remove(w.english);
                    cout << "CHÚC MỪNG! \"" << w.english << "\" đã THÀNH THẠO!\n";
                }
            }
            account.getSavedWords()->saveToFile();
            safeInput("\nNhấn Enter để tiếp tục...");
        }

        // === XEM DANH SÁCH TỪ ===
        cout << "\nTỪ TRONG: " << selected << " (" << wordList.size() << " từ)\n";
        for (size_t i = 0; i < wordList.size(); ++i)
            cout << i + 1 << ". " << wordList[i].english << (wordList[i].learned ? " [ĐÃ HỌC]" : "") << "\n";

        while (true) {
            vector<string> engList;
            for (const auto& w : wordList) engList.push_back(w.english);
            int wordIdx = getChoice(engList.size(), "\nChọn từ (0 để thoát): ");
            if (wordIdx <= 0) break;
            const Word& w = wordList[wordIdx - 1];
            saveHistory(w.english);
            cout << "\nCHI TIẾT TỪ\n";
            cout << "Từ: " << w.english << "\n";
            cout << "Nghĩa: " << w.vietnamese << "\n";
            cout << "Phiên âm: " << w.ipaText << "\n";
            cout << "Ví dụ: " << w.example << "\n";
            cout << "Chủ đề: " << w.topic << "\n";
#ifdef _WIN32
            string c = safeInput("\nPhát âm? (1: Có / 0: Không): ");
            if (trim(c) == "1") {
                speak(w.english);
                string r = safeInput("Lặp lại? (y/n): ");
                if (!trim(r).empty() && tolower(trim(r)[0]) == 'y') speakRepeat(w.english);
            }
#endif
            if (account.isLogin()) {
                account.getSavedWords()->autoLearn(w.english);
                refreshLearnedStatus();
            }
            if (account.isLogin() && !account.isWordSaved(w.english)) {
                string yn = safeInput("\nLưu từ này vào danh sách cá nhân? (y/n): ");
                if (!trim(yn).empty() && tolower(trim(yn)[0]) == 'y') {
                    account.saveWord(const_cast<Word&>(w));
                    cout << "Đã lưu chính thức!\n";
                }
            }
            cout << "────────────────────\n";
        }
        safeInput("\nNhấn Enter để quay lại danh sách chủ đề...");
    }

public:
    void buildData() { /* ... giữ nguyên ... */ }
    void viewTopic() {
        if (topicList.empty()) {
            cout << "Chưa có dữ liệu. Vui lòng thêm file CSV vào thư mục Data.\n";
            safeInput("Nhấn Enter để tiếp tục...");
            return;
        }
        while (true) {
            cout << "\nCHỦ ĐỀ\n";
            for (size_t i = 0; i < topicList.size(); ++i) {
                const auto& topic = topicMap.at(topicList[i]);
                int learned = 0;
                for (const auto& w : topic.getWords()) if (w.learned) learned++;
                double percent = topic.size() > 0 ? (learned * 100.0 / topic.size()) : 0.0;
                cout << i + 1 << ". " << topicList[i]
                     << " (" << learned << "/" << topic.size() << " - "
                     << fixed << setprecision(0) << percent << "%)\n";
            }
            int choice = getChoice(topicList.size(), "\nChọn (0 để thoát): ");
            if (choice == 0) return;
            viewTopicByIndex(choice);
        }
    }
    // ... các hàm khác giữ nguyên ...
    void showProgress() {
        refreshLearnedStatus();
        int total = allWords.size();
        int learned = 0, mastered = 0;
        for (const auto& w : allWords) if (w.learned) learned++;
        if (account.isLogin()) {
            for (const auto& w : account.getSavedWords()->getWords())
                if (w.isMastered) mastered++;
        }
        ifstream mf("mastered_words.txt");
        string line; while (getline(mf, line)) if (!trim(line).empty()) mastered++;

        cout << "\nTIẾN ĐỘ HỌC TẬP\n";
        cout << "Đã học: " << learned << "/" << total << " từ\n";
        cout << "Thành thạo: " << mastered << " từ\n";
        if (account.isLogin()) {
            int totalEXP = 0;
            for (const auto& w : account.getSavedWords()->getWords()) totalEXP += w.exp;
            cout << "Tổng EXP: " << totalEXP << " điểm\n";
        }
    }
    // ... menu(), main() giữ nguyên ...
};

// ==============================
// main
// ==============================
int main() {
    srand((unsigned)time(nullptr));
    VocabularyApp app;
    app.menu();
    return 0;
}