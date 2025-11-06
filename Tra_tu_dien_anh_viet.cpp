#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include <ctime>
#include <cctype>
using namespace std;

// ===============================
// Enum IPA
// ===============================
enum IPA { IPA_ae };

// ===============================
// Struct Word
// ===============================
struct Word {
    string engLish, vietNam, example;
    IPA ipa;
    int exp = 0;
    bool isNew = true;

    // Constructor đầy đủ để tránh lỗi khởi tạo
    Word(const string& e = "", const string& v = "", IPA i = IPA_ae,
         const string& ex = "", int xp = 0, bool n = true)
        : engLish(e), vietNam(v), ipa(i), example(ex), exp(xp), isNew(n) {}
};

// ===============================
// Class Dictionary
// ===============================
class Dictionary {
private:
    vector<Word> words;
public:
    string getIPAString(IPA ipa) const {
        return ipa == IPA_ae ? "/æ/" : "(unknown)";
    }
    void addWord(const Word& w) { words.push_back(w); }
    const vector<Word>& getWords() const { return words; }
    vector<Word>& getWords() { return words; }
    void showAllWords(const string& title = "TỪ ĐIỂN") const {
        cout << "===== " << title << " =====\n";
        for (const auto& w : words)
            cout << w.engLish << " - " << w.vietNam
                 << " | IPA: " << getIPAString(w.ipa)
                 << " | Example: " << w.example << "\n";
        if (words.empty()) cout << "(Trống)\n";
    }
};

// ===============================
// Class Topic
// ===============================
class Topic : public Dictionary {
private:
    string name;
public:
    Topic(const string& n = "") : name(n) {}
    string getName() const { return name; }
    void showTopicWords() const { showAllWords("Chủ đề: " + name); }
};

// ===============================
// Class Account
// ===============================
class Account : public Topic {
private:
    string username, password;
    bool isLoggedIn = false;
    vector<Word> savedWords;
    const string accountFile = "accounts.txt";
    string getUserWordFile() const { return username + "_words.txt"; }

protected:
    void saveWordsToFile() const {
        const char D = '|';
        ofstream f(getUserWordFile());
        if (!f) return;
        for (const auto& w : savedWords)
            f << w.engLish << D << w.vietNam << D
              << static_cast<int>(w.ipa) << D << w.example << D
              << w.exp << D << (w.isNew ? "1" : "0") << "\n";
    }

    void loadSavedWords() {
        const char D = '|';
        savedWords.clear();
        ifstream f(getUserWordFile());
        if (!f) return;
        string line;
        while (getline(f, line)) {
            size_t p[6]; p[0] = line.find(D);
            for (int i = 1; i < 6; ++i) p[i] = line.find(D, p[i-1] + 1);
            if (p[5] == string::npos) continue;
            Word w;
            w.engLish = line.substr(0, p[0]);
            w.vietNam = line.substr(p[0]+1, p[1]-p[0]-1);
            w.ipa = static_cast<IPA>(stoi(line.substr(p[1]+1, p[2]-p[1]-1)));
            w.example = line.substr(p[2]+1, p[3]-p[2]-1);
            w.exp = stoi(line.substr(p[3]+1, p[4]-p[3]-1));
            w.isNew = (line.substr(p[4]+1, p[5]-p[4]-1) == "1");
            savedWords.push_back(w);
        }
    }

public:
    Account(const string& topicName = "") : Topic(topicName) {}

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
        string u, p; while (in >> u >> p)
            if (u == username && p == password) { isLoggedIn = true; loadSavedWords(); cout << "Chào, " << username << "!\n"; return true; }
        return cout << "Sai thông tin!\n", false;
    }

    void logout() {
        if (isLoggedIn) { saveWordsToFile(); isLoggedIn = false; savedWords.clear(); cout << "Đăng xuất!\n"; }
        else cout << "Chưa đăng nhập.\n";
    }

    void saveWord(const Word& w, const string& topicName) {
        if (!isLoggedIn) return cout << "Đăng nhập trước!\n", void();
        if (find_if(savedWords.begin(), savedWords.end(), [&](const auto& sw){ return sw.engLish == w.engLish; }) != savedWords.end())
            return cout << "Từ đã lưu!\n", void();
        savedWords.push_back(w);
        cout << "Đã lưu: " << w.engLish << " (" << topicName << ")\n";
    }

    void deleteSavedWord(const string& eng) {
        if (!isLoggedIn) return cout << "Cần đăng nhập!\n", void();
        auto it = remove_if(savedWords.begin(), savedWords.end(), [&](const auto& w){ return w.engLish == eng; });
        if (it != savedWords.end()) { savedWords.erase(it, savedWords.end()); cout << "Đã xóa!\n"; }
        else cout << "Không tìm thấy!\n";
    }

    void showSavedWords() const {
        if (!isLoggedIn) return cout << "Cần đăng nhập!\n", void();
        cout << "===== Từ của " << username << " =====\n";
        if (savedWords.empty()) return cout << "(Trống)\n", void();
        for (const auto& w : savedWords)
            cout << w.engLish << " - " << w.vietNam
                 << " | IPA: " << getIPAString(w.ipa)
                 << " | EXP: " << w.exp << " | " << (w.isNew ? "MỚI" : "CŨ") << "\n";
    }

    bool isLogin() const { return isLoggedIn; }
    vector<Word>& getSavedWords() { return savedWords; }
    const vector<Word>& getSavedWords() const { return savedWords; }
};

// ===============================
// Class Review
// ===============================
class Review : public Account {
    mt19937 rng{ static_cast<unsigned>(time(nullptr)) };
    vector<size_t> newIdx, oldIdx, allIdx; // Dùng chỉ số thay vì con trỏ

    vector<string> generateOptions(const Word* c, int t) {
        vector<string> opts = { t == 0 ? c->engLish : c->vietNam };
        vector<const Word*> pool;
        for (size_t i = 0; i < allIdx.size(); ++i)
            if (&getSavedWords()[allIdx[i]] != c) pool.push_back(&getSavedWords()[allIdx[i]]);
        shuffle(pool.begin(), pool.end(), rng);
        for (int i = 0; i < 3 && i < pool.size(); ++i)
            opts.push_back(t == 0 ? pool[i]->engLish : pool[i]->vietNam);
        shuffle(opts.begin(), opts.end(), rng);
        return opts;
    }

    string maskExample(const string& ex, const string& w) {
        string s = ex; size_t p = 0;
        while ((p = s.find(w, p)) != string::npos)
            s.replace(p, w.length(), string(w.length(), '.')), p += w.length();
        return s;
    }

    void updateAllWords() {
        newIdx.clear(); oldIdx.clear(); allIdx.clear();
        for (size_t i = 0; i < getSavedWords().size(); ++i) {
            allIdx.push_back(i);
            if (getSavedWords()[i].isNew) newIdx.push_back(i);
            else oldIdx.push_back(i);
        }
    }

public:
    Review(const string& topicName = "") : Account(topicName) {}

    void addNewWord(const Word& w, const string& topic) {
        if (!isLogin()) return cout << "Đăng nhập trước!\n", void();
        if (find_if(getSavedWords().begin(), getSavedWords().end(), [&](const auto& sw){ return sw.engLish == w.engLish; }) != getSavedWords().end())
            return cout << "Từ đã lưu!\n", void();
        getSavedWords().push_back(w);
        getSavedWords().back().isNew = true;
        getSavedWords().back().exp = 0;
        cout << "Đã lưu: " << w.engLish << " (" << topic << ")\n";
    }

    void startReview(int mode) {
        if (!isLogin()) return cout << "Đăng nhập trước!\n", void();
        updateAllWords();

        vector<size_t> selected;
        if (mode == 0 && !newIdx.empty()) {
            // Lấy 10 từ mới gần đây nhất
            int n = min(10, (int)newIdx.size());
            for (int i = 0; i < n; ++i)
                selected.push_back(newIdx[newIdx.size() - 1 - i]);
        } else if (mode == 1 && !oldIdx.empty()) {
            // Ngẫu nhiên 10 từ cũ
            vector<size_t> pool = oldIdx;
            shuffle(pool.begin(), pool.end(), rng);
            int n = min(10, (int)pool.size());
            selected.assign(pool.begin(), pool.begin() + n);
        } else if (!allIdx.empty()) {
            // Ngẫu nhiên từ tất cả
            vector<size_t> pool = allIdx;
            shuffle(pool.begin(), pool.end(), rng);
            int n = min(10, (int)pool.size());
            selected.assign(pool.begin(), pool.begin() + n);
        } else return cout << "Chưa có từ!\n", void();

        cout << "\n=== " << (mode == 0 ? "ÔN TỪ MỚI" : mode == 1 ? "ÔN TỪ CŨ" : "ÔN NGẪU NHIÊN") << " ===\n";
        int correct = 0;

        for (size_t i = 0; i < selected.size(); ++i) {
            size_t idx = selected[i];
            Word* w = &getSavedWords()[idx];
            int type = rng() % 3;
            string q = type == 0 ? w->vietNam : type == 1 ? w->engLish : maskExample(w->example, w->engLish);
            auto opts = generateOptions(w, type);
            int ansIdx = -1;
            for (int j = 0; j < 4; ++j)
                if (opts[j] == (type == 0 ? w->engLish : w->vietNam)) ansIdx = j;

            cout << "\nCâu " << i+1 << ": " << q << "\n";
            for (int j = 0; j < 4; ++j) cout << " " << (char)('A'+j) << ". " << opts[j] << "\n";
            char ch; cout << "Chọn: "; cin >> ch; cin.ignore();
            int choice = toupper(ch) - 'A';

            if (choice == ansIdx) { cout << "Đúng! +1 exp\n"; w->exp++; correct++; }
            else { cout << "Sai! -2 exp (Đáp án: " << (char)('A'+ansIdx) << ")\n"; w->exp = max(0, w->exp - 2); }

            if (w->exp >= 10 && w->isNew) {
                w->isNew = false;
                cout << "→ '" << w->engLish << "' đã thành TỪ CŨ!\n";
            }
        }
        cout << "\nKết thúc! Đúng: " << correct << "/" << selected.size() << "\n";
        saveWordsToFile();
    }
};

// ===============================
// MAIN
// ===============================
int main() {
    Review app("Fruits");
    app.addWord(Word("apple", "quả táo", IPA_ae, "I eat an apple every day."));
    app.addWord(Word("banana", "quả chuối", IPA_ae, "Bananas are yellow."));
    app.addWord(Word("grape", "quả nho", IPA_ae, "Grapes are sweet."));

    int choice;
    while (true) {
        cout << "\n===== MENU =====\n"
             << "1. Đăng ký\n2. Đăng nhập\n3. Xem chủ đề\n4. Lưu từ\n"
             << "5. Xem từ lưu\n6. Xóa từ\n7. Ôn tập\n8. Đăng xuất\n0. Thoát\n"
             << "Chọn: ";
        cin >> choice; cin.ignore();
        switch (choice) {
            case 1: app.signUp(); break;
            case 2: app.login(); break;
            case 3: app.showTopicWords(); break;
            case 4: {
                if (!app.isLogin()) { cout << "Đăng nhập trước!\n"; break; }
                app.showTopicWords();
                cout << "Số thứ tự: "; int i; cin >> i; cin.ignore();
                if (i >= 0 && i < (int)app.getWords().size())
                    app.addNewWord(app.getWords()[i], app.getName());
                break;
            }
            case 5: app.showSavedWords(); break;
            case 6: {
                if (!app.isLogin()) { cout << "Đăng nhập trước!\n"; break; }
                cout << "Từ xóa: "; string w; getline(cin, w);
                app.deleteSavedWord(w); break;
            }
            case 7: {
                if (!app.isLogin()) { cout << "Đăng nhập trước!\n"; break; }
                cout << "1. MỚI 2. CŨ 3. NGẪU NHIÊN\nChọn: "; int m; cin >> m; cin.ignore();
                if (m >= 1 && m <= 3) app.startReview(m-1);
                break;
            }
            case 8: app.logout(); break;
            case 0: app.logout(); cout << "Tạm biệt!\n"; return 0;
            default: cout << "Sai lựa chọn!\n";
        }
    }
}