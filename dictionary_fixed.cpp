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
    size_t end   = s.find_last_not_of (" \t\r\n");
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
// Word
// ==============================
struct Word {
    string english, vietnamese, ipaText, example, topic;
    bool learned = false;
    int exp = 0;
    bool isNew = true;

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
        if (k1 < k2) node->left  = insert(node->left,  w, eng);
        else if (k1 > k2) node->right = insert(node->right, w, eng);
        return node;
    }

    Word* find(Node* node, const string& key, bool eng) {
        if (!node) return nullptr;
        string k = normalize(eng ? node->data.english : node->data.vietnamese);
        if (key == k) return &node->data;
        return (key < k) ? find(node->left,  key, eng) : find(node->right, key, eng);
    }

    void suggest(Node* node, const string& key, bool eng, vector<string>& res, int limit) {
        if (!node || (int)res.size() >= limit) return;
        string w = normalize(eng ? node->data.english : node->data.vietnamese);
        if (w.find(key) != string::npos) res.push_back(eng ? node->data.english : node->data.vietnamese);
        suggest(node->left,  key, eng, res, limit);
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

    void saveToFile() const {
        ofstream f(saveFile);
        if (!f) return;
        for (const auto& w : words)
            f << w.english << "|" << w.vietnamese << "|" << w.ipaText << "|" << w.example << "|"
              << w.exp << "|" << (w.isNew ? "1" : "0") << "|" << w.topic << "|" << (w.learned ? "1" : "0") << "\n";
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
            if (parts.size() < 8) continue;

            Word w(parts[0], parts[1], parts[2], parts[3], parts[6]);
            try { w.exp = stoi(parts[4]); } catch(...) { w.exp = 0; }
            w.isNew = (parts[5] == "1");
            w.learned = (parts[7] == "1");

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
            cout << "Cập nhật: " << word.english << " (+10 EXP) [" << word.topic << "]\n";
        } else {
            Word newWord = word;
            newWord.exp = 10;
            newWord.isNew = true;
            newWord.learned = true;
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
        } else {
            Word temp(eng, "", "", "", "");
            temp.learned = true;
            temp.exp = 10;
            temp.isNew = false;
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
        loadFromFile();
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
                 << " | Chủ đề: " << w.topic << "\n";
        }
    }

    bool contains(const string& eng) const {
        return any_of(words.begin(), words.end(),
                      [&](const Word& w) { return w.english == eng; });
    }

    const vector<Word>& getWords() const { return words; }
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
// GrammarTrainer – ĐẦY ĐỦ 100%
// ==============================
class GrammarTrainer {
    struct Theory { string formulaAff, formulaNeg, formulaQ, signals, example; };
    struct Question { string question; vector<string> options; int correct; };
    map<string, Theory> theories;
    map<string, vector<Question>> grammarQuestions;

    struct MCQ { string question; vector<string> options; int answer; };
    struct GapFill { string question, answer, hint, baseVerb; };
    struct Rearrange { string jumbled, correct; };
    struct PracticeTest { vector<MCQ> mcqs; vector<GapFill> gapfills; vector<Rearrange> rearranges; };
    vector<PracticeTest> practiceTests;

    string getTenseName(int c) {
        static const vector<string> names = {
            "", "PRESENT SIMPLE", "PRESENT CONTINUOUS", "PRESENT PERFECT", "PRESENT PERFECT CONTINUOUS",
            "PAST SIMPLE", "PAST CONTINUOUS", "PAST PERFECT", "PAST PERFECT CONTINUOUS",
            "FUTURE SIMPLE", "FUTURE CONTINUOUS", "FUTURE PERFECT", "FUTURE PERFECT CONTINUOUS"
        };
        return (c >= 1 && c <= 12) ? names[c] : "";
    }

public:
    bool loadFromFile(const string& filename) {
        ifstream file(filename);
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
                    try { curQ.correct = stoi(num); } catch(...) { curQ.correct = 1; }
                    curQs.push_back(curQ);
                    optionIndex = 0; inQuestion = false;
                }
            }
        }
        file.close();
        return true;
    }

    bool loadPracticeFile(const string& filename) {
        ifstream f(filename);
        if (!f.is_open()) return false;

        string line;
        PracticeTest cur;
        bool inMCQ = false, inGap = false, inRearr = false;
        MCQ curMCQ; GapFill curGap; Rearrange curRearr;
        int mcqStep = 0;

        auto trimStr = [](string& s) {
            s.erase(0, s.find_first_not_of(" \t\r\n"));
            s.erase(s.find_last_not_of(" \t\r\n") + 1);
        };

        while (getline(f, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.empty()) continue;

            if (line.front() == '[' && line.back() == ']') {
                string tag = line.substr(1, line.size() - 2);
                if (tag == "END") { practiceTests.push_back(cur); cur = PracticeTest(); inMCQ = inGap = inRearr = false; mcqStep = 0; }
                else if (tag == "MCQ") { inMCQ = true; inGap = inRearr = false; mcqStep = 0; }
                else if (tag == "GAPFILL") { inGap = true; inMCQ = inRearr = false; }
                else if (tag == "REARRANGE") { inRearr = true; inMCQ = inGap = false; }
                continue;
            }

            if (inMCQ) {
                if (mcqStep == 0) { curMCQ = MCQ(); curMCQ.question = line; mcqStep = 1; }
                else if (mcqStep >= 1 && mcqStep <= 4) {
                    string opt = line;
                    size_t start = opt.find_first_not_of(" \t");
                    if (start != string::npos && opt.size() > start + 2) opt = opt.substr(start + 2);
                    curMCQ.options.push_back(trim(opt));
                    mcqStep++;
                }
                else if (line.find("ANSWER:") == 0) {
                    curMCQ.answer = stoi(trim(line.substr(7)));
                    cur.mcqs.push_back(curMCQ);
                    mcqStep = 0;
                }
            }
            else if (inGap) {
                size_t p = line.find('=');
                if (p != string::npos) {
                    GapFill g;
                    string left = line.substr(0, p);
                    string right = line.substr(p + 1);
                    trimStr(left); trimStr(right);

                    size_t verbStart = left.find(" (");
                    if (verbStart != string::npos && left.back() == ')') {
                        size_t verbEnd = left.size() - 1;
                        g.baseVerb = left.substr(verbStart + 2, verbEnd - verbStart - 2);
                        trimStr(g.baseVerb);
                        g.question = left.substr(0, verbStart) + " _____";
                        trimStr(g.question);
                    } else {
                        g.baseVerb = "";
                        g.question = left;
                    }

                    size_t paren = right.find(" (");
                    if (paren != string::npos) {
                        g.answer = right.substr(0, paren);
                        g.hint = right.substr(paren + 2, right.size() - paren - 3);
                        trimStr(g.hint);
                    } else {
                        size_t bar = right.find('|');
                        if (bar != string::npos) {
                            g.answer = right.substr(0, bar);
                            g.hint = right.substr(bar + 1);
                        } else { g.answer = right; g.hint = ""; }
                    }
                    trimStr(g.answer);
                    cur.gapfills.push_back(g);
                }
            }
            else if (inRearr) {
                size_t p = line.find('=');
                if (p != string::npos) {
                    string l = line.substr(0, p), r = line.substr(p + 1);
                    trimStr(l); trimStr(r);
                    curRearr.jumbled = l; curRearr.correct = r;
                    cur.rearranges.push_back(curRearr);
                }
            }
        }
        f.close();
        return true;
    }

    void practiceTest() {
        if (practiceTests.empty()) { cout << "Chưa có bài ôn.\n"; return; }

        random_device rd; mt19937 g(rd());
        int testIdx = uniform_int_distribution<int>(0, (int)practiceTests.size() - 1)(g);
        const auto& test = practiceTests[testIdx];

        int score = 0, total = 0;
        cout << "\n=== ÔN TẬP TỔNG HỢP ===\n";

        auto compareAnswer = [](const string& user, const string& correct) -> bool {
            auto clean = [](string s) -> string {
                string res;
                bool space = false;
                for (char ch : s) {
                    if (isspace(static_cast<unsigned char>(ch))) {
                        if (!space && !res.empty()) { res += ' '; space = true; }
                    } else {
                        res += tolower(static_cast<unsigned char>(ch));
                        space = false;
                    }
                }
                if (!res.empty() && res.front() == ' ') res.erase(0, 1);
                if (!res.empty() && res.back() == ' ') res.pop_back();
                return res;
            };
            return clean(user) == clean(correct);
        };

        for (const auto& q : test.mcqs) {
            cout << "\n" << q.question << "\n";
            for (size_t i = 0; i < q.options.size(); ++i)
                cout << "  " << i+1 << ". " << q.options[i] << "\n";

            int ans = 0;
            while (true) {
                string tmp = safeInput("Chọn (1-4): ");
                tmp = trim(tmp);
                if (tmp.empty()) continue;
                try { ans = stoi(tmp); if (ans >= 1 && ans <= 4) break; }
                catch (...) {}
                cout << "Nhập số từ 1 đến 4.\n";
            }
            if (ans == q.answer) { cout << "Đúng!\n"; score++; }
            else cout << "Sai! Đáp án: " << q.options[q.answer-1] << "\n";
            total++;
        }

        for (const auto& g : test.gapfills) {
            cout << "\n" << g.question;
            if (!g.baseVerb.empty()) cout << "  (gợi ý: " << g.baseVerb << " → đổi thì phù hợp)";
            cout << "\n→ ";
            string ans = safeInput();
            if (compareAnswer(ans, g.answer)) { cout << "Đúng!\n"; score++; }
            else {
                cout << "Sai! Đáp án đúng: \"" << g.answer << "\"\n";
                if (!g.hint.empty()) cout << "   Gợi ý: " << g.hint << "\n";
            }
            total++;
        }

        for (const auto& r : test.rearranges) {
            cout << "\nSắp xếp: " << r.jumbled << "\n→ ";
            string ans = safeInput();
            if (compareAnswer(ans, r.correct)) { cout << "Đúng!\n"; score++; }
            else cout << "Sai. Đáp án: " << r.correct << "\n";
            total++;
        }

        cout << "\nKết quả: " << score << "/" << total << "\n";
        safeInput("Nhấn Enter để tiếp tục...");
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
                safeInput("\nNhấn Enter để tiếp tục...");
                continue;
            }

            const auto& t = theories[tense];
            cout << "\n===== " << tense << " =====\n";
            cout << "Khẳng định: " << t.formulaAff << "\n";
            cout << "Phủ định: " << t.formulaNeg << "\n";
            cout << "Nghi vấn: " << t.formulaQ << "\n";
            cout << "Dấu hiệu: " << t.signals << "\n";
            cout << "Ví dụ: " << t.example << "\n";

            auto it = grammarQuestions.find(tense);
            if (it == grammarQuestions.end() || it->second.empty()) {
                cout << "Chưa có câu hỏi ôn tập cho thì này.\n";
                safeInput("\nNhấn Enter để tiếp tục...");
                continue;
            }

            vector<Question> source = it->second;
            int numQuestions = min(10, (int)source.size());
            vector<Question> quiz(source.begin(), source.begin() + numQuestions);
            random_device rd; mt19937 g(rd());
            shuffle(quiz.begin(), quiz.end(), g);

            int score = 0;
            cout << "\n--- ÔN TẬP " << tense << " (" << numQuestions << " CÂU) ---\n";
            for (int i = 0; i < numQuestions; ++i) {
                cout << "\n" << i + 1 << ". " << quiz[i].question << "\n";
                for (size_t j = 0; j < quiz[i].options.size(); ++j)
                    cout << "  " << j + 1 << ". " << quiz[i].options[j] << "\n";

                int ans = 0;
                while (true) {
                    string tmp = safeInput("Chọn (1-4): ");
                    tmp = trim(tmp);
                    if (tmp.empty()) continue;
                    try { ans = stoi(tmp); if (ans >= 1 && ans <= 4) break; }
                    catch (...) {}
                    cout << "Nhập số từ 1 đến 4.\n";
                }

                if (ans == quiz[i].correct) { cout << "Đúng!\n"; score++; }
                else {
                    cout << "Sai. Đáp án: " << quiz[i].correct << ". "
                         << quiz[i].options[quiz[i].correct - 1] << "\n";
                }
            }
            cout << "\nKết quả: " << score << "/" << numQuestions << "\n";
            safeInput("\nNhấn Enter để tiếp tục...");
        }
    }
};

// ==============================
// VocabularyApp – CHỈ SỬA viewTopic()
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

    // ĐÚNG HÀM CỦA BẠN
    void suggestNextTopic() {
        cout << "\nSuggested next topic: ";
        if (!topicList.empty())
            cout << topicList[rand() % topicList.size()] << "\n";
        else
            cout << "No topics available.\n";
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

        cout << "\nTỪ TRONG: " << selected << " (" << wordList.size() << " từ)\n";
        for (size_t i = 0; i < wordList.size(); ++i)
            cout << i + 1 << ". " << wordList[i].english << "\n";

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
    void buildData() {
        allWords.clear();
        topicMap.clear();
        topicList.clear();
        globalDict = Dictionary();

        fs::path dataPath = fs::current_path() / "Data";
        if (!fs::exists(dataPath)) {
            cout << "Không tìm thấy thư mục 'Data' → đang tạo...\n";
            fs::create_directory(dataPath);
            ofstream sample(dataPath / "animals.csv");
            sample << "English,Vietnamese,IPA,Example,Topic\n";
            sample << "dog,chó,/dɒɡ/,I have a dog.,animals\n";
            sample.close();
            cout << "Đã tạo file mẫu: Data/animals.csv\n";
        }

        int total = 0;
        for (const auto& entry : fs::directory_iterator(dataPath)) {
            if (entry.path().extension() != ".csv") continue;

            string fileName = entry.path().filename().string();
            string topicName = fileName.substr(0, fileName.find(".csv"));
            auto [it, inserted] = topicMap.try_emplace(topicName, topicName);
            Topic& topic = it->second;
            topicList.push_back(topicName);

            ifstream file(entry.path());
            if (!file.is_open()) continue;

            string header; getline(file, header);
            if (header.size() >= 3 && (unsigned char)header[0] == 0xEF &&
                (unsigned char)header[1] == 0xBB && (unsigned char)header[2] == 0xBF)
                header = header.substr(3);

            string line;
            while (getline(file, line)) {
                if (line.empty()) continue;
                string current;
                vector<string> parts;
                bool inQuote = false;
                for (char c : line + ',') {
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
                for (const auto& w : topic.getWords()) if (w.learned) learned++;
                double percent = topic.size() > 0 ? (learned * 100.0 / topic.size()) : 0.0;
                cout << i + 1 << ". " << topicList[i]
                     << " (" << learned << "/" << topic.size() << " - "
                     << fixed << setprecision(0) << percent << "%)\n";
            }

            int suggestOption = topicList.size() + 1;
            cout << suggestOption << ". Đề xuất 1 chủ đề\n";

            int maxChoice = suggestOption;
            int choice = getChoice(maxChoice, "\nChọn (0 để thoát): ");
            if (choice == 0) return;

            if (choice == suggestOption) {
                suggestNextTopic();

                if (!topicList.empty()) {
                    cout << "Nhấn 1 để vào ngay chủ đề này!\n";
                    cout << "Nhấn Enter để quay lại.\n";
                    string go = safeInput("→ ");
                    if (trim(go) == "1") {
                        int randIdx = rand() % topicList.size();
                        viewTopicByIndex(randIdx + 1);
                    }
                } else {
                    safeInput("Nhấn Enter để tiếp tục...");
                }
                continue;
            }

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

#ifdef _WIN32
            string c = safeInput("\nPhát âm? (1: Có / 0: Không): ");
            if (trim(c) == "1") {
                speak(w->english);
                string r = safeInput("Lặp lại? (y/n): ");
                if (!trim(r).empty() && tolower(trim(r)[0]) == 'y')
                    speakRepeat(w->english);
            }
#endif

            if (account.isLogin()) {
                account.getSavedWords()->autoLearn(w->english);
                refreshLearnedStatus();
            }

            if (account.isLogin() && !account.isWordSaved(w->english)) {
                string yn = safeInput("\nLưu từ này? (y/n): ");
                if (!trim(yn).empty() && tolower(trim(yn)[0]) == 'y') {
                    account.saveWord(*w);
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
        int total = allWords.size();
        int learned = 0;
        for (const auto& w : allWords) if (w.learned) learned++;

        double percent = total > 0 ? (learned * 100.0 / total) : 0.0;
        cout << fixed << setprecision(1);
        cout << "\nTIẾN ĐỘ HỌC TẬP\n";
        cout << "Đã học: " << learned << "/" << total << " từ (" << percent << "%)\n";

        if (account.isLogin() && account.getSavedWords()) {
            int totalEXP = 0;
            for (const auto& w : account.getSavedWords()->getWords())
                totalEXP += w.exp;
            cout << "Tổng EXP: " << totalEXP << " điểm\n";
        }
        cout << "────────────────────\n";
    }

    void menu() {
        enableUTF8();
        buildData();

        if (!grammar.loadFromFile("grammar_data.txt"))
            cout << "Cảnh báo: Không tải được grammar_data.txt\n";
        if (!grammar.loadPracticeFile("practice_data.txt"))
            cout << "Cảnh báo: Không tải được practice_data.txt\n";

        auto history = loadHistory();
        if (!history.empty()) {
            cout << "\nLỊCH SỬ TRA TỪ (5 từ gần nhất)\n";
            for (int i = history.size() - 1; i >= 0; --i)
                cout << "• " << history[i] << "\n";
            cout << "────────────────────\n";
        }

        int choice;
        do {
            cout << "\n════════════════════════════════════\n";
            cout << "        TỪ ĐIỂN ANH - VIỆT PRO+      \n";
            cout << "════════════════════════════════════\n";
            if (account.isLogin()) cout << "Người dùng: " << account.getUsername() << "\n";

            cout << "1. Xem từ theo chủ đề\n";
            cout << "2. Tra từ (Anh to Việt)\n";
            cout << "3. Xem tiến độ học\n";
            cout << "4. Từ đã lưu (cá nhân)\n";
            cout << "5. Đăng nhập / Đăng ký\n";
            cout << "6. Đăng xuất\n";
            cout << "7. Xóa từ đã lưu\n";
            if (account.isLogin()) {
                cout << "8. Ôn 12 thì tiếng Anh\n";
                cout << "9. Ôn tập Grammar tổng hợp\n";
            }
            cout << "0. Thoát\n";
            cout << "────────────────────────────────────\n";

            int maxChoice = account.isLogin() ? 9 : 7;
            choice = getChoice(maxChoice, "Chọn (0-" + to_string(maxChoice) + "): ");
            if (choice == 0) break;

            switch (choice) {
                case 1: viewTopic(); refreshLearnedStatus(); break;
                case 2: searchWord(); refreshLearnedStatus(); break;
                case 3: showProgress(); break;
                case 4: account.showSavedWords(); break;
                case 5: {
                    cout << "1. Đăng nhập\n2. Đăng ký\n";
                    int sub = getChoice(2, "Chọn: ");
                    if (sub == 1) account.login();
                    else if (sub == 2) account.signUp();
                    buildData();
                    break;
                }
                case 6: account.logout(); buildData(); break;
                case 7: {
                    if (!account.isLogin()) { cout << "Đăng nhập trước!\n"; break; }
                    string eng = safeInput("Nhập từ tiếng Anh muốn xóa: ");
                    if (!trim(eng).empty()) account.deleteSavedWord(trim(eng));
                    buildData();
                    break;
                }
                case 8: if (account.isLogin()) grammar.learnGrammars(); break;
                case 9: if (account.isLogin()) grammar.practiceTest(); break;
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
