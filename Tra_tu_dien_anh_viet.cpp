#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
using namespace std;

// ===============================
// Enum IPA
// ===============================
enum IPA {
    IPA_ae,
    // có thể bổ sung thêm
};

// ===============================
// Struct Word: từ vựng chung
// ===============================
struct Word {
    string engLish;
    string vietNam;
    IPA ipa;
    string example;
};

// ===============================
// Dictionary: chứa toàn bộ từ vựng
// ===============================
class Dictionary {
protected:
    vector<Word> words;

    string getIPAString(IPA ipa) const {
        switch (ipa) {
            case IPA_ae: return "/æ/";
            default: return "(unknown)";
        }
    }

public:
    void addWord(const Word& w) { words.push_back(w); }

    const vector<Word>& getWords() const { return words; }

    void showAllWords() const {
        cout << "===== TỪ ĐIỂN CHUNG =====\n";
        for (auto& w : words) {
            cout << w.engLish << " - " << w.vietNam
                 << " | IPA: " << getIPAString(w.ipa)
                 << " | Example: " << w.example << "\n";
        }
    }
};

// ===============================
// Topic: gom nhóm từ theo chủ đề
// ===============================
class Topic : public Dictionary {
protected:
    string name;
public:
    Topic(const string& n = "") : name(n) {}
    string getName() const { return name; }
    void setName(const string& n) { name = n; }

    void showTopicWords() const {
        cout << "===== Chủ đề: " << name << " =====\n";
        showAllWords();
    }
};

// ===============================
// Account: lưu từ vựng cá nhân + đăng ký/đăng nhập bằng file
// ===============================
class Account {
private:
    string username;
    string password;
    bool isLoggedIn = false;
    vector<Word> savedWords; // kho từ cá nhân
    const string filename = "accounts.txt"; // file lưu tài khoản

    string getIPAString(IPA ipa) const {
        switch (ipa) {
            case IPA_ae: return "/æ/";
            default: return "(unknown)";
        }
    }

public:
    Account(const string& u="", const string& p="") : username(u), password(p) {}

    // Đăng ký -> lưu username/password vào file
    bool signUp() {
        cout << "\n=== ĐĂNG KÝ ===\n";
        cout << "Tên đăng nhập: ";
        getline(cin, username);
        cout << "Mật khẩu: ";
        getline(cin, password);

        // kiểm tra trùng
        ifstream inFile(filename);
        string u, p;
        while (inFile >> u >> p) {
            if (u == username) {
                cout << "❌ Tên đăng nhập đã tồn tại!\n";
                return false;
            }
        }
        inFile.close();

        // ghi thêm vào file
        ofstream outFile(filename, ios::app);
        if (outFile) {
            outFile << username << " " << password << "\n";
            cout << "✅ Đăng ký thành công!\n";
            return true;
        } else {
            cout << "❌ Không thể ghi vào file.\n";
            return false;
        }
    }

    // Đăng nhập -> đọc file và kiểm tra
    bool login() {
        cout << "\n=== ĐĂNG NHẬP ===\n";
        cout << "Tên đăng nhập: ";
        getline(cin, username);
        cout << "Mật khẩu: ";
        getline(cin, password);

        ifstream inFile(filename);
        if (!inFile) {
            cout << "❌ Chưa có tài khoản nào.\n";
            return false;
        }

        string u, p;
        while (inFile >> u >> p) {
            if (u == username && p == password) {
                cout << "✅ Đăng nhập thành công! Xin chào, " << username << "!\n";
                isLoggedIn = true;
                return true;
            }
        }

        cout << "❌ Sai tên đăng nhập hoặc mật khẩu.\n";
        return false;
    }

    // Đăng xuất
    void logout() {
        if (isLoggedIn) {
            isLoggedIn = false;
            cout << "👋 Đăng xuất thành công!\n";
        } else {
            cout << "Bạn chưa đăng nhập.\n";
        }
    }

    // Lưu từ vựng từ một Topic
    void saveWord(const Word& w, const Topic& t) {
        if (!isLoggedIn) {
            cout << "⚠️ Bạn cần đăng nhập trước.\n";
            return;
        }
        savedWords.push_back(w);
        cout << "Đã lưu từ: " << w.engLish 
             << " (thuộc chủ đề: " << t.getName() << ")\n";
    }

    void showSavedWords() const {
        if (!isLoggedIn) {
            cout << "⚠️ Bạn cần đăng nhập để xem từ vựng.\n";
            return;
        }
        cout << "===== Từ vựng đã lưu của " << username << " =====\n";
        for (auto& w : savedWords) {
            cout << w.engLish << " - " << w.vietNam
                 << " | IPA: " << getIPAString(w.ipa)
                 << " | Example: " << w.example << "\n";
        }
    }
};

// ===============================
// Demo main
// ===============================
int main() {
    Topic fruits("Fruits");
    fruits.addWord({"apple", "quả táo", IPA_ae, "I eat an apple every day"});
    fruits.addWord({"banana", "quả chuối", IPA_ae, "Bananas are yellow"});

    fruits.showTopicWords();

    Account acc;

    if (acc.signUp()) {          // đăng ký
        if (acc.login()) {       // đăng nhập
            acc.saveWord(fruits.getWords()[0], fruits);
            acc.saveWord(fruits.getWords()[1], fruits);

            acc.showSavedWords();
            acc.logout();
        }
    }

    return 0;
}