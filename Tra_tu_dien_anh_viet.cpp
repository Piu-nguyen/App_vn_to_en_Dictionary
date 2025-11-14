#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#include <limits>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <random>
#include <memory>
#include <set>
#include <conio.h>
#include <windows.h>
using namespace std;

// === THAY THẾ <filesystem> & <thread> - CHẠY TRÊN MỌI MinGW ===
vector<string> getCsvFiles() {
    vector<string> files;
#ifdef _WIN32
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("Data\\*.csv", &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        CreateDirectory("Data", NULL);
        return files;
    }
    do {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            files.push_back("Data\\" + string(findFileData.cFileName));
        }
    } while (FindNextFile(hFind, &findFileData) != 0);
    FindClose(hFind);
#endif
    return files;
}

// ==============================
// UTF-8 & Safe Input
// ==============================
void enableUTF8() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    system("chcp 65001 > nul");
}

string safeInput(const string& prompt = "") {
    cout << prompt;
    cout.flush();
    string line;
    if (!getline(cin, line)) {
        cin.clear();
        clearerr(stdin);
        return "";
    }
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
        // lowercase
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
    for (size_t i = 0; i < s.size(); ) {
        unsigned char uc = static_cast<unsigned char>(s[i]);
        if (uc < 128) {
            res += tolower(uc);
            ++i;
            continue;
        }
        auto it = m.find(static_cast<char>(uc));
        res += (it != m.end()) ? it->second : tolower(uc);
        ++i;
        while (i < s.size() && (static_cast<unsigned char>(s[i]) & 0xC0) == 0x80) ++i;
    }
    return res;
}

// ==============================
// Phát âm (Windows)
// ==============================
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
    cout << "\nĐang phát lặp liên tục... (Nhấn phím bất kỳ để dừng)\n";
    while (!_kbhit()) {
        speak(text);
        Sleep(600);
    }
    _getch();
    cout << "Đã dừng phát âm.\n";
}

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
// Word
// ==============================
struct Word {
    string english, vietnamese, ipaText, example, topic;
    bool learned = false;
    int exp = 0;
    bool isNew = true;
    time_t lastReview = 0;
    bool isMastered = false;

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
// SavedWords
// ==============================
class SavedWords : public Dictionary {
private:
    string username;
    vector<Word> words;
    string saveFile;
    void loadFromFile();

public:
    SavedWords(const string& user);
    void saveToFile() const;
    void addOrUpdate(const Word& word);
    void autoLearn(const string& eng);
    bool remove(const string& eng);
    void display() const;
    bool contains(const string& eng) const;
    const vector<Word>& getWords() const { return words; }
    vector<Word>& getWords() { return words; }
};

SavedWords::SavedWords(const string& user) : username(user), saveFile(user + "_words.txt") {
    loadFromFile();
}

void SavedWords::loadFromFile() {
    words.clear();
    clear(rootEng); clear(rootVie); rootEng = rootVie = nullptr;
    ifstream f(saveFile.c_str());
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

void SavedWords::saveToFile() const {
    ofstream f(saveFile.c_str());
    if (!f) return;
    for (size_t i = 0; i < words.size(); ++i) {
        const Word& w = words[i];
        f << w.english << "|" << w.vietnamese << "|" << w.ipaText << "|" << w.example << "|"
          << w.exp << "|" << (w.isNew ? "1" : "0") << "|" << w.topic << "|"
          << (w.learned ? "1" : "0") << "|" << w.lastReview << "|" << (w.isMastered ? "1" : "0") << "\n";
    }
}

void SavedWords::addOrUpdate(const Word& word) {
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

void SavedWords::autoLearn(const string& eng) {
    auto it = find_if(words.begin(), words.end(),
                      [&](const Word& w) { return w.english == eng; });
    if (it != words.end()) {
        if (!it->learned) {
            it->learned = true;
            it->exp += 10;
            cout << "Đã học: " << eng << " (+10 EXP)\n";
        } else {
            it->exp += 2;  // Ôn lại nhẹ: +2 EXP
            cout << "Ôn lại: " << eng << " (+2 EXP)\n";
        }
        it->isNew = false;
        it->lastReview = time(nullptr);
    }
    
}

bool SavedWords::remove(const string& eng) {
    auto it = remove_if(words.begin(), words.end(),
                        [&](const Word& w) { return w.english == eng; });
    if (it == words.end()) return false;
    words.erase(it, words.end());
    saveToFile();
    clear(rootEng); clear(rootVie); rootEng = rootVie = nullptr;
    for (size_t i = 0; i < words.size(); ++i) Dictionary::add(words[i]);
    return true;
}

void SavedWords::display() const {
    cout << "\n===== TỪ CỦA " << username << " =====\n";
    if (words.empty()) { cout << "(Trống)\n"; return; }
    for (size_t i = 0; i < words.size(); ++i) {
        const Word& w = words[i];
        cout << w.english << " - " << w.vietnamese
             << " | IPA: " << w.ipaText
             << " | EXP: " << w.exp
             << " | " << (w.isNew ? "MỚI" : "CŨ")
             << " | Chủ đề: " << w.topic
             << (w.isMastered ? " [THÀNH THẠO]" : "") << "\n";
    }
}

bool SavedWords::contains(const string& eng) const {
    for (size_t i = 0; i < words.size(); ++i)
        if (words[i].english == eng) return true;
    return false;
}
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
        ifstream in(accountFile.c_str());
        string u, p; while (in >> u >> p) if (u == username) return cout << "Tên đã tồn tại!\n", false;
        in.close();
        ofstream out(accountFile.c_str(), ios::app);
        if (out) { out << username << " " << password << "\n"; cout << "Đăng ký thành công!\n"; return true; }
        return cout << "Lỗi file!\n", false;
    }

    bool login() {
        cout << "\n=== ĐĂNG NHẬP ===\nTên: "; getline(cin, username);
        cout << "Mật khẩu: "; getline(cin, password);
        ifstream in(accountFile.c_str());
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
// GrammarTrainer
// ==============================
class GrammarTrainer {
private:
    struct Theory { string formulaAff, formulaNeg, formulaQ, signals, example; };
    struct Question { string question; vector<string> options; int correct; };
    map<string, Theory> theories;
    map<string, vector<Question>> grammarQuestions;

    string getTenseName(int c) {
        switch (c) {
            case 1: return "PRESENT SIMPLE"; case 2: return "PRESENT CONTINUOUS"; case 3: return "PRESENT PERFECT";
            case 4: return "PRESENT PERFECT CONTINUOUS"; case 5: return "PAST SIMPLE"; case 6: return "PAST CONTINUOUS";
            case 7: return "PAST PERFECT"; case 8: return "PAST PERFECT CONTINUOUS"; case 9: return "FUTURE SIMPLE";
            case 10: return "FUTURE CONTINUOUS"; case 11: return "FUTURE PERFECT"; case 12: return "FUTURE PERFECT CONTINUOUS";
            default: return "";
        }
    }

    void showTheory(const string& tense, const Theory& t) {
        cout << "\n===== " << tense << " =====\n";
        cout << "Khẳng định: " << t.formulaAff << "\n";
        cout << "Phủ định: " << t.formulaNeg << "\n";
        cout << "Nghi vấn: " << t.formulaQ << "\n";
        cout << "Dấu hiệu: " << t.signals << "\n";
        cout << "Ví dụ: " << t.example << "\n";
    }

public:
    bool loadFromFile(const string& filename) {
        ifstream file(filename.c_str());
        if (!file.is_open()) return false;

        string line, currentTense;
        Theory curTheory;
        vector<Question> curQs;
        Question curQ;
        bool inQuestion = false;
        int optionIndex = 0;

        while (getline(file, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.empty()) continue;

            if (line.front() == '[' && line.back() == ']') {
                string tag = line.substr(1, line.size() - 2);
                if (tag == "END") {
                    if (!currentTense.empty()) {
                        theories[currentTense] = curTheory;
                        grammarQuestions[currentTense] = curQs;
                        curQs.clear(); currentTense.clear();
                    }
                    inQuestion = false; continue;
                }
                if (tag == "QUESTION") { inQuestion = true; optionIndex = 0; curQ = Question(); continue; }
                currentTense = tag; curTheory = Theory(); inQuestion = false; continue;
            }

            if (!inQuestion && !currentTense.empty()) {
                size_t pos = line.find(':');
                if (pos != string::npos) {
                    string key = trim(line.substr(0, pos));
                    string val = trim(line.substr(pos + 1));
                    if (key == "FORMULA_AFF") curTheory.formulaAff = val;
                    else if (key == "FORMULA_NEG") curTheory.formulaNeg = val;
                    else if (key == "FORMULA_Q") curTheory.formulaQ = val;
                    else if (key == "SIGNALS") curTheory.signals = val;
                    else if (key == "EXAMPLE") curTheory.example = val;
                }
                continue;
            }

            if (inQuestion) {
                if (optionIndex == 0) { curQ.question = line; optionIndex = 1; }
                else if (optionIndex >= 1 && optionIndex <= 4) {
                    string opt = line;
                    size_t start = opt.find_first_not_of(" \t");
                    if (start != string::npos && opt.size() > start + 2) opt = opt.substr(start + 2);
                    curQ.options.push_back(trim(opt));
                    optionIndex++;
                }
                else if (line.find("ANSWER:") == 0) {
                    string num = trim(line.substr(7));
                    try { curQ.correct = stoi(num); } catch (...) { curQ.correct = 1; }
                    curQs.push_back(curQ);
                    optionIndex = 0; inQuestion = false;
                }
            }
        }
        file.close();
        return true;
    }

    void learnGrammars() {
        while (true) {
            cout << "\n=== HỌC 12 THÌ ===\n";
            for (int i = 1; i <= 12; ++i) cout << i << ". " << getTenseName(i) << "\n";
            cout << "0. Thoát\n";

            int choice = 0;
            while (true) {
                string tmp = safeInput("Chọn (0-12): ");
                try { choice = stoi(tmp); if (choice >= 0 && choice <= 12) break; }
                catch (...) {}
                cout << "Nhập số từ 0 đến 12.\n";
            }
            if (choice == 0) break;

            string tense = getTenseName(choice);
            if (theories.find(tense) == theories.end()) {
                cout << "Chưa có dữ liệu cho thì này.\n";
                continue;
            }

            auto it = grammarQuestions.find(tense);
            if (it == grammarQuestions.end() || it->second.empty()) {
                showTheory(tense, theories[tense]);
                safeInput("\nNhấn Enter để tiếp tục...");
                continue;
            }

            showTheory(tense, theories[tense]);
            vector<Question> quiz = it->second;
            int num = min(10, (int)quiz.size());
            random_device rd; mt19937 g(rd());
            shuffle(quiz.begin(), quiz.begin() + num, g);

            int score = 0;
            cout << "\n--- ÔN TẬP " << tense << " (" << num << " CÂU) ---\n";
            for (int i = 0; i < num; ++i) {
                cout << "\n" << i + 1 << ". " << quiz[i].question << "\n";
                for (size_t j = 0; j < quiz[i].options.size(); ++j)
                    cout << "  " << j + 1 << ". " << quiz[i].options[j] << "\n";

                int ans = 0;
                while (true) {
                    string tmp = safeInput("Chọn (1-4): ");
                    try { ans = stoi(tmp); if (ans >= 1 && ans <= 4) break; }
                    catch (...) {}
                    cout << "Nhập số từ 1 đến 4.\n";
                }

                if (ans == quiz[i].correct) { cout << "Đúng!\n"; score++; }
                else cout << "Sai. Đáp án: " << quiz[i].correct << ". " << quiz[i].options[quiz[i].correct - 1] << "\n";
            }
            cout << "\nKết quả: " << score << "/" << num << "\n";
            safeInput("\nNhấn Enter để tiếp tục...");
        }
    }
};

// ==============================
// VocabularyApp
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
            for (size_t i = 0; i < account.getSavedWords()->getWords().size(); ++i) {
                const Word& w = account.getSavedWords()->getWords()[i];
                if (w.learned) learnedEng.insert(normalize(w.english));
            }
        }
        for (size_t i = 0; i < allWords.size(); ++i) allWords[i].learned = learnedEng.count(normalize(allWords[i].english));
    }

    bool needsReview(const Word& w) {
        if (w.isMastered || w.isNew || !w.learned) return false;
        return (time(nullptr) - w.lastReview) >= (7 * 24 * 3600);
    }

    void moveToMastered(const Word& w) {
        ifstream in("mastered_words.txt");
        string line;
        while (getline(in, line)) {
            string eng; stringstream ss(line); getline(ss, eng, '|');
            if (eng == w.english) return;
        }
        ofstream out("mastered_words.txt", ios::app);
        if (out) out << w.english << "|" << w.vietnamese << "|" << w.ipaText << "|" << w.example << "|" << w.topic << "\n";
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

        vector<const Word*> reviewWords;
        if (account.isLogin() && account.getSavedWords()) {
            for (size_t i = 0; i < wordList.size(); ++i) {
                const Word& w = wordList[i];
                auto it = find_if(account.getSavedWords()->getWords().begin(),
                                  account.getSavedWords()->getWords().end(),
                                  [&](const Word& sw) { return sw.english == w.english; });
                if (it != account.getSavedWords()->getWords().end() && needsReview(*it))
                    reviewWords.push_back(&(*it));
            }
        }

        if (!reviewWords.empty()) {
            cout << "\nÔN TẬP LẠI (" << reviewWords.size() << " từ cần)\n";
            random_device rd; mt19937 g(rd());
            shuffle(reviewWords.begin(), reviewWords.end(), g);

            for (size_t i = 0; i < reviewWords.size(); ++i) {
                Word& w = const_cast<Word&>(*reviewWords[i]);
                cout << "\nTừ: " << w.english << " (IPA: " << w.ipaText << ")\n";
                cout << "? Nghĩa: ";
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

        cout << "\nTỪ TRONG: " << selected << " (" << wordList.size() << " từ)\n";
        for (size_t i = 0; i < wordList.size(); ++i)
            cout << i + 1 << ". " << wordList[i].english << (wordList[i].learned ? " [ĐÃ HỌC]" : "") << "\n";

        while (true) {
            int wordIdx = getChoice(wordList.size(), "\nChọn từ (0 để thoát): ");
            if (wordIdx <= 0) break;
            const Word& w = wordList[wordIdx - 1];
            saveHistory(w.english);
            cout << "\nCHI TIẾT TỪ\n";
            cout << "Từ: " << w.english << "\n";
            cout << "Nghĩa: " << w.vietnamese << "\n";
            cout << "Phiên âm: " << w.ipaText << "\n";
            cout << "Ví dụ: " << w.example << "\n";
            cout << "Chủ đề: " << w.topic << "\n";

            string c = safeInput("\nPhát âm? (1: Có / 0: Không): ");
            if (trim(c) == "1") {
                speak(w.english);
                string r = safeInput("Lặp lại? (y/n): ");
                if (!trim(r).empty() && tolower(trim(r)[0]) == 'y') speakRepeat(w.english);
            }

            if (account.isLogin()) {
                account.getSavedWords()->autoLearn(w.english);
                refreshLearnedStatus();
            }

            if (account.isLogin() && !account.isWordSaved(w.english)) {
                string yn = safeInput("\nLưu từ này? (y/n): ");
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
    void buildData() {
    allWords.clear(); topicMap.clear(); topicList.clear(); globalDict = Dictionary();

    ifstream check("Data\\animals.csv");
    if (!check.good()) {
        ofstream sample("Data\\animals.csv");
        sample << "Word,Meaning (Vietnamese),IPA,Example\n";
        sample << "cat,con mèo,/kæt/,The cat is sleeping on the sofa.\n";
        sample << "dog,con chó,/dɒɡ/,The dog is barking loudly.\n";
        sample << "bird,chim,/bɜːd/,Birds can fly.\n";
        sample.close();
        cout << "Đã tạo file mẫu: Data\\animals.csv\n";
    }
    check.close();

    vector<string> csvFiles = getCsvFiles();
    if (csvFiles.empty()) {
        cout << "Không tìm thấy file CSV nào trong thư mục Data.\n";
        safeInput("Nhấn Enter để tiếp tục...");
        return;
    }

    int total = 0;
    for (size_t i = 0; i < csvFiles.size(); ++i) {
        string filePath = csvFiles[i];
        size_t pos = filePath.find_last_of("\\/");
        string fileName = (pos == string::npos) ? filePath : filePath.substr(pos + 1);
        string topicName = fileName.substr(0, fileName.find(".csv"));

        // SỬA TẠI ĐÂY: DÙNG insert() thay operator[]
        pair<map<string, Topic>::iterator, bool> result = topicMap.insert(
            make_pair(topicName, Topic(topicName))
        );
        Topic& topic = result.first->second;
        if (result.second) {
            topicList.push_back(topicName);
        }

        ifstream file(filePath.c_str());
        if (!file.is_open()) continue;

        string header; getline(file, header);
        // Bỏ BOM UTF-8
        if (header.size() >= 3 && (unsigned char)header[0] == 0xEF &&
            (unsigned char)header[1] == 0xBB && (unsigned char)header[2] == 0xBF)
            header = header.substr(3);

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            vector<string> parts;
            string current;
            bool inQuote = false;
            for (size_t j = 0; j <= line.size(); ++j) {
                char c = (j < line.size()) ? line[j] : ',';
                if (c == '"') inQuote = !inQuote;
                else if (c == ',' && !inQuote) {
                    parts.push_back(trim(current));
                    current.clear();
                } else current += c;
            }
            if (parts.size() < 4) continue;

            Word w(parts[0], parts[1], parts[2], parts[3], topicName);
            topic.addWord(w);
            globalDict.add(w);
            allWords.push_back(w);
            total++;
        }
        file.close();
    }

    sort(topicList.begin(), topicList.end(),
         [&](const string& a, const string& b) { return normalize(a) < normalize(b); });

    refreshLearnedStatus();
    cout << "Đã tải " << total << " từ vựng từ " << topicMap.size() << " chủ đề!\n\n";
}

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
                for (size_t j = 0; j < topic.getWords().size(); ++j)
                    if (topic.getWords()[j].learned) learned++;
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

    void searchWord() {
        cout << "\nTRA TỪ ĐIỂN\n";
        string input = safeInput("Nhập từ cần tra: ");
        input = trim(input);
        if (input.empty() || input == "0") return;

        Word* w = globalDict.findEng(input);
        if (!w) w = globalDict.findVie(input);

        if (w) {
            saveHistory(w->english);
            cout << "\nKẾT QUẢ TRA CỨU\n";
            cout << "Tiếng Anh: " << w->english << "\n";
            cout << "Tiếng Việt: " << w->vietnamese << "\n";
            cout << "Phiên âm: " << w->ipaText << "\n";
            cout << "Ví dụ: " << w->example << "\n";
            cout << "Chủ đề: " << w->topic << "\n";

            string c = safeInput("\nPhát âm? (1: Có / 0: Không): ");
            if (trim(c) == "1") {
                speak(w->english);
                string r = safeInput("Lặp lại? (y/n): ");
                if (!trim(r).empty() && tolower(trim(r)[0]) == 'y') speakRepeat(w->english);
            }

            if (account.isLogin()) {
                account.getSavedWords()->autoLearn(w->english);  // Chỉ ghi nhận
                refreshLearnedStatus();
            }

            if (account.isLogin() && !account.isWordSaved(w->english)) {
                string yn = safeInput("\nLưu từ này vào danh sách cá nhân? (y/n): ");
                if (!trim(yn).empty() && tolower(trim(yn)[0]) == 'y') {
                    account.saveWord(*w);  // GỌI addOrUpdate → LƯU CHÍNH THỨC
                    cout << "Đã lưu chính thức!\n";
                }
            }
        } else {
            cout << "\nKHÔNG TÌM THẤY \"" << input << "\"\n";
            auto s1 = globalDict.suggestEng(input);
            if (!s1.empty()) {
                cout << "Gợi ý (Anh): ";
                for (size_t i = 0; i < s1.size(); ++i)
                    cout << s1[i] << (i < s1.size()-1 ? ", " : "\n");
            }
        }
        cout << "────────────────────\n";
    }

    void showProgress() {
    refreshLearnedStatus();

    // --- Khai báo biến ---
    int totalWords = allWords.size();
    int learnedTotal = 0, masteredTotal = 0, needReviewCount = 0;
    int learnedTodayCount = 0, reviewedTodayCount = 0, totalEXP = 0;

    time_t now = time(nullptr);
    struct tm* todayTm = localtime(&now);
    int todayKey = (todayTm->tm_year + 1900) * 1000 + todayTm->tm_yday;

    set<int> learnedDays;  // Lưu ngày đã học

    // --- Đếm từ đã lưu của user ---
    if (account.isLogin() && account.getSavedWords()) {
        const auto& saved = account.getSavedWords()->getWords();
        for (const auto& w : saved) {
            if (!w.learned) continue;

            learnedTotal++;
            totalEXP += w.exp;
            if (w.isMastered) masteredTotal++;

            if (w.lastReview > 0) {
                struct tm* wt = localtime(&w.lastReview);
                int dayKey = (wt->tm_year + 1900) * 1000 + wt->tm_yday;
                learnedDays.insert(dayKey);

                // Hôm nay?
                if (dayKey == todayKey) {
                    if (w.isNew) learnedTodayCount++;
                    else reviewedTodayCount++;
                }
            }

            // Cần ôn?
            if (needsReview(w)) needReviewCount++;
        }
    }

    // --- Đếm từ thành thạo ngoài file saved ---
    ifstream mf("mastered_words.txt");
    string line;
    while (getline(mf, line)) {
        if (trim(line).empty()) continue;
        string eng;
        stringstream ss(line);
        getline(ss, eng, '|');
        if (!eng.empty() && 
            (!account.isLogin() || !account.getSavedWords()->contains(eng)))
            masteredTotal++;
    }
    mf.close();

    // --- Tính Streak ---
    int streak = 0;
    int checkDay = todayKey;
    while (learnedDays.count(checkDay)) {
        streak++;
        checkDay--;
    }

    // --- HIỂN THỊ ---
    cout << "\n";
    cout << "═══════════════════════════════════════\n";
    cout << "        THỐNG KÊ HỌC TẬP CHI TIẾT        \n";
    cout << "═══════════════════════════════════════\n";

    cout << "HÔM NAY (" << put_time(todayTm, "%d/%m/%Y") << ")\n";
    cout << " • Học mới: " << learnedTodayCount << " từ\n";
    cout << " • Ôn lại: " << reviewedTodayCount << " từ\n";
    cout << " • Tổng hoạt động: " << (learnedTodayCount + reviewedTodayCount) << " lần\n";

    cout << "\nTIẾN ĐỘ TỔNG\n";
    cout << " • Đã học: " << learnedTotal << " / " << totalWords << " từ";
    if (totalWords > 0)
        cout << " (" << fixed << setprecision(1) << (learnedTotal * 100.0 / totalWords) << "%)";
    cout << "\n • Thành thạo: " << masteredTotal << " từ\n";
    cout << " • Cần ôn lại: " << needReviewCount << " từ (quá 7 ngày)\n";
    cout << " • Tổng EXP: " << totalEXP << " điểm\n";

    cout << "\nSTREAK (liên tục học)\n";
    if (streak > 0)
        cout << " • Bạn đang giữ: " << streak << " ngày liên tục!\n";
    else
        cout << " • Chưa có streak. Học hôm nay để bắt đầu!\n";

    cout << "═══════════════════════════════════════\n";
    safeInput("\nNhấn Enter để tiếp tục...");
}
    void menu() {
        enableUTF8();
        buildData();

        if (!grammar.loadFromFile("grammar_data.txt"))
            cout << "Cảnh báo: Không tải được grammar_data.txt\n";

        auto history = loadHistory();
        if (!history.empty()) {
            cout << "\nLỊCH SỬ TRA TỪ (5 từ gần nhất)\n";
            for (int i = history.size() - 1; i >= 0; --i)
                cout << "• " << history[i] << "\n";
            cout << "────────────────────\n";
        }

        int choice;
        do {
            cout << "\n";
            cout << "════════════════════════════════════\n";
            cout << "        TỪ ĐIỂN ANH - VIỆT PRO+      \n";
            cout << "════════════════════════════════════\n";
            if (account.isLogin()) cout << "Người dùng: " << account.getUsername() << "\n";

            cout << "1. Xem từ theo chủ đề\n";
            cout << "2. Tra từ (Anh <-> Việt)\n";
            cout << "3. Xem tiến độ học\n";
            cout << "4. Từ đã lưu (cá nhân)\n";
            cout << "5. Đăng nhập / Đăng ký\n";
            cout << "6. Đăng xuất\n";
            cout << "7. Xóa từ đã lưu\n";
                        if (account.isLogin()) {
                cout << "8. Ôn 12 thì tiếng Anh\n";
                cout << "9. Xem từ đã lưu & quản lý\n";  
            }
            cout << "0. Thoát\n";
            cout << "════════════════════════════════════\n";
            int maxChoice = account.isLogin() ? 9 : 7; 
            choice = getChoice(maxChoice, "Chọn (0-" + to_string(maxChoice) + "): ");
            if (choice == 0) break;

            switch (choice) {
                case 1: viewTopic(); break;
                case 2: searchWord(); break;
                case 3: showProgress(); break;
                case 4: account.showSavedWords(); break;
                case 5: {
                    cout << "1. Đăng nhập\n2. Đăng ký\n";
                    int sub = getChoice(2, "Chọn: ");
                    if (sub == 1) account.login();
                    else if (sub == 2) account.signUp();
                    break;
                }
                case 6: account.logout(); break;
                case 7: {
                    if (!account.isLogin()) { cout << "Đăng nhập trước!\n"; break; }
                    string eng = safeInput("Nhập từ tiếng Anh muốn xóa: ");
                    if (!trim(eng).empty()) account.deleteSavedWord(trim(eng));
                    break;
                }
                case 8: if (account.isLogin()) grammar.learnGrammars(); else cout << "Đăng nhập trước!\n"; break;
                case 9:
                    if (!account.isLogin()) {
                        cout << "Đăng nhập trước!\n";
                        break;
                    }
                    account.showSavedWords();
                    safeInput("\nNhấn Enter để tiếp tục...");
                    break;
            }
        } while (true);

        if (account.isLogin()) account.logout();
        cout << "\nCảm ơn bạn đã sử dụng từ điển!\n";
    }
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