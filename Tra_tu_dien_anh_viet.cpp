#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include <ctime>
#include <stack>
#include <queue>
using namespace std;

// ===============================
// Enum IPA
// ===============================
enum IPA { IPA_ae };

// ===============================
// Struct Word: từ vựng chung
// ===============================
struct Word {
    string engLish;
    string vietNam;
    IPA ipa;
    string example;
    int reviewCount = 0; // số lần đã ôn
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
class Account : public Topic {
protected:
    string username;
    string password;
    bool isLoggedIn = false;
    vector<Word> savedWords;           // kho từ cá nhân (lịch sử lưu)
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

    // Lưu từ vựng từ một Topic vào kho cá nhân (lịch sử)
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
                 << " | Example: " << w.example
                 << " | Đã ôn: " << w.reviewCount << " lần\n";
        }
    }

    const vector<Word>& getSavedWords() const { return savedWords; }
    bool getLoginStatus() const { return isLoggedIn; }
    string getUsername() const { return username; }
};

// ===============================
// Review: kế thừa Account, quản lý ôn tập
// ===============================
class Review : public Account {
private:
    stack<Word> newWords;   // quản lý từ mới (LIFO)
    queue<Word> oldWords;   // quản lý từ cũ (FIFO)
    mt19937 rng;

    // Tạo 4 đáp án từ pool (1 đúng + 3 sai)
    vector<string> makeOptions(const Word& q, const vector<Word>& pool, bool askEnglish) {
        vector<string> opts;
        if (askEnglish) {
            opts.push_back(q.vietNam);
            for (auto& w : pool) {
                if (w.engLish != q.engLish && opts.size() < 4) opts.push_back(w.vietNam);
            }
        } else {
            opts.push_back(q.engLish);
            for (auto& w : pool) {
                if (w.vietNam != q.vietNam && opts.size() < 4) opts.push_back(w.engLish);
            }
        }
        shuffle(opts.begin(), opts.end(), rng);
        return opts;
    }

    // Đặt câu hỏi trắc nghiệm trên 10 từ trong pool
    void askQuizOnPool(vector<Word>& pool) {
        if (pool.empty()) {
            cout << "⚠️ Không có từ nào để ôn.\n";
            return;
        }
        if (pool.size() > 10) pool.resize(10);

        int score = 0;
        for (int i = 0; i < (int)pool.size(); i++) {
            Word& q = pool[i];
            bool askEnglish = uniform_int_distribution<int>(0,1)(rng) == 1;

            cout << "\nCâu " << (i+1) << ": ";
            if (askEnglish) cout << "Nghĩa của từ \"" << q.engLish << "\" là gì?\n";
            else cout << "Từ tiếng Anh của \"" << q.vietNam << "\" là gì?\n";

            auto options = makeOptions(q, pool, askEnglish);
            for (int j = 0; j < (int)options.size(); j++) {
                cout << j+1 << ". " << options[j] << "\n";
            }

            int ans;
            do {
                cout << "Chọn đáp án (1-4): ";
                if (!(cin >> ans)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); ans = -1; }
            } while (ans < 1 || ans > 4);
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            bool correct = (askEnglish && options[ans-1] == q.vietNam)
                           || (!askEnglish && options[ans-1] == q.engLish);
            if (correct) {
                cout << "✅ Chính xác!\n";
                score++;
            } else {
                cout << "❌ Sai. Đáp án đúng: " << (askEnglish ? q.vietNam : q.engLish) << "\n";
            }
        }

        cout << "\n===== KẾT THÚC ÔN TẬP =====\n";
        cout << "Điểm của bạn: " << score << "/" << pool.size() << "\n";
    }

public:
    using Account::Account;

    Review() : rng((unsigned)time(nullptr)) {}

    // Đẩy từ đã lưu vào stack (mới) khi bắt đầu ôn (hoặc khi lưu mới)
    void primeNewFromSaved() {
        // Đưa tất cả từ đã lưu vào stack như từ mới (nếu stack trống)
        if (newWords.empty()) {
            for (auto it = getSavedWords().rbegin(); it != getSavedWords().rend(); ++it) {
                newWords.push(*it);
            }
        }
    }

    // Thêm từ mới (đồng thời thêm vào savedWords để giữ lịch sử)
    void addNewWord(const Word& w) {
        Topic tmp(getName());
        saveWord(w, tmp); // lưu vào lịch sử
        newWords.push(w); // thêm vào stack từ mới
    }

    // Ôn tập ngẫu nhiên: lấy tối đa 10 từ từ toàn bộ savedWords
    void reviewRandom() {
        if (!getLoginStatus()) {
            cout << "⚠️ Bạn cần đăng nhập để ôn tập.\n";
            return;
        }
        auto words = getSavedWords();
        if (words.empty()) {
            cout << "⚠️ Bạn chưa lưu từ nào để ôn.\n";
            return;
        }
        shuffle(words.begin(), words.end(), rng);
        if (words.size() > 10) words.resize(10);
        cout << "\n===== ÔN TẬP NGẪU NHIÊN =====\n";
        askQuizOnPool(words);
    }

    // Ôn từ mới: lấy 10 từ từ top của stack; nếu không đủ, fallback random
    void reviewNew() {
        if (!getLoginStatus()) {
            cout << "⚠️ Bạn cần đăng nhập để ôn tập.\n";
            return;
        }
        primeNewFromSaved(); // đảm bảo có dữ liệu mới

        if (newWords.size() < 10) {
            cout << "⚠️ Từ mới không đủ 10. Chuyển sang ôn ngẫu nhiên.\n";
            reviewRandom();
            return;
        }

        // Lấy 10 từ từ stack (top-first)
        vector<Word> pool;
        stack<Word> temp = newWords; // copy stack để pop mà không ảnh hưởng
        for (int i = 0; i < 10 && !temp.empty(); i++) {
            pool.push_back(temp.top());
            temp.pop();
        }
        cout << "\n===== ÔN TẬP TỪ MỚI (Stack) =====\n";
        askQuizOnPool(pool);

        // Giả lập tăng reviewCount, tốt nghiệp nếu đủ 10
        // Ta thực hiện bằng cách pop/push lại stack với cập nhật reviewCount
        stack<Word> rebuilt;
        while (!newWords.empty()) {
            Word w = newWords.top(); newWords.pop();
            // nếu w nằm trong pool, tăng reviewCount
            for (auto& q : pool) {
                if (q.engLish == w.engLish && q.vietNam == w.vietNam) {
                    w.reviewCount++;
                    break;
                }
            }
            // nếu đủ 10 lần, chuyển sang queue cũ
            if (w.reviewCount >= 10) {
                cout << "🎓 Từ \"" << w.engLish << "\" đã chuyển sang từ cũ.\n";
                oldWords.push(w);
            } else {
                rebuilt.push(w);
            }
        }
        // khôi phục thứ tự stack (đảo lại để giữ gần giống ban đầu)
        stack<Word> restore;
        while (!rebuilt.empty()) { restore.push(rebuilt.top()); rebuilt.pop(); }
        newWords = restore;
    }

    // Ôn từ cũ: lấy 10 từ từ front của queue; nếu không đủ, fallback random
    void reviewOld() {
        if (!getLoginStatus()) {
            cout << "⚠️ Bạn cần đăng nhập để ôn tập.\n";
            return;
        }
        if (oldWords.size() < 10) {
            cout << "⚠️ Từ cũ không đủ 10. Chuyển sang ôn ngẫu nhiên.\n";
            reviewRandom();
            return;
        }

        // Lấy đúng 10 từ từ queue theo FIFO
        vector<Word> pool;
        queue<Word> temp = oldWords;
        for (int i = 0; i < 10 && !temp.empty(); i++) {
            pool.push_back(temp.front());
            temp.pop();
        }
        cout << "\n===== ÔN TẬP TỪ CŨ (Queue) =====\n";
        askQuizOnPool(pool);

        // Sau mỗi lần ôn cũ: tăng reviewCount và xoay vòng
        int processed = 0;
        int total = oldWords.size();
        for (int i = 0; i < total; i++) {
            Word w = oldWords.front(); oldWords.pop();
            // nếu w nằm trong pool, tăng reviewCount
            for (auto& q : pool) {
                if (q.engLish == w.engLish && q.vietNam == w.vietNam) {
                    w.reviewCount++;
                    break;
                }
            }
            oldWords.push(w); // xoay vòng
            processed++;
        }
    }
};

// ===============================
// Demo main
// ===============================
int main() {
    // Tạo chủ đề Fruits
    Topic fruits("Fruits");
    fruits.addWord({"apple", "quả táo", IPA_ae, "I eat an apple every day"});
    fruits.addWord({"banana", "quả chuối", IPA_ae, "Bananas are yellow"});
    fruits.addWord({"orange", "quả cam", IPA_ae, "Orange juice is tasty"});
    fruits.addWord({"grape", "quả nho", IPA_ae, "Grapes are purple"});
    fruits.addWord({"mango", "quả xoài", IPA_ae, "Mango is sweet"});
    fruits.addWord({"lemon", "quả chanh", IPA_ae, "Lemon is sour"});
    fruits.addWord({"pear", "quả lê", IPA_ae, "Pears are juicy"});
    fruits.addWord({"peach", "quả đào", IPA_ae, "Peaches are soft"});
    fruits.addWord({"plum", "quả mận", IPA_ae, "Plums are small"});
    fruits.addWord({"melon", "dưa lưới", IPA_ae, "Melon is refreshing"});
    fruits.addWord({"papaya", "đu đủ", IPA_ae, "Papaya has many seeds"});

    // Tài khoản với Review
    Review acc;
    if (acc.signUp()) {
        if (acc.login()) {
            acc.setName("Fruits");

            // Lưu một số từ vào tài khoản (kho savedWords)
            for (const auto& w : fruits.getWords()) {
                acc.saveWord(w, fruits);
            }

            // Đồng thời đưa một số từ mới vào stack (từ đầu danh sách)
            // Hoặc dùng acc.addNewWord(w) nếu muốn vừa lưu vừa thêm vào stack
            for (int i = 0; i < 10 && i < (int)fruits.getWords().size(); i++) {
                acc.addNewWord(fruits.getWords()[i]);
            }

            cout << "\n===== MENU ÔN TẬP =====\n";
            cout << "1. Ôn ngẫu nhiên\n";
            cout << "2. Ôn từ mới (Stack)\n";
            cout << "3. Ôn từ cũ (Queue)\n";
            cout << "Chọn: ";
            int mode; 
            if (!(cin >> mode)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); mode = 1; }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (mode == 1) acc.reviewRandom();
            else if (mode == 2) acc.reviewNew();
            else if (mode == 3) acc.reviewOld();
            else {
                cout << "Lựa chọn không hợp lệ. Mặc định ôn ngẫu nhiên.\n";
                acc.reviewRandom();
            }

            cout << "\nHiển thị từ đã lưu (tham khảo):\n";
            acc.showSavedWords();

            acc.logout();
        }
    }

    return 0;
}