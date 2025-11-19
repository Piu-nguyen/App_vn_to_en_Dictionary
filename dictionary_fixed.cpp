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
#include <atomic>
#include <cmath>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif
using namespace std;
namespace fs = std::filesystem;

// ==============================
// ENUM CLASS: Trạng thái học tập
// ==============================
enum class LearnStage : int {
    NotLearned = 0, // Chưa học
    Basic = 1, // Level 1–5
    Advanced = 2, // Level 6–9
    Mastered = 3 // Level 10
};

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
    if (!getline(cin, line)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
        {char(0x111),'d'}, {char(0x110),'D'}, // đ Đ
        {char(0x1A1),'o'}, {char(0x1A0),'O'}, // ơ Ơ
        {char(0x1B0),'u'}, {char(0x1AF),'U'}, // ư Ư
        // uppercase
        {char(0xC0),'A'},{char(0xC1),'A'},{char(0x1EA2),'A'},{char(0xC3),'A'},{char(0x1EA0),'A'},
        {char(0x102),'A'},{char(0x1EAE),'A'},{char(0x1EB0),'A'},{char(0x1EB2),'A'},{char(0x1EB4),'A'},{char(0x1EB6),'A'},
        {char(0xC2),'A'},{char(0x1EA6),'A'},{char(0x1EA8),'A'},{char(0x1EAA),'A'},{char(0x1EAC),'A'},
        {char(0xC8),'E'},{char(0xC9),'E'},{char(0x1EBA),'E'},{char(0x1EBC),'E'},{char(0x1EB8),'E'},
        {char(0xCA),'E'},{char(0x1EC0),'E'},{char(0x1EBE),'E'},{char(0x1EC2),'E'},{char(0x1EC4),'E'},{char(0x1EC6),'E'},
        {char(0xCC),'I'},{char(0xCD),'I'},{char(0x1EC8),'I'},{char(0x128),'I'},{char(0x1ECA),'I'},
        {char(0xD2),'O'},{char(0xD3),'O'},{char(0x1ECE),'O'},{char(

0xD5),'O'},{char(0x1ECC),'O'},
        {char(0xD4),'O'},{char(0x1ED2),'O'},{char(0x1ED0),'O'},{char(0x1ED4),'O'},{char(0x1ED6),'O'},{char(0x1ED8),'O'},
        {char(0x1A0),'O'},{char(0x1EDC),'O'},{char(0x1EDA),'O'},{char(0x1EDE),'O'},{char(0x1EE0),'O'},{char(0x1EE2),'O'},
        {char(0xD9),'U'},{char(0xDA),'U'},{char(0x1EE6),'U'},{char(0x168),'U'},{char(0x1EE4),'U'},
        {char(0x1AF),'U'},{char(0x1EEA),'U'},{char(0x1EEC),'U'},{char(0x1EEE),'U'},{char(0x1EF0),'U'},
        {char(0x1EF2),'Y'},{char(0xDD),'Y'},{char(0x1EF6),'Y'},{char(0x1EF8),'Y'},{char(0x1EF4),'Y'}
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
// Phát âm (Windows + Linux/macOS fallback)
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
    atomic<bool> stop{false};
    thread t([text, &stop]() {
        while (!stop) {
            speak(text);
            std::this_thread::sleep_for(chrono::milliseconds(800));
        }
    });
    safeInput("");
    stop = true;
    t.join();
}
#else
void speak(const string& text) {
    string cmd = "espeak \"" + text + "\" 2>/dev/null || say \"" + text + "\"";
    system(cmd.c_str());
}
void speakRepeat(const string& text) {
    cout << "\nTính năng lặp lại chỉ hỗ trợ trên Windows.\n";
    safeInput("Nhấn Enter để tiếp tục...");
}
#endif

// ==============================
// Lịch sử tra từ
// ==============================
void saveHistory(const string& word) {
    ofstream f("history.txt", ios::app);
    if (f.is_open()) { f << word << "\n"; f.close(); }
}
vector<string> loadHistory(int limit = 10) {
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
// Word (với enum)
// ==============================
struct Word {
    string english, vietnamese, ipaText, example, topic;
    bool learned = false;
    int exp = 0;
    int level = 0;
    bool isNew = true;
    bool isMastered = false;
    Word() = default;
    Word(const string& e, const string& v, const string& ipaTxt, const string& ex, const string& t = "")
        : english(e), vietnamese(v), ipaText(ipaTxt), example(ex), topic(t) {}
    int expToNextLevel() const {
        if (level >= 10) return 0;
        if (level < 5) return 100;
        return static_cast<int>(100 * pow(1.2, level - 4));
    }
    LearnStage getStage() const {
        if (level == 0) return LearnStage::NotLearned;
        if (level <= 5) return LearnStage::Basic;
        if (level < 10) return LearnStage::Advanced;
        return LearnStage::Mastered;
    }
    string stageName() const {
        switch (getStage()) {
            case LearnStage::NotLearned: return "Chưa học";
            case LearnStage::Basic: return "Cơ bản";
            case LearnStage::Advanced: return "Nâng cao";
            case LearnStage::Mastered: return "Thông thạo";
        }
        return "";
    }
};

// ==============================
// BST Dictionary
// ==============================
struct Node {
    Word data;
    Node *left = nullptr, *right = nullptr;
    enum class KeyMode { Eng, Vie } mode;
    union {
        struct { char eng_key[64]; };     // chỉ dùng khi mode == Eng
        struct { char vie_key[96]; };     // chỉ dùng khi mode == Vie
    };

    Node(const Word& w, KeyMode m) : data(w), mode(m) {
        if (mode == KeyMode::Eng) {
            strncpy(eng_key, w.english.c_str(), sizeof(eng_key)-1);
            eng_key[sizeof(eng_key)-1] = '\0';
        } else {
        strncpy(vie_key, w.vietnamese.c_str(), sizeof(vie_key)-1);
            vie_key[sizeof(vie_key)-1] = '\0';
        }
    }
};
// ==============================
// class Dictionary
// ==============================
class Dictionary {
protected:
    Node *rootEng = nullptr, *rootVie = nullptr;

    Node* insert(Node* node, const Word& w, bool eng) {
        if (!node) return new Node(w, eng ? Node::KeyMode::Eng : Node::KeyMode::Vie);

        string current_key = eng ? 
            string(node->mode == Node::KeyMode::Eng ? node->eng_key : "") :
            string(node->mode == Node::KeyMode::Vie ? node->vie_key : "");

        string new_key = eng ? w.english : w.vietnamese;

        if (normalize(new_key) < normalize(current_key))
            node->left = insert(node->left, w, eng);
        else if (normalize(new_key) > normalize(current_key))
            node->right = insert(node->right, w, eng);

        return node;
    }

    Word* find(Node* node, const string& key, bool eng) {
        if (!node) return nullptr;

        string node_key = eng ?
            (node->mode == Node::KeyMode::Eng ? string(node->eng_key) : node->data.english) :
            (node->mode == Node::KeyMode::Vie ? string(node->vie_key) : node->data.vietnamese);

        if (normalize(key) == normalize(node_key)) return &node->data;
        if (normalize(key) < normalize(node_key))
            return find(node->left, key, eng);
        return find(node->right, key, eng);
    }

    void suggest(Node* node, const string& prefix, bool eng, vector<string>& res, int limit) {
        if (!node || (int)res.size() >= limit) return;

        string word = eng ? node->data.english : node->data.vietnamese;
        if (normalize(word).find(normalize(prefix)) != string::npos)
            res.push_back(word);

        suggest(node->left, prefix, eng, res, limit);
        suggest(node->right, prefix, eng, res, limit);
    }

    void clear(Node* n) { if (n) { clear(n->left); clear(n->right); delete n; } }

public:
    ~Dictionary() { clear(rootEng); clear(rootVie); }

    void add(const Word& w) { 
        rootEng = insert(rootEng, w, true); 
        rootVie = insert(rootVie, w, false); 
    }

    Word* findEng(const string& k) { return find(rootEng, k, true); }
    Word* findVie(const string& k) { return find(rootVie, k, false); }

    vector<string> suggestEng(const string& k) { vector<string> r; suggest(rootEng, k, true, r, 8); return r; }
    vector<string> suggestVie(const string& k) { vector<string> r; suggest(rootVie, k, false, r, 8); return r; }
};
// ==============================
// SavedWords (dùng enum)
// ==============================
class SavedWords : public Dictionary {
private:
    string username;
    vector<Word> words;
    string saveFile;
    string masteredFile;
    void saveToFile() const {
        ofstream f(saveFile);
        if (!f) return;
        for (const auto& w : words)
            f << w.english << "|" << w.vietnamese << "|" << w.ipaText << "|" << w.example << "|"
              << w.exp << "|" << (w.isNew ? "1" : "0") << "|" << w.topic << "|"
              << (w.learned ? "1" : "0") << "|" << w.level << "|" << (w.isMastered ? "1" : "0") << "\n";
    }
    void saveMastered() const {
        ofstream f(masteredFile);
        if (!f) return;
        for (const auto& w : words)
            if (w.isMastered)
                f << w.english << "|" << w.vietnamese << "|" << w.topic << "\n";
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
            try { w.level = stoi(parts[8]); } catch(...) { w.level = 0; }
            w.isMastered = (parts[9] == "1");
            if (w.level >= 10) w.isMastered = true;
            words.push_back(w);
            Dictionary::add(w);
        }
    }
public:
    SavedWords(const string& user) : username(user),
        saveFile(user + "_words.txt"), masteredFile(user + "_mastered.txt") {
        loadFromFile();
    }
    bool addOrUpdateInteractive(const Word& word, bool askSave = true) {
        auto it = find_if(words.begin(), words.end(),
                          [&](const Word& w) { return w.english == word.english; });
        bool isNewWord = (it == words.end());
        if (!askSave && !isNewWord) return false;
        if (isNewWord) {
            string yn = safeInput("Lưu từ \"" + word.english + "\" vào danh sách cá nhân? (y/n): ");
            if (trim(yn).empty() || tolower(trim(yn)[0]) != 'y') {
                cout << "Đã bỏ qua lưu từ.\n";
                return false;
            }
        }
        if (isNewWord) {
            Word newWord = word;
            newWord.exp = 10;
            newWord.isNew = true;
            newWord.learned = true;
            newWord.level = 1;
            words.push_back(newWord);
            Dictionary::add(newWord);
            cout << "Đã lưu: " << word.english << " (Level 1 - MỚI +10 EXP) [" << word.topic << "]\n";
        } else {
            if (it->getStage() == LearnStage::Mastered) {
                cout << "Từ \"" << word.english << "\" đã thông thạo!\n";
                return false;
            }
            it->exp += 10;
            it->isNew = false;
            it->learned = true;
            int oldLevel = it->level;
            while (it->level < 10 && it->exp >= it->expToNextLevel()) {
                it->exp -= it->expToNextLevel();
                it->level++;
                if (it->level == 10) {
                    it->isMastered = true;
                    cout << "THÔNG THÁO: " << word.english << "!\n";
                    saveMastered();
                    break;
                }
            }
            cout << "Cập nhật: " << word.english << " (+10 EXP) → Level " << it->level
                 << " (" << it->stageName() << ")\n";
            if (oldLevel < it->level && it->level == 5) {
                cout << "→ Đã đạt Cơ bản!\n";
            }
        }
        saveToFile();
        return true;
    }
    void autoLearn(const string& eng) {
        auto it = find_if(words.begin(), words.end(),
                          [&](const Word& w) { return w.english == eng; });
        if (it != words.end()) {
            if (it->getStage() == LearnStage::Mastered) return;
            if (!it->learned) {
                it->learned = true;
                it->exp += 10;
                cout << "Đã học: " << eng << " (+10 EXP)\n";
            } else {
                it->exp += 5;
                cout << "Ôn lại: " << eng << " (+5 EXP)\n";
            }
            it->isNew = false;
            int oldLevel = it->level;
            while (it->level < 10 && it->exp >= it->expToNextLevel()) {
                it->exp -= it->expToNextLevel();
                it->level++;
                if (it->level == 10) {
                    it->isMastered = true;
                    cout << "THÔNG THÁO: " << eng << "!\n";
                    saveMastered();
                    break;
                }
            }
            if (oldLevel < it->level) {
                cout << "→ Lên Level " << it->level << " (" << it->stageName() << ")\n";
            }
        } else {
            Word temp(eng, "", "", "", "");
            temp.learned = true;
            temp.exp = 10;
            temp.isNew = false;
            temp.level = 1;
            words.push_back(temp);
            Dictionary::add(temp);
            cout << "Tự động ghi nhận: " << eng << " là đã học (+10 EXP, Level 1)\n";
        }
        saveToFile();
    }
    bool remove(const string& eng) {
        auto it = remove_if(words.begin(), words.end(),
                            [&](const Word& w) { return w.english == eng; });
        if (it == words.end()) return false;
        words.erase(it, words.end());
        clear(rootEng); clear(rootVie); rootEng = rootVie = nullptr;
        for (const auto& w : words) Dictionary::add(w);
        saveToFile();
        return true;
    }
    void display() const {
        cout << "\n===== TỪ CỦA " << username << " =====\n";
        if (words.empty()) { cout << "(Trống)\n"; return; }
        vector<Word> notLearned, basic, advanced, mastered;
        for (const auto& w : words) {
            switch (w.getStage()) {
                case LearnStage::NotLearned: notLearned.push_back(w); break;
                case LearnStage::Basic: basic.push_back(w); break;
                case LearnStage::Advanced: advanced.push_back(w); break;
                case LearnStage::Mastered: mastered.push_back(w); break;
            }
        }
        auto printGroup = [](const string& title, const vector<Word>& list) {
            if (list.empty()) return;
            cout << "\n--- " << title << " (" << list.size() << ") ---\n";
            for (const auto& w : list) {
                int next = w.expToNextLevel();
                cout << w.english << " - " << w.vietnamese
                     << " | L" << w.level << " (" << w.stageName() << ")"
                     << " | EXP: " << w.exp << (next > 0 ? "/" + to_string(next) : "")
                     << " | " << w.topic << "\n";
            }
        };
        printGroup("CHƯA HỌC", notLearned);
        printGroup("CƠ BẢN (L1-5)", basic);
        printGroup("NÂNG CAO (L6-9)", advanced);
        printGroup("THÔNG THÁO (L10)", mastered);
    }
    bool contains(const string& eng) const {
        return any_of(words.begin(), words.end(),
                      [&](const Word& w) { return w.english == eng; });
    }
    const vector<Word>& getWords() const { return words; }
    int countMasteredInTopic(const string& topic) const {
        return count_if(words.begin(), words.end(),
            [&](const Word& w) { return w.getStage() == LearnStage::Mastered && w.topic == topic; });
    }
    int getTotalEXP() const {
        int total = 0;
        for (const auto& w : words) {
            total += w.exp;
            if (w.level > 0) total += (w.level - 1) * 100;
        }
        return total;
    }
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
        cout << "\n=== ĐĂNG KÝ ===\n";
        username = safeInput("Tên: ");
        password = safeInput("Mật khẩu: ");
        if (username.empty() || password.empty()) return cout << "Không để trống!\n", false;
        ifstream in(accountFile);
        string u, p; while (in >> u >> p) if (u == username) return cout << "Tên đã tồn tại!\n", false;
        in.close();
        ofstream out(accountFile, ios::app);
        if (out) { out << username << " " << password << "\n"; cout << "Đăng ký thành công!\n"; return true; }
        return cout << "Lỗi file!\n", false;
    }
    bool login() {
        cout << "\n=== ĐĂNG NHẬP ===\n";
        username = safeInput("Tên: ");
        password = safeInput("Mật khẩu: ");
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
                cout << " " << i+1 << ". " << q.options[i] << "\n";
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
            if (!g.baseVerb.empty()) cout << " (gợi ý: " << g.baseVerb << " → đổi thì phù hợp)";
            cout << "\n→ ";
            string ans = safeInput();
            if (compareAnswer(ans, g.answer)) { cout << "Đúng!\n"; score++; }
            else {
                cout << "Sai! Đáp án đúng: \"" << g.answer << "\"\n";
                if (!g.hint.empty()) cout << " Gợi ý: " << g.hint << "\n";
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
                    cout << " " << j + 1 << ". " << quiz[i].options[j] << "\n";
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
    mt19937 rng;
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
    string suggestNextTopic() {
        cout << "\nGợi ý chủ đề tiếp theo: ";
        if (topicList.empty()) {
            cout << "Chưa có chủ đề.\n";
            return "";
        }
        uniform_int_distribution<> dis(0, topicList.size() - 1);
        string suggested = topicList[dis(rng)];
        cout << suggested << "\n";
        return suggested;
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
            string c = safeInput("\nPhát âm? (1: Có / 0: Không): ");
            if (trim(c) == "1") {
                speak(w.english);
                string r = safeInput("Lặp lại? (y/n): ");
                if (!trim(r).empty() && tolower(trim(r)[0]) == 'y') speakRepeat(w.english);
            }
            if (account.isLogin()) {
                if (account.isWordSaved(w.english)) {
                    account.getSavedWords()->autoLearn(w.english);
                } else {
                    account.getSavedWords()->addOrUpdateInteractive(w);
                }
            }
            cout << "────────────────────\n";
        }
        safeInput("\nNhấn Enter để quay lại danh sách chủ đề...");
    }
    void reviewVocabulary() {
        if (allWords.empty()) {
            cout << "Chưa có từ vựng để ôn!\n";
            safeInput("Nhấn Enter..."); return;
        }
        vector<Word> candidateList;
        if (account.isLogin() && account.getSavedWords()) {
            for (const auto& w : account.getSavedWords()->getWords()) {
                if (w.learned && !w.isMastered) {
                    auto it = find_if(allWords.begin(), allWords.end(),
                        [&w](const Word& ww) { return ww.english == w.english; });
                    if (it != allWords.end()) candidateList.push_back(*it);
                }
            }
        } else {
            candidateList = allWords;
        }
        if (candidateList.empty()) {
            cout << "Bạn chưa học từ nào!\n";
            safeInput("Nhấn Enter..."); return;
        }
        cout << "\n=== ÔN TẬP TỪ VỰNG ===\n";
        cout << "1. Ôn từ mới (ưu tiên level thấp)\n";
        cout << "2. Ôn ngẫu nhiên\n";
        int modeChoice = getChoice(2, "Chọn kiểu ôn: ");
        if (modeChoice == 0) return;
        vector<Word> reviewList = candidateList;
        if (modeChoice == 1) {
            sort(reviewList.begin(), reviewList.end(), [&](const Word& a, const Word& b) {
                int la = 0, lb = 0;
                auto wa = account.getSavedWords()->getWords();
                auto fa = find_if(wa.begin(), wa.end(), [&](const Word& w) { return w.english == a.english; });
                auto fb = find_if(wa.begin(), wa.end(), [&](const Word& w) { return w.english == b.english; });
                if (fa != wa.end()) la = fa->level;
                if (fb != wa.end()) lb = fb->level;
                return la < lb;
            });
        } else {
            shuffle(reviewList.begin(), reviewList.end(), rng);
        }
        reviewList.resize(min(10, (int)reviewList.size()));
        cout << "\nSố từ: " << reviewList.size() << "\n";
        cout << "1. Flashcard (Anh to Việt)\n";
        cout << "2. Trắc nghiệm (4 đáp án)\n";
        cout << "3. Điền vào chỗ trống\n";
        cout << "4. Nghe và viết chính tả\n";
        cout << "0. Thoát\n";
        int mode = getChoice(4, "Chọn kiểu ôn: ");
        if (mode == 0) return;
        int score = 0;
        for (size_t i = 0; i < reviewList.size(); ++i) {
            const Word& w = reviewList[i];
            bool correct = false;
            if (mode == 1) {
                cout << "\n[" << i+1 << "/" << reviewList.size() << "] Từ: " << w.english << "\n";
                safeInput("→ Nhấn Enter để xem nghĩa...");
                cout << "Nghĩa: " << w.vietnamese << "\n";
                cout << "IPA: " << w.ipaText << "\n";
                cout << "Ví dụ: " << w.example << "\n";
                speak(w.english);
                correct = true;
            }
            else if (mode == 2) {
                vector<string> options = { w.vietnamese };
                vector<Word> others = allWords;
                others.erase(remove_if(others.begin(), others.end(),
                    [&w](const Word& ww) { return ww.english == w.english; }), others.end());
                shuffle(others.begin(), others.end(), rng);
                for (int j = 0; j < 3 && j < (int)others.size(); ++j)
                    options.push_back(others[j].vietnamese);
                shuffle(options.begin(), options.end(), rng);
                cout << "\n[" << i+1 << "/" << reviewList.size() << "] Nghĩa của \"" << w.english << "\" là?\n";
                for (size_t j = 0; j < options.size(); ++j)
                    cout << " " << j+1 << ". " << options[j] << "\n";
                int ans = 0;
                while (true) {
                    string in = safeInput("Chọn (1-4): ");
                    if (isNumber(in)) { ans = stoi(in); if (ans >= 1 && ans <= 4) break; }
                }
                int correctIdx = find(options.begin(), options.end(), w.vietnamese) - options.begin() + 1;
                if (ans == correctIdx) { cout << "Đúng!\n"; correct = true; score++; }
                else cout << "Sai! Đáp án: " << correctIdx << ". " << w.vietnamese << "\n";
            }
            else if (mode == 3) {
                string sentence = w.example;
                size_t pos = sentence.find(w.english);
                if (pos != string::npos) {
                    string blank = string(w.english.size(), '_');
                    sentence.replace(pos, w.english.size(), blank);
                } else {
                    sentence = "I _____ every day.";
                }
                cout << "\n[" << i+1 << "/" << reviewList.size() << "] Điền từ: " << sentence << "\n";
                string ans = safeInput("→ ");
                if (normalize(ans) == normalize(w.english)) {
                    cout << "Đúng!\n"; correct = true; score++;
                } else {
                    cout << "Sai! Đáp án: " << w.english << "\n";
                }
            }
            else if (mode == 4) {
                cout << "\n[" << i+1 << "/" << reviewList.size() << "] Nghe và viết từ:\n";
                speak(w.english);
                std::this_thread::sleep_for(chrono::milliseconds(500));
                speak(w.english);
                string ans = safeInput("→ ");
                if (normalize(ans) == normalize(w.english)) {
                    cout << "Đúng!\n"; correct = true; score++;
                } else {
                    cout << "Sai! Từ là: " << w.english << "\n";
                }
            }
            if (correct && account.isLogin()) {
                account.getSavedWords()->autoLearn(w.english);
            }
        }
        if (mode >= 2 && mode <= 4) {
            cout << "\nKết quả: " << score << "/" << reviewList.size() << "\n";
        }
        safeInput("Nhấn Enter để tiếp tục...");
    }
    void showSearchHistory() {
        auto history = loadHistory(10);
        cout << "\n=== LỊCH SỬ TRA TỪ (10 từ gần nhất) ===\n";
        if (history.empty()) {
            cout << "(Chưa tra từ nào)\n";
        } else {
            for (int i = history.size() - 1; i >= 0; --i) {
                cout << (history.size() - i) << ". " << history[i] << "\n";
            }
        }
        safeInput("\nNhấn Enter để quay lại...");
    }
    void deleteSavedWord() {
        string eng = safeInput("Nhập từ tiếng Anh muốn xóa: ");
        eng = trim(eng);
        if (eng.empty()) return;
        if (account.getSavedWords()->remove(eng)) {
            cout << "Đã xóa: " << eng << "\n";
        } else {
            cout << "Không tìm thấy từ \"" << eng << "\" trong danh sách.\n";
        }
        buildData();
    }
public:
    VocabularyApp() : rng(chrono::steady_clock::now().time_since_epoch().count()) {}
    vector<string> parseCSVLine(const string& line) {
        vector<string> parts;
        string current;
        bool inQuote = false;
        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (c == '"') {
                if (inQuote && i + 1 < line.size() && line[i + 1] == '"') {
                    current += '"'; i++;
                } else {
                    inQuote = !inQuote;
                }
            } else if (c == ',' && !inQuote) {
                parts.push_back(trim(current));
                current.clear();
            } else {
                current += c;
            }
        }
        if (!current.empty()) parts.push_back(trim(current));
        return parts;
    }
    void buildData() {
        allWords.clear();
        topicMap.clear();
        topicList.clear();
        globalDict = Dictionary();
        fs::path dataPath = fs::current_path() / "Data";
        if (!fs::exists(dataPath)) {
            cout << "Không tìm thấy thư mục 'Data' to đang tạo...\n";
            fs::create_directory(dataPath);
            ofstream sample(dataPath / "animals.csv", ios::binary);
            sample << "\xEF\xBB\xBF"; // BOM UTF-8
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
                auto parts = parseCSVLine(line);
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
                int mastered = account.isLogin() ? account.getSavedWords()->countMasteredInTopic(topicList[i]) : 0;
                double percent = topic.size() > 0 ? (mastered * 100.0 / topic.size()) : 0.0;
                cout << i + 1 << ". " << topicList[i]
                     << " (" << mastered << "/" << topic.size() << " thông thạo - "
                     << fixed << setprecision(0) << percent << "%)\n";
            }
            int suggestOption = topicList.size() + 1;
            cout << suggestOption << ". Đề xuất 1 chủ đề\n";
            int maxChoice = suggestOption;
            int choice = getChoice(maxChoice, "\nChọn (0 để thoát): ");
            if (choice == 0) return;
            if (choice == suggestOption) {
                string suggested = suggestNextTopic();
                if (!suggested.empty()) {
                    cout << "Nhấn 1 để vào ngay chủ đề \"" << suggested << "\"!\n";
                    cout << "Nhấn Enter để quay lại.\n";
                    string go = safeInput("→ ");
                    if (trim(go) == "1") {
                        auto it = find(topicList.begin(), topicList.end(), suggested);
                        if (it != topicList.end()) {
                            viewTopicByIndex(it - topicList.begin() + 1);
                        }
                    }
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
            string c = safeInput("\nPhát âm? (1: Có / 0: Không): ");
            if (trim(c) == "1") {
                speak(w->english);
                string r = safeInput("Lặp lại? (y/n): ");
                if (!trim(r).empty() && tolower(trim(r)[0]) == 'y')
                    speakRepeat(w->english);
            }
            if (account.isLogin()) {
                if (account.isWordSaved(w->english)) {
                    account.getSavedWords()->autoLearn(w->english);
                } else {
                    account.getSavedWords()->addOrUpdateInteractive(*w);
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
        int learned = 0, basic = 0, advanced = 0, mastered = 0;
        map<string, int> topicMastered;
        if (account.isLogin() && account.getSavedWords()) {
            for (const auto& w : account.getSavedWords()->getWords()) {
                if (w.learned) learned++;
                if (w.level <= 5 && w.level > 0) basic++;
                else if (w.level < 10) advanced++;
                else if (w.level >= 10) mastered++;
                if (w.isMastered) topicMastered[w.topic]++;
            }
        }
        cout << fixed << setprecision(1);
        cout << "\n=== TIẾN ĐỘ HỌC TẬP CHI TIẾT ===\n";
        cout << "Tổng từ trong từ điển: " << total << "\n";
        cout << "Đã học: " << learned << " từ\n";
        cout << " • Cơ bản (L1-5): " << basic << "\n";
        cout << " • Nâng cao (L6-9): " << advanced << "\n";
        cout << " • Thông thạo (L10): " << mastered << " ★\n";
        if (account.isLogin()) {
            cout << "Tổng EXP: " << account.getSavedWords()->getTotalEXP() << " điểm\n";
        }
        cout << "\n--- TIẾN ĐỘ THEO CHỦ ĐỀ (chỉ tính từ thông thạo) ---\n";
        for (const auto& t : topicList) {
            int m = account.isLogin() ? account.getSavedWords()->countMasteredInTopic(t) : 0;
            int totalInTopic = topicMap.at(t).size();
            double percent = totalInTopic > 0 ? (m * 100.0 / totalInTopic) : 0.0;
            cout << t << ": " << m << "/" << totalInTopic << " thông thạo (" << percent << "%)\n";
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

        int choice;
        do {
            cout << "\n════════════════════════════════════\n";
            cout << " TỪ ĐIỂN ANH - VIỆT PRO+ \n";
            cout << "════════════════════════════════════\n";

            if (account.isLogin()) {
                cout << "Người dùng: " << account.getUsername()
                     << " | EXP: " << account.getSavedWords()->getTotalEXP() << " ★\n";
            }

            cout << "1. Xem từ theo chủ đề\n";
            cout << "2. Tra từ (Anh to Việt)\n";

            int maxChoice;
            if (!account.isLogin()) {
                cout << "3. Đăng nhập\n";
                cout << "4. Đăng ký\n";
                cout << "0. Thoát\n";
                maxChoice = 4;
            } else {
                cout << "3. Xem tiến độ học\n";
                cout << "4. Từ đã lưu (cá nhân)\n";
                cout << "5. Ôn tập từ vựng\n";
                cout << "6. Học 12 thì tiếng Anh\n";
                cout << "7. Ôn tập Grammar tổng hợp\n";
                cout << "8. Lịch sử tra từ\n";
                cout << "9. Xóa từ đã lưu\n";
                cout << "10. Đăng xuất\n";
                cout << "0. Thoát\n";
                maxChoice = 10;
            }

            cout << "────────────────────────────────────\n";
            choice = getChoice(maxChoice, "Chọn (0-" + to_string(maxChoice) + "): ");
            if (choice == 0) break;

            if (!account.isLogin()) {
                switch (choice) {
                    case 1: viewTopic(); break;
                    case 2: searchWord(); break;
                    case 3: account.login(); buildData(); break;
                    case 4: account.signUp(); buildData(); break;
                }
            } else {
                switch (choice) {
                    case 1: viewTopic(); break;
                    case 2: searchWord(); break;
                    case 3: showProgress(); break;
                    case 4: account.getSavedWords()->display(); break;
                    case 5: reviewVocabulary(); break;
                    case 6: grammar.learnGrammars(); break;
                    case 7: grammar.practiceTest(); break;
                    case 8: showSearchHistory(); break;
                    case 9: deleteSavedWord(); break;
                    case 10: account.logout(); buildData(); break;
                }
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
    VocabularyApp app;
    app.menu();
    return 0;
}
