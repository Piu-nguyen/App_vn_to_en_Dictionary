#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
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
    // 🔒 Thuộc tính
    string username;
    string password;
    bool isLoggedIn = false;
    vector<Word> savedWords;
    const string accountFile = "accounts.txt";

public:
    // ✅ Phương thức công khai
    Account(const string& u = "", const string& p = "") : username(u), password(p) {}

    bool signUp();
    bool login();
    void logout();

    void saveWord(const Word& w, const Topic& t);
    void deleteSavedWord(const string& engWord);
    void showSavedWords() const;

private:
    // 🔧 Phương thức hỗ trợ nội bộ
    string getUserWordFile() const;
    void loadSavedWords();
    void saveWordsToFile() const;
};

// ===============================
// Triển khai phương thức Account
// ===============================
bool Account::signUp() {
    cout << "\n=== ĐĂNG KÝ ===\n";
    cout << "Tên đăng nhập: ";
    getline(cin, username);
    cout << "Mật khẩu: ";
    getline(cin, password);

    ifstream inFile(accountFile);
    string u, p;
    while (inFile >> u >> p) {
        if (u == username) {
            cout << "❌ Tên đăng nhập đã tồn tại!\n";
            return false;
        }
    }
    inFile.close();

    ofstream outFile(accountFile, ios::app);
    if (outFile) {
        outFile << username << " " << password << "\n";
        cout << "✅ Đăng ký thành công!\n";
        return true;
    } else {
        cout << "❌ Không thể ghi vào file.\n";
        return false;
    }
}

bool Account::login() {
    cout << "\n=== ĐĂNG NHẬP ===\n";
    cout << "Tên đăng nhập: ";
    getline(cin, username);
    cout << "Mật khẩu: ";
    getline(cin, password);

    ifstream inFile(accountFile);
    if (!inFile) {
        cout << "❌ Chưa có tài khoản nào.\n";
        return false;
    }

    string u, p;
    while (inFile >> u >> p) {
        if (u == username && p == password) {
            isLoggedIn = true;
            loadSavedWords();
            cout << "✅ Đăng nhập thành công! Xin chào, " << username << "!\n";
            return true;
        }
    }

    cout << "❌ Sai tên đăng nhập hoặc mật khẩu.\n";
    return false;
}

void Account::logout() {
    if (isLoggedIn) {
        saveWordsToFile();
        isLoggedIn = false;
        cout << "👋 Đăng xuất thành công!\n";
    } else {
        cout << "Bạn chưa đăng nhập.\n";
    }
}

void Account::saveWord(const Word& w, const Topic& t) {
    if (!isLoggedIn) {
        cout << "⚠️ Bạn cần đăng nhập trước.\n";
        return;
    }

    for (auto& word : savedWords) {
        if (word.engLish == w.engLish) {
            cout << "⚠️ Từ '" << w.engLish << "' đã được lưu trước đó.\n";
            return;
        }
    }

    savedWords.push_back(w);
    cout << "✅ Đã lưu từ: " << w.engLish << " (chủ đề: " << t.getName() << ")\n";
}

void Account::deleteSavedWord(const string& engWord) {
    if (!isLoggedIn) {
        cout << "⚠️ Bạn cần đăng nhập để xóa từ vựng.\n";
        return;
    }

    auto it = remove_if(savedWords.begin(), savedWords.end(),
                        [&](const Word& w) { return w.engLish == engWord; });

    if (it != savedWords.end()) {
        savedWords.erase(it, savedWords.end());
        cout << "✅ Đã xóa từ: " << engWord << "\n";
    } else {
        cout << "❌ Không tìm thấy từ '" << engWord << "' trong danh sách.\n";
    }
}

void Account::showSavedWords() const {
    if (!isLoggedIn) {
        cout << "⚠️ Bạn cần đăng nhập để xem từ vựng.\n";
        return;
    }

    cout << "===== Từ vựng đã lưu của " << username << " =====\n";
    for (auto& w : savedWords) {
        cout << w.engLish << " - " << w.vietNam
             << " | IPA: /æ/"
             << " | Example: " << w.example << "\n";
    }
}

string Account::getUserWordFile() const {
    return username + "_words.txt";
}

void Account::loadSavedWords() {
    savedWords.clear();
    ifstream inFile(getUserWordFile());
    string line;
    while (getline(inFile, line)) {
        size_t p1 = line.find("|");
        size_t p2 = line.find("|", p1 + 1);
        size_t p3 = line.find("|", p2 + 1);
        if (p1 != string::npos && p2 != string::npos && p3 != string::npos) {
            Word w;
            w.engLish = line.substr(0, p1);
            w.vietNam = line.substr(p1 + 1, p2 - p1 - 1);
            w.ipa = static_cast<IPA>(stoi(line.substr(p2 + 1, p3 - p2 - 1)));
            w.example = line.substr(p3 + 1);
            savedWords.push_back(w);
        }
    }
}

void Account::saveWordsToFile() const {
    ofstream outFile(getUserWordFile());
    for (auto& w : savedWords) {
        outFile << w.engLish << "|" << w.vietNam << "|" << w.ipa << "|" << w.example << "\n";
    }
}
int main() {
    Topic fruits("Fruits");
    fruits.addWord({"apple", "quả táo", IPA_ae, "I eat an apple every day"});
    fruits.addWord({"banana", "quả chuối", IPA_ae, "Bananas are yellow"});
    fruits.addWord({"grape", "quả nho", IPA_ae, "Grapes are sweet"});

    Account acc;
    int choice;
    bool running = true;

    while (running) {
        cout << "\n===== MENU =====\n";
        cout << "1. Đăng ký\n";
        cout << "2. Đăng nhập\n";
        cout << "3. Hiển thị từ theo chủ đề\n";
        cout << "4. Lưu từ vào tài khoản\n";
        cout << "5. Xem từ đã lưu\n";
        cout << "6. Xóa từ đã lưu\n";
        cout << "7. Đăng xuất\n";
        cout << "0. Thoát\n";
        cout << "Chọn: ";
        cin >> choice;
        cin.ignore(); // loại bỏ ký tự '\n' sau khi nhập số

        switch (choice) {
            case 1:
                acc.signUp();
                break;
            case 2:
                acc.login();
                break;
            case 3:
                fruits.showTopicWords();
                break;
            case 4: {
                fruits.showTopicWords();
                cout << "Nhập số thứ tự từ muốn lưu (0-" << fruits.getWords().size() - 1 << "): ";
                int index;
                cin >> index;
                cin.ignore();
                if (index >= 0 && index < fruits.getWords().size()) {
                    acc.saveWord(fruits.getWords()[index], fruits);
                } else {
                    cout << "❌ Vị trí không hợp lệ.\n";
                }
                break;
            }
            case 5:
                acc.showSavedWords();
                break;
            case 6: {
                cout << "Nhập từ tiếng Anh muốn xóa: ";
                string word;
                getline(cin, word);
                acc.deleteSavedWord(word);
                break;
            }
            case 7:
                acc.logout();
                break;
            case 0:
                acc.logout(); // lưu từ trước khi thoát
                running = false;
                cout << "👋 Tạm biệt!\n";
                break;
            default:
                cout << "❌ Lựa chọn không hợp lệ.\n";
        }
    }

    return 0;
}