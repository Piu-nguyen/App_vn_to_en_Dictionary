#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

// ===============================
// Struct chứa thông tin một từ
// ===============================
struct Word {
    string english;
    string ipa;
    string vietnam;
    string example;
};

// ===============================
// Class Dictionary (cha)
// ===============================
class Dictionary {
protected:
    vector<Word> words;

public:
    void loadFromFile(const string& filename);
    void displayAll() const;
    string translate(const string& eng) const;
};

// ===============================
// Class Topic kế thừa Dictionary
// ===============================
class Topic : public Dictionary {
private:
    string name;
public:
    Topic(const string& n);
    string getName() const;
    void showTopic() const;
};

// ===============================
// Triển khai hàm Dictionary
// ===============================

// Đọc dữ liệu từ file (định dạng như file animals_topic_part1.txt)
void Dictionary::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "❌ Không thể mở file: " << filename << endl;
        return;
    }

    string line;
    Word currentWord;
    while (getline(file, line)) {
        // Bỏ dòng trống hoặc dòng tiêu đề
        if (line.empty() || line[0] == '#') continue;

        // Nếu dòng bắt đầu bằng "Example:", thì là ví dụ của từ trước
        if (line.rfind("Example:", 0) == 0) {
            currentWord.example = line.substr(8); // bỏ "Example:"
            words.push_back(currentWord);
            continue;
        }

        // Ngược lại là dòng chứa từ mới: dạng "dog /dɒɡ/ - chó"
        size_t slash1 = line.find('/');
        size_t slash2 = line.find('/', slash1 + 1);
        size_t dash = line.find('-');

        if (slash1 != string::npos && slash2 != string::npos && dash != string::npos) {
            currentWord.english = line.substr(0, slash1);
            currentWord.english.erase(currentWord.english.find_last_not_of(" \t") + 1);

            currentWord.ipa = line.substr(slash1, slash2 - slash1 + 1);

            currentWord.vietnam = line.substr(dash + 1);
            currentWord.vietnam.erase(0, currentWord.vietnam.find_first_not_of(" \t"));
            currentWord.vietnam.erase(currentWord.vietnam.find_last_not_of(" \t") + 1);
        }
    }
    file.close();

    cout << "✅ Đã nạp " << words.size() << " từ từ file " << filename << endl;
}

// In ra toàn bộ từ trong từ điển
void Dictionary::displayAll() const {
    for (const auto& w : words) {
        cout << w.english << " " << w.ipa << " - " << w.vietnam << endl;
        cout << "   ➤ Example:" << w.example << endl << endl;
    }
}

// Tra nghĩa tiếng Anh
string Dictionary::translate(const string& eng) const {
    for (const auto& w : words)
        if (w.english == eng)
            return w.vietnam;
    return "Không tìm thấy từ này!";
}

// ===============================
// Triển khai hàm Topic
// ===============================
Topic::Topic(const string& n) : name(n) {}

string Topic::getName() const {
    return name;
}

void Topic::showTopic() const {
    cout << "📘 Chủ đề: " << name << endl;
    displayAll();
}

// ===============================
// Hàm main
// ===============================
int main() {
    cout << "=== VOCAB BY TOPIC ===\n";

    // Quét thư mục hiện tại lấy tất cả file txt (mỗi file = 1 topic)
    vector<string> topics;
    for (auto& p : fs::directory_iterator(".")) {
        if (p.path().extension() == ".txt")
            topics.push_back(p.path().string());
    }

    if (topics.empty()) {
        cout << "❗ Không tìm thấy file chủ đề (.txt) nào!\n";
        return 0;
    }

    cout << "Danh sách chủ đề:\n";
    for (size_t i = 0; i < topics.size(); ++i)
        cout << i + 1 << ". " << topics[i] << endl;

    cout << "Chọn số thứ tự chủ đề: ";
    int choice;
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > (int)topics.size()) {
        cout << "Lựa chọn không hợp lệ!\n";
        return 0;
    }

    string fileName = topics[choice - 1];
    string topicName = fs::path(fileName).stem().string();

    Topic selected(topicName);
    selected.loadFromFile(fileName);
    selected.showTopic();

    // Cho phép tra nghĩa
    string w;
    cout << "\nNhập từ tiếng Anh cần tra: ";
    getline(cin, w);
    cout << "→ " << selected.translate(w) << endl;

    return 0;
}
