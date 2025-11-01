#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <random>
#include <fstream>
using namespace std;

// ===============================
// Enum cho âm IPA (giữ nguyên như ban đầu)
// ===============================
enum IPA {
    IPA_ae,
    // ... thêm âm khác sau
};

// ===============================
// Struct chứa thông tin một từ (giữ nguyên như ban đầu)
// ===============================
struct Word {
    string engLish;
    string vietNam;
    IPA ipa;
    string example;
};

// ===============================
// Union hỗ trợ tìm kiếm (giữ nguyên như ban đầu)
// ===============================
union SearchKey {
    const char* keyWord;
    int hash;
};

// ===============================
// Class Dictionary (cha) - giữ cấu trúc, mở rộng hàm học ngữ pháp
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
    // Hàm học ngữ pháp (12 thì, mỗi thì 10 câu)
    // ===============================
    void learnGrammars() {
    while (true) {
        int choice;
        cout << "\n=== HỌC NGỮ PHÁP 12 THÌ ===\n";
        cout << "1  Present Simple\n2  Present Continuous\n3  Present Perfect\n4  Present Perfect Continuous\n";
        cout << "5  Past Simple\n6  Past Continuous\n7  Past Perfect\n8  Past Perfect Continuous\n";
        cout << "9  Future Simple\n10 Future Continuous\n11 Future Perfect\n12 Future Perfect Continuous\n";
        cout << "0  Thoát\n";
        cout << "Chọn số (0-12): ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Nhập sai. Vui lòng nhập một số.\n";
            continue;
        }
        cin.ignore(10000, '\n');

        if (choice == 0) {
            cout << "Thoát học ngữ pháp.\n";
            break;
        }

        switch (choice) {
            case 1:
                showTheory("PRESENT SIMPLE", "S + V(s/es) + O",
                           "Thói quen, sự thật hiển nhiên, hành động lặp lại.",
                           "I usually get up at 6 a.m.");
                grammarQuiz(presentSimpleQuestions());
                break;
            case 2:
                showTheory("PRESENT CONTINUOUS", "S + am/is/are + V-ing",
                           "Hành động đang diễn ra ngay lúc nói.",
                           "He is talking on the phone right now.");
                grammarQuiz(presentContinuousQuestions());
                break;
            case 3:
                showTheory("PRESENT PERFECT", "S + have/has + V3",
                           "Hành động đã xảy ra và còn ảnh hưởng đến hiện tại.",
                           "She has visited Japan twice.");
                grammarQuiz(presentPerfectQuestions());
                break;
            case 4:
                showTheory("PRESENT PERFECT CONTINUOUS", "S + have/has been + V-ing",
                           "Hành động bắt đầu trong quá khứ và còn tiếp diễn đến hiện tại.",
                           "I have been learning English for three years.");
                grammarQuiz(presentPerfContQuestions());
                break;
            case 5:
                showTheory("PAST SIMPLE", "S + V2 + O",
                           "Hành động đã xảy ra và kết thúc trong quá khứ.",
                           "We watched a movie last night.");
                grammarQuiz(pastSimpleQuestions());
                break;
            case 6:
                showTheory("PAST CONTINUOUS", "S + was/were + V-ing",
                           "Hành động đang diễn ra tại một thời điểm trong quá khứ.",
                           "I was reading when the phone rang.");
                grammarQuiz(pastContinuousQuestions());
                break;
            case 7:
                showTheory("PAST PERFECT", "S + had + V3",
                           "Hành động xảy ra trước một hành động khác trong quá khứ.",
                           "They had eaten dinner before I arrived.");
                grammarQuiz(pastPerfectQuestions());
                break;
            case 8:
                showTheory("PAST PERFECT CONTINUOUS", "S + had been + V-ing",
                           "Hành động kéo dài đến một thời điểm trong quá khứ.",
                           "He had been driving for hours before he stopped.");
                grammarQuiz(pastPerfContQuestions());
                break;
            case 9:
                showTheory("FUTURE SIMPLE", "S + will + V",
                           "Dự đoán, quyết định ngay lúc nói, lời hứa.",
                           "She will start college next year.");
                grammarQuiz(futureSimpleQuestions());
                break;
            case 10:
                showTheory("FUTURE CONTINUOUS", "S + will be + V-ing",
                           "Hành động sẽ đang diễn ra tại một thời điểm trong tương lai.",
                           "At 9 a.m. tomorrow, I will be working in the office.");
                grammarQuiz(futureContinuousQuestions());
                break;
            case 11:
                showTheory("FUTURE PERFECT", "S + will have + V3",
                           "Hành động sẽ hoàn thành trước một thời điểm trong tương lai.",
                           "By next week, they will have finished the project.");
                grammarQuiz(futurePerfectQuestions());
                break;
            case 12:
                showTheory("FUTURE PERFECT CONTINUOUS", "S + will have been + V-ing",
                           "Hành động sẽ kéo dài đến một thời điểm trong tương lai.",
                           "By 2030, she will have been living abroad for ten years.");
                grammarQuiz(futurePerfContQuestions());
                break;
            default:
                cout << "Vui lòng chọn số hợp lệ (0-12).\n";
        }
    }
}


private:
    // ===============================
    // Helper: hiển thị lý thuyết
    // ===============================
    void showTheory(const string& title, const string& formula,
                    const string& usage, const string& example) {
        cout << "\n----------------------------------------\n";
        cout << "THÌ: " << title << "\n";
        cout << "Cấu trúc: " << formula << "\n";
        cout << "Cách dùng: " << usage << "\n";
        cout << "Ví dụ: " << example << "\n";
        cout << "----------------------------------------\n\n";
    }

    // ===============================
    // Quiz runner: shuffle & hỏi toàn bộ câu (10 câu mỗi thì)
    // ===============================
    struct Question {
        string question;
        vector<string> options;
        int correct; // 1-based
    };

    void grammarQuiz(vector<Question> questions) {
        if (questions.empty()) {
            cout << "Chưa có câu hỏi cho phần này.\n";
            return;
        }

        random_device rd;
        mt19937 g(rd());
        shuffle(questions.begin(), questions.end(), g);

        // ensure we ask all available questions (should be 10)
        int total = static_cast<int>(questions.size());
        int score = 0;

        for (int i = 0; i < total; ++i) {
            cout << "Câu " << (i + 1) << ": " << questions[i].question << "\n";
            for (size_t j = 0; j < questions[i].options.size(); ++j)
                cout << "   " << (j + 1) << ". " << questions[i].options[j] << "\n";

            int ans;
            while (true) {
                cout << "Chọn đáp án (1-" << questions[i].options.size() << "): ";
                if ((cin >> ans) && ans >= 1 && ans <= (int)questions[i].options.size()) {
                    cin.ignore(10000, '\n');
                    break;
                } else {
                    cout << "Nhập sai. Vui lòng nhập số hợp lệ.\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                }
            }

            if (ans == questions[i].correct) {
                cout << "Chính xác!\n\n";
                ++score;
            } else {
                cout << "Sai rồi. Đáp án đúng là: " << questions[i].options[questions[i].correct - 1] << "\n\n";
            }
        }

        cout << "Kết quả: " << score << "/" << total << " đúng.\n";
    }

    // ===============================
    // Dữ liệu: 10 câu cho mỗi thì
    // ===============================
    vector<Question> presentSimpleQuestions() {
        return {
            {"He ____ football every Sunday.", {"play", "plays", "playing", "played"}, 2},
            {"They ____ to school by bus.", {"go", "goes", "went", "gone"}, 1},
            {"My father ____ coffee every morning.", {"drink", "drinks", "is drinking", "drank"}, 2},
            {"The sun ____ in the east.", {"rise", "rises", "rising", "rose"}, 2},
            {"She ____ not like bananas.", {"does", "do", "is", "was"}, 1},
            {"Cats usually ____ at night.", {"hunt", "hunts", "hunted", "hunting"}, 2},
            {"He ____ his homework after school.", {"do", "does", "did", "doing"}, 2},
            {"Water ____ at 100°C.", {"boil", "boils", "boiled", "boiling"}, 2},
            {"My sister ____ TV every evening.", {"watch", "watches", "watched", "watching"}, 2},
            {"Birds ____ when they fly.", {"sing", "sings", "singing", "sang"}, 1}
        };
    }

    vector<Question> presentContinuousQuestions() {
        return {
            {"She ____ TV now.", {"watch", "is watching", "watches", "watched"}, 2},
            {"We ____ dinner at the moment.", {"are having", "have", "had", "has"}, 1},
            {"I ____ to music right now.", {"listen", "am listening", "listens", "listened"}, 2},
            {"They ____ in the park.", {"run", "are running", "runs", "ran"}, 2},
            {"Look! The baby ____.", {"cry", "is crying", "cries", "cried"}, 2},
            {"It ____ raining outside.", {"is", "are", "was", "were"}, 1},
            {"He ____ for the bus now.", {"wait", "is waiting", "waits", "waited"}, 2},
            {"We ____ on a new project this week.", {"work", "are working", "works", "worked"}, 2},
            {"The dog ____ at the cat.", {"bark", "is barking", "barks", "barked"}, 2},
            {"You ____ too fast on the phone.", {"speak", "are speaking", "speaks", "spoke"}, 2}
        };
    }

    vector<Question> presentPerfectQuestions() {
        return {
            {"I ____ finished my homework.", {"has", "have", "had", "am"}, 2},
            {"She ____ never been to Paris.", {"has", "have", "is", "was"}, 1},
            {"They ____ lived here for ten years.", {"have", "has", "had", "are"}, 1},
            {"He ____ just eaten lunch.", {"have", "has", "had", "is"}, 2},
            {"We ____ not met before.", {"has", "have", "did", "are"}, 2},
            {"It ____ rained all day.", {"has", "have", "is", "was"}, 1},
            {"You ____ done a great job.", {"have", "has", "had", "are"}, 1},
            {"I ____ lost my keys.", {"have", "has", "had", "am"}, 1},
            {"She ____ already left.", {"have", "has", "had", "is"}, 2},
            {"They ____ seen that movie twice.", {"have", "has", "had", "are"}, 1}
        };
    }

    vector<Question> presentPerfContQuestions() {
        return {
            {"I ____ studying for two hours.", {"have been", "has been", "had been", "have"}, 1},
            {"She ____ been working here since May.", {"has", "has been", "have been", "had"}, 2},
            {"They ____ been waiting for you.", {"have been", "has been", "had been", "have"}, 1},
            {"He looks tired because he ____ running.", {"has been", "have been", "had been", "is"}, 1},
            {"We ____ been learning English for years.", {"have been", "has been", "had been", "are"}, 1},
            {"You ____ been practicing too much.", {"have been", "has been", "had been", "are"}, 1},
            {"It ____ been raining all morning.", {"has been", "have been", "had been", "is"}, 1},
            {"She ____ been reading that book for weeks.", {"has been", "have been", "had been", "is"}, 1},
            {"I ____ been working on this project lately.", {"have been", "has been", "had been", "am"}, 1},
            {"They ____ been arguing since yesterday.", {"have been", "has been", "had been", "are"}, 1}
        };
    }

    vector<Question> pastSimpleQuestions() {
        return {
            {"I ____ to the park yesterday.", {"go", "went", "goes", "gone"}, 2},
            {"They ____ football last Sunday.", {"play", "played", "plays", "playing"}, 2},
            {"She ____ a letter to her friend.", {"write", "wrote", "writes", "writing"}, 2},
            {"He ____ late for school yesterday.", {"is", "was", "were", "be"}, 2},
            {"We ____ dinner at 7 p.m. last night.", {"have", "had", "has", "having"}, 2},
            {"Tom ____ his keys yesterday.", {"lose", "lost", "loses", "losing"}, 2},
            {"I ____ at home all day.", {"stay", "stayed", "stays", "staying"}, 2},
            {"She ____ the movie two days ago.", {"see", "saw", "seen", "seeing"}, 2},
            {"They ____ a party last weekend.", {"have", "had", "has", "having"}, 2},
            {"He ____ the letter before he left.", {"read", "read", "reads", "reading"}, 1}
        };
    }

    vector<Question> pastContinuousQuestions() {
        return {
            {"I ____ watching TV at 8 p.m. last night.", {"was", "were", "am", "is"}, 1},
            {"They ____ playing football when it started to rain.", {"was", "were", "are", "is"}, 2},
            {"She ____ cooking while he was cleaning.", {"was", "were", "is", "are"}, 1},
            {"We ____ having dinner at 7pm.", {"was", "were", "are", "is"}, 2},
            {"He ____ reading when I entered.", {"was", "were", "is", "are"}, 1},
            {"It ____ raining all evening yesterday.", {"was", "were", "is", "are"}, 1},
            {"You ____ sleeping at that time.", {"was", "were", "are", "is"}, 2},
            {"The children ____ playing in the yard.", {"was", "were", "are", "is"}, 2},
            {"I ____ not paying attention then.", {"was", "were", "am", "is"}, 1},
            {"She ____ listening to music when it happened.", {"was", "were", "is", "are"}, 1}
        };
    }

    vector<Question> pastPerfectQuestions() {
        return {
            {"He ____ already left when I arrived.", {"has", "had", "have", "is"}, 2},
            {"They ____ finished the meal before we came.", {"had", "have", "has", "is"}, 1},
            {"I ____ seen the film before yesterday.", {"have", "had", "has", "was"}, 2},
            {"She ____ cooked by the time he arrived.", {"had", "have", "has", "is"}, 1},
            {"We ____ not spoken to him before that day.", {"had", "have", "has", "was"}, 1},
            {"You ____ never visited that place until 2019.", {"had", "have", "has", "is"}, 1},
            {"The train ____ left when I reached the station.", {"had", "has", "have", "is"}, 1},
            {"By the time the show started, she ____ sat down.", {"had", "has", "have", "is"}, 1},
            {"He ____ finished his work before break.", {"had", "have", "has", "is"}, 1},
            {"They ____ already gone when we called.", {"had", "have", "has", "is"}, 1}
        };
    }

    vector<Question> pastPerfContQuestions() {
        return {
            {"She ____ been waiting for an hour before the bus came.", {"had been", "has been", "have been", "had"}, 1},
            {"They ____ been playing for two hours when it started raining.", {"had been", "have been", "has been", "had"}, 1},
            {"He ____ been working all day before he rested.", {"had been", "have been", "has been", "is"}, 1},
            {"I ____ been thinking about it before you mentioned it.", {"had been", "have been", "has been", "had"}, 1},
            {"We ____ been living there for years before we moved.", {"had been", "have been", "has been", "was"}, 1},
            {"You ____ been practicing when I saw you.", {"had been", "have been", "has been", "was"}, 1},
            {"It ____ been raining for hours before it stopped.", {"had been", "have been", "has been", "is"}, 1},
            {"She ____ been studying hard prior to the exam.", {"had been", "have been", "has been", "is"}, 1},
            {"They ____ been arguing earlier that day.", {"had been", "have been", "has been", "are"}, 1},
            {"He ____ been trying to fix it before calling help.", {"had been", "have been", "has been", "is"}, 1}
        };
    }

    vector<Question> futureSimpleQuestions() {
        return {
            {"I ____ call you tomorrow.", {"will", "shall", "am", "was"}, 1},
            {"They ____ travel to Da Nang next week.", {"will", "is", "are", "was"}, 1},
            {"She ____ come to the party tonight.", {"will", "is", "was", "does"}, 1},
            {"We ____ study harder next semester.", {"will", "shall", "are", "was"}, 1},
            {"It ____ rain tomorrow.", {"will", "is", "was", "has"}, 1},
            {"He thinks she ____ be late.", {"will", "is", "was", "has"}, 1},
            {"I ____ help you with that.", {"will", "am", "was", "is"}, 1},
            {"They ____ decide after meeting.", {"will", "are", "have", "was"}, 1},
            {"She ____ probably come.", {"will", "is", "was", "does"}, 1},
            {"We ____ not forget this day.", {"will", "do", "did", "have"}, 1}
        };
    }

    vector<Question> futureContinuousQuestions() {
        return {
            {"At 8 pm tonight I ____ watching the match.", {"will be", "will", "am", "is"}, 1},
            {"She ____ be working at 9 tomorrow.", {"will be", "will", "is", "am"}, 1},
            {"We ____ be staying at the new hotel next week.", {"will be", "will", "are", "is"}, 1},
            {"They ____ be travelling around this time next year.", {"will be", "will", "are", "is"}, 1},
            {"He ____ be studying when you arrive.", {"will be", "will", "is", "are"}, 1},
            {"I ____ be waiting for you at the station.", {"will be", "will", "am", "is"}, 1},
            {"You ____ be meeting the team tomorrow noon.", {"will be", "will", "are", "is"}, 1},
            {"It ____ be snowing then.", {"will be", "will", "is", "are"}, 1},
            {"She ____ be presenting at the conference.", {"will be", "will", "is", "was"}, 1},
            {"We ____ be celebrating our anniversary next month.", {"will be", "will", "are", "is"}, 1}
        };
    }

    vector<Question> futurePerfectQuestions() {
        return {
            {"By 2025 she ____ graduated.", {"will have", "will be", "will", "has"}, 1},
            {"They ____ finished the work by tomorrow.", {"will have", "will be", "will", "have"}, 1},
            {"I ____ left by the time you arrive.", {"will have", "will be", "will", "am"}, 1},
            {"He ____ completed the report before Monday.", {"will have", "will be", "will", "has"}, 1},
            {"We ____ saved enough money by then.", {"will have", "will be", "will", "have"}, 1},
            {"You ____ read the book by the end of week.", {"will have", "will be", "will", "have"}, 1},
            {"It ____ stopped raining by noon.", {"will have", "will be", "will", "has"}, 1},
            {"She ____ already left by then.", {"will have", "will be", "will", "has"}, 1},
            {"They ____ completed ten tasks by Friday.", {"will have", "will be", "will", "have"}, 1},
            {"I ____ finished the project before the meeting.", {"will have", "will be", "will", "have"}, 1}
        };
    }

    vector<Question> futurePerfContQuestions() {
        return {
            {"By next month I ____ been working here for a year.", {"will have been", "will be", "will have", "have been"}, 1},
            {"She ____ been traveling for hours by the time she arrives.", {"will have been", "will be", "will", "has been"}, 1},
            {"They ____ been living here for ten years by 2030.", {"will have been", "will be", "will", "have been"}, 1},
            {"He ____ been playing football for two hours by then.", {"will have been", "will be", "will", "has been"}, 1},
            {"We ____ been waiting for three hours by midnight.", {"will have been", "will be", "will", "have been"}, 1},
            {"I ____ been studying for five hours by dinner.", {"will have been", "will be", "will", "have been"}, 1},
            {"You ____ been running for an hour by 7am.", {"will have been", "will be", "will", "have been"}, 1},
            {"It ____ been snowing for days by the time they leave.", {"will have been", "will be", "will", "has been"}, 1},
            {"She ____ been practicing the piano for months by recital.", {"will have been", "will be", "will", "has been"}, 1},
            {"They ____ been preparing the show for weeks by opening night.", {"will have been", "will be", "will", "have been"}, 1}
        };
    }

}; // end class Dictionary

// ===============================
// Class Topic kế thừa Dictionary (giữ nguyên như bạn muốn)
// ===============================
class Topic : public Dictionary {
private:
    string name;
public:
    Topic() : name("") {}
    Topic(const string& n) : name(n) {}
    void setName(const string& n) { name = n; }
    string getName() const { return name; }
};

// ===============================
// Class Account (đăng nhập) (mới thêm để lưu username/password ra file)
// ===============================
class Account {
private:
    string username;
    string password;
    const string filename = "accounts.txt";

public:
    Account() : username(""), password("") {}
    Account(const string& u, const string& p) : username(u), password(p) {}
    void setUsername(const string& u) { username = u; }
    void setPassword(const string& p) { password = p; }
    string getUsername() const { return username; }

    // Đăng ký -> lưu username password vào file accounts.txt
    bool signUp() {
        cout << "\n=== ĐĂNG KÝ ===\n";
        cout << "Tên đăng nhập: ";
        getline(cin, username);
        cout << "Mật khẩu: ";
        getline(cin, password);

        // Kiểm tra trùng username
        ifstream inFile(filename);
        string u, p;
        while (inFile >> u >> p) {
            if (u == username) {
                cout << "❌ Tên đăng nhập đã tồn tại!\n";
                return false;
            }
        }
        inFile.close();

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
                return true;
            }
        }

        cout << "❌ Sai tên đăng nhập hoặc mật khẩu.\n";
        return false;
    }
};

// ===============================
//Main test
// ===============================
int main() {

    Dictionary dict;
    Account acc;

    while (true) {
        cout << "\n=== MENU CHÍNH ===\n";
        cout << "1. Đăng ký\n2. Đăng nhập\n0. Thoát\nChọn: ";
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Nhập sai. Vui lòng nhập số.\n";
            continue;
        }
        cin.ignore(10000, '\n');

        if (choice == 1) {
            acc.signUp();
        } else if (choice == 2) {
            if (acc.login()) {
                dict.learnGrammars();
            }
        } else if (choice == 0) {
            cout << "Thoát chương trình.\n";
            break;
        } else {
            cout << "Lựa chọn không hợp lệ.\n";
        }
    }

    return 0;
    
}
