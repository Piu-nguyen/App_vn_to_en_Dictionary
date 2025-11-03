#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <random>
#include <fstream>
#include <map>
using namespace std;

enum IPA { IPA_ae };

struct Word {
    string engLish;
    string vietNam;
    IPA ipa;
    string example;
};

union SearchKey {
    const char* keyWord;
    int hash;
};

class Dictionary {
protected:
    struct Question {
        string question;
        vector<string> options;
        int correct;
    };

    struct Theory {
        string formulaAff;
        string formulaNeg;
        string formulaQ;
        string signals;
        string example;
    };

    map<string, Theory> theories;                 // tenseName -> theory
    map<string, vector<Question>> questionsBank;  // tenseName -> questions

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
    // load all data from file (theory + questions)
    bool loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Không thể mở file: " << filename << "\n";
            return false;
        }

        string line;
        string currentTense;
        enum ParseState { OUT, IN_THEORY, IN_QUESTIONS } state = OUT;

        Theory curTheory;
        Dictionary::Question curQ;
        int qLineCount = 0;

        while (getline(file, line)) {
            // trim trailing carriage return (Windows)
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.empty()) continue;

            // Detect header [TENSE]
            if (line.size() > 1 && line.front() == '[' && line.back() == ']') {
                string inner = line.substr(1, line.size() - 2);
                // If it's [END], finish current tense
                if (inner == "END") {
                    if (!currentTense.empty()) {
                        if (state == IN_QUESTIONS) {
                            // finalize any partially read question (should not happen)
                        }
                        // store theory if not already stored (it should be)
                        theories[currentTense] = curTheory;
                        currentTense.clear();
                        state = OUT;
                    }
                    continue;
                }

                // New tense encountered
                currentTense = inner;
                state = IN_THEORY;
                // reset theory
                curTheory = Theory();
                questionsBank[currentTense].clear();
                continue;
            }

            if (state == IN_THEORY) {
                // theory lines expected in form "KEY: value"
                size_t p = line.find(':');
                if (p != string::npos) {
                    string key = line.substr(0, p);
                    string val = line.substr(p + 1);
                    // trim leading spaces
                    while (!val.empty() && (val.front() == ' ' || val.front() == '\t')) val.erase(val.begin());
                    if (key == "FORMULA_AFF") curTheory.formulaAff = val;
                    else if (key == "FORMULA_NEG") curTheory.formulaNeg = val;
                    else if (key == "FORMULA_Q") curTheory.formulaQ = val;
                    else if (key == "SIGNALS") curTheory.signals = val;
                    else if (key == "EXAMPLE") curTheory.example = val;
                    else if (key == "QUESTIONS") {
                        // switch to reading questions next lines
                        state = IN_QUESTIONS;
                        qLineCount = 0;
                    }
                } else {
                    // ignore
                }
            } else if (state == IN_QUESTIONS) {
                // each question block: 1 question line + 4 option lines + 1 correct index line (=6 lines)
                if (qLineCount == 0) {
                    curQ = Question();
                    curQ.question = line;
                    curQ.options.clear();
                    qLineCount = 1;
                } else if (qLineCount >= 1 && qLineCount <= 4) {
                    curQ.options.push_back(line);
                    qLineCount++;
                } else if (qLineCount == 5) {
                    // correct
                    try {
                        curQ.correct = stoi(line);
                    } catch (...) { curQ.correct = 1; }
                    questionsBank[currentTense].push_back(curQ);
                    qLineCount = 0;
                }
            }
        }

        file.close();
        return true;
    }

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
                cout << "Nhập sai. Vui lòng nhập số.\n";
                continue;
            }
            cin.ignore(10000, '\n');

            if (choice == 0) {
                cout << "Thoát học ngữ pháp.\n";
                break;
            }

            string tenseName = getTenseName(choice);
            if (tenseName.empty()) {
                cout << "Số không hợp lệ!\n";
                continue;
            }

            if (theories.find(tenseName) == theories.end()) {
                cout << "Không có dữ liệu lý thuyết cho thì " << tenseName << ".\n";
                continue;
            }

            showTheory(tenseName, theories[tenseName]);

            vector<Question> qv;
            if (questionsBank.find(tenseName) != questionsBank.end())
                qv = questionsBank[tenseName];

            if (qv.empty()) {
                cout << "Không có câu hỏi cho thì này.\n";
                continue;
            }

            grammarQuiz(qv);
        }
    }

private:
    string getTenseName(int choice) {
        switch (choice) {
            case 1: return "PRESENT SIMPLE";
            case 2: return "PRESENT CONTINUOUS";
            case 3: return "PRESENT PERFECT";
            case 4: return "PRESENT PERFECT CONTINUOUS";
            case 5: return "PAST SIMPLE";
            case 6: return "PAST CONTINUOUS";
            case 7: return "PAST PERFECT";
            case 8: return "PAST PERFECT CONTINUOUS";
            case 9: return "FUTURE SIMPLE";
            case 10: return "FUTURE CONTINUOUS";
            case 11: return "FUTURE PERFECT";
            case 12: return "FUTURE PERFECT CONTINUOUS";
            default: return "";
        }
    }

    void showTheory(const string& tense, const Theory& t) {
        cout << "\n----------------------------------------\n";
        cout << "THÌ: " << tense << "\n";
        cout << "Công thức (khẳng định): " << t.formulaAff << "\n";
        cout << "Công thức (phủ định): " << t.formulaNeg << "\n";
        cout << "Công thức (nghi vấn): " << t.formulaQ << "\n";
        cout << "Dấu hiệu nhận biết: " << t.signals << "\n";
        cout << "Ví dụ: " << t.example << "\n";
        cout << "----------------------------------------\n\n";
    }

    void grammarQuiz(vector<Question> questions) {
        random_device rd;
        mt19937 g(rd());
        shuffle(questions.begin(), questions.end(), g);

        int total = (int)questions.size();
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
};

class Topic : public Dictionary {
private:
    string name;
public:
    Topic() : name("") {}
    Topic(const string& n) : name(n) {}
    void setName(const string& n) { name = n; }
    string getName() const { return name; }
};

class Account {
private:
    string username;
    string password;
public:
    Account() : username(""), password("") {}
    Account(const string& u, const string& p) : username(u), password(p) {}
    void setUsername(const string& u) { username = u; }
    void setPassword(const string& p) { password = p; }
    string getUsername() const { return username; }
};

int main() {
    Dictionary dict;
    if (!dict.loadFromFile("grammar_data.txt")) {
        cerr << "Lỗi: không đọc được file grammar_data.txt. Hãy chắc file ở cùng thư mục.\n";
        return 1;
    }
    dict.learnGrammars();
    return 0;
}
