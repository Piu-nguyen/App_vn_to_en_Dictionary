#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

// ===============================
// 🎵 Enum cho âm IPA
// ===============================
enum IPA {
    IPA_ae,
    // ... thêm âm khác sau
};

// ===============================
// 📘 Struct chứa thông tin một từ
// ===============================
struct Word {
    string engLish;    // English word
    string vietNam;    // Vietnamese meaning
    IPA ipa;           // IPA symbol
    string example;    // Example sentence
};

// ===============================
// 🔍 Union hỗ trợ tìm kiếm
// ===============================
union SearchKey {
    const char* keyWord;
    int hash;
};

// ===============================
// 📚 Class Dictionary (cha)
// ===============================
class Dictionary {
protected:
    vector<Word> words;

    int hashString(const string& s) {
        int hash = 0;
        for (char c : s) hash = hash * 31 + tolower((unsigned char)c);
        return hash;
    }

    string getIPAString(IPA ipa) const {
        switch (ipa) {
            case IPA_ae: return "/æ/";
            default: return "(unknown)";
        }
    }

public:
    // ===============================
    // 🎓 Hàm học ngữ pháp
    // ===============================
    void learnGrammars() {
        int choice;
        cout << "\n=== HỌC NGỮ PHÁP TIẾNG ANH ===\n";
        cout << "1. Hiện tại đơn (Present Simple)\n";
        cout << "2. Hiện tại tiếp diễn (Present Continuous)\n";
        cout << "3. Quá khứ đơn (Past Simple)\n";
        cout << "4. Tương lai đơn (Future Simple)\n";
        cout << "0. Thoát\n";
        cout << "Chọn thì bạn muốn học: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                showTheory("HIỆN TẠI ĐƠN (Present Simple)",
                    "S + V(s/es) + O",
                    "Diễn tả thói quen, sự thật hiển nhiên, hành động lặp lại.",
                    "He plays football every Sunday.");
                grammarQuiz({
                    {"He ____ football every Sunday.", {"play", "plays", "playing", "played"}, 2},
                    {"They ____ to school by bus.", {"go", "goes", "went", "gone"}, 1},
                    {"My father ____ coffee every morning.", {"drink", "drinks", "is drinking", "drank"}, 2},
                    {"The sun ____ in the east.", {"rise", "rises", "rising", "rose"}, 2},
                    {"She ____ not like bananas.", {"does", "do", "is", "was"}, 1}
                });
                break;

            case 2:
                showTheory("HIỆN TẠI TIẾP DIỄN (Present Continuous)",
                    "S + am/is/are + V-ing",
                    "Diễn tả hành động đang xảy ra ngay lúc nói hoặc xung quanh thời điểm nói.",
                    "She is reading a book now.");
                grammarQuiz({
                    {"She ____ TV now.", {"watch", "is watching", "watches", "watched"}, 2},
                    {"We ____ dinner at the moment.", {"are having", "have", "had", "has"}, 1},
                    {"I ____ to music right now.", {"listen", "am listening", "listens", "listened"}, 2},
                    {"They ____ in the park.", {"run", "are running", "runs", "ran"}, 2},
                    {"Look! The baby ____.", {"cry", "is crying", "cries", "cried"}, 2}
                });
                break;

            case 3:
                showTheory("QUÁ KHỨ ĐƠN (Past Simple)",
                    "S + V2/ed + O",
                    "Diễn tả hành động đã xảy ra và kết thúc trong quá khứ.",
                    "I visited Hanoi last year.");
                grammarQuiz({
                    {"I ____ to the park yesterday.", {"go", "went", "goes", "gone"}, 2},
                    {"They ____ football last Sunday.", {"play", "played", "plays", "playing"}, 2},
                    {"She ____ a letter to her friend.", {"write", "wrote", "writes", "writing"}, 2},
                    {"He ____ late for school yesterday.", {"is", "was", "were", "be"}, 2},
                    {"We ____ dinner at 7 p.m. last night.", {"have", "had", "has", "having"}, 2}
                });
                break;

            case 4:
                showTheory("TƯƠNG LAI ĐƠN (Future Simple)",
                    "S + will + V + O",
                    "Diễn tả hành động sẽ xảy ra trong tương lai.",
                    "I will call you tomorrow.");
                grammarQuiz({
                    {"I ____ call you tomorrow.", {"will", "shall", "am", "was"}, 1},
                    {"They ____ travel to Da Nang next week.", {"will", "is", "are", "was"}, 1},
                    {"She ____ come to the party tonight.", {"will", "is", "was", "does"}, 1},
                    {"We ____ study harder next semester.", {"will", "shall", "are", "was"}, 1},
                    {"It ____ rain tomorrow.", {"will", "is", "was", "has"}, 1}
                });
                break;

            default:
                cout << "Thoát học ngữ pháp.\n";
                break;
        }
    }

private:
    // ===============================
    // 🧩 Cấu trúc câu hỏi
    // ===============================
    struct Question {
        string question;
        vector<string> options;
        int correct;
    };

    // ===============================
    // 📘 Hàm hiển thị lý thuyết
    // ===============================
    void showTheory(const string& title, const string& formula,
                    const string& usage, const string& example) {
        cout << "\n==============================\n";
        cout << "🕓 Thì " << title << "\n";
        cout << "👉 Cấu trúc: " << formula << "\n";
        cout << "👉 Cách dùng: " << usage << "\n";
        cout << "📌 Ví dụ: " << example << "\n";
        cout << "==============================\n\n";
    }

    // ===============================
    // 🧠 Mini quiz luyện tập
    // ===============================
    void grammarQuiz(const vector<Question>& questions) {
        int score = 0;
        for (size_t i = 0; i < questions.size(); ++i) {
            cout << "Câu " << i + 1 << ": " << questions[i].question << "\n";
            for (size_t j = 0; j < questions[i].options.size(); ++j)
                cout << "   " << j + 1 << ". " << questions[i].options[j] << "\n";
            cout << "Chọn đáp án (1-" << questions[i].options.size() << "): ";
            int ans; cin >> ans;
            if (ans == questions[i].correct) {
                cout << "✅ Chính xác!\n\n";
                ++score;
            } else {
                cout << "❌ Sai rồi! Đáp án đúng là: "
                     << questions[i].options[questions[i].correct - 1] << "\n\n";
            }
        }
        cout << "🎯 Kết quả: " << score << "/" << questions.size() << " câu đúng!\n";
    }
};

// ===============================
// 🌍 Class Topic kế thừa Dictionary
// ===============================
class Topic : public Dictionary {
private:
    string name;
public:
};

// ===============================
// 👤 Class Account (đăng nhập)
// ===============================
class Account {
private:
    string username;
    string password;
public:
};

// ===============================
// 🧪 Main test
// ===============================
int main() {
    Dictionary dict;
    dict.learnGrammars();
    return 0;
}
