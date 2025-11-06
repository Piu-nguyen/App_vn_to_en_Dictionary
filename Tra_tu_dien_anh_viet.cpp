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
// ?? MAIN - DEMO CHỈ ĐỂ TEST ACCOUNT
// ===============================
int main() {
    Topic fruits("Fruits");
    fruits.addWord({"apple", "quả táo", IPA_ae, "I eat an apple every day."});
    fruits.addWord({"banana", "quả chuối", IPA_ae, "Bananas are yellow."});

    Account acc;
    int choice;
    bool running = true;

    while (running) {
        cout << "\n===== MENU DEMO ACCOUNT =====\n";
        cout << "1. Đăng ký\n";
        cout << "2. Đăng nhập\n";
        cout << "3. Xem chủ đề Fruits\n";
        cout << "4. Lưu từ (chọn số thứ tự)\n";
        cout << "5. Xem từ đã lưu\n";
        cout << "6. Xóa từ đã lưu\n";
        cout << "7. Đăng xuất\n";
        cout << "0. Thoát\n";
        cout << "Chọn: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: acc.signUp(); break;
            case 2: acc.login(); break;
            case 3: fruits.showTopicWords(); break;
            case 4: {
                if (!acc.isLogin()) { cout << "Đăng nhập trước!\n"; break; }
                fruits.showTopicWords();
                cout << "Nhập số thứ tự (0-" << fruits.getWords().size() - 1 << "): ";
                int idx; cin >> idx; cin.ignore();
                if (idx >= 0 && idx < fruits.getWords().size()) {
                    acc.saveWord(fruits.getWords()[idx], "Fruits");
                } else cout << "Sai số!\n";
                break;
            }
            case 5: acc.showSavedWords(); break;
            case 6: {
                if (!acc.isLogin()) { cout << "Đăng nhập trước!\n"; break; }
                cout << "Nhập từ tiếng Anh muốn xóa: ";
                string word; getline(cin, word);
                acc.deleteSavedWord(word);
                break;
            }
            case 7: acc.logout(); break;
            case 0: acc.logout(); running = false; cout << "Tạm biệt!\n"; break;
            default: cout << "Lựa chọn không hợp lệ.\n";
        }
    }
    return 0;
}