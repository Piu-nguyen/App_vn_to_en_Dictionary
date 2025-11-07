#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <random>
#include <fstream>
#include <map>
#include <sstream>
using namespace std;

// ===============================
// ENUM & STRUCT CƠ BẢN
// ===============================
enum IPA { IPA_ae };

struct Word {
    string engLish;
    string vietNam;
    IPA ipa;
    string example;
};

// ===============================
// CẤU TRÚC CÂU HỎI & BÀI ÔN TẬP
// ===============================
struct MCQ {
    string question;
    vector<string> options;
    int answer;
};

struct GapFill {
    string question;
    string answer;
    string hint;
};

struct Rearrange {
    string jumbled;
    string correct;
};

struct PracticeTest {
    vector<MCQ> mcqs;
    vector<GapFill> gapfills;
    vector<Rearrange> rearranges;
};

// ===============================
// CLASS CHÍNH: DICTIONARY
// ===============================
class Dictionary {
protected:
    struct Theory {
        string formulaAff;
        string formulaNeg;
        string formulaQ;
        string signals;
        string example;
    };

    struct Question {
        string question;
        vector<string> options;
        int correct;
    };

    map<string, Theory> theories;
    map<string, vector<Question>> grammarQuestions;
    vector<PracticeTest> practiceTests;

public:
    bool loadFromFile(const string& filename);
    void learnGrammars();
    bool loadPracticeFile(const string& filename);
    void practiceTest();

private:
    string getTenseName(int choice);
    void showTheory(const string& tense, const Theory& t);
    void grammarQuiz(const vector<Question>& questions);
};

// ===============================
// ĐỌC FILE LÝ THUYẾT 12 THÌ
// ===============================
bool Dictionary::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    string line, currentTense;
    Theory curTheory;
    vector<Question> curQs;
    Question curQ;
    enum State { OUT, THEORY, QUESTION } state = OUT;
    int step = 0;

    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        if (line.front() == '[' && line.back() == ']') {
            string tag = line.substr(1, line.size() - 2);
            if (tag == "END") {
                if (!currentTense.empty()) {
                    theories[currentTense] = curTheory;
                    grammarQuestions[currentTense] = curQs;
                    curQs.clear();
                    currentTense.clear();
                }
                state = OUT;
            } else if (tag.rfind("QUESTION", 0) == 0) {
                state = QUESTION;
                step = 0;
            } else {
                currentTense = tag;
                curTheory = Theory();
                state = THEORY;
            }
            continue;
        }

        if (state == THEORY) {
            size_t p = line.find(':');
            if (p != string::npos) {
                string key = line.substr(0, p);
                string val = line.substr(p + 1);
                while (!val.empty() && isspace(val.front())) val.erase(val.begin());
                if (key == "FORMULA_AFF") curTheory.formulaAff = val;
                else if (key == "FORMULA_NEG") curTheory.formulaNeg = val;
                else if (key == "FORMULA_Q") curTheory.formulaQ = val;
                else if (key == "SIGNALS") curTheory.signals = val;
                else if (key == "EXAMPLE") curTheory.example = val;
            }
        } else if (state == QUESTION) {
            if (step == 0) {
                curQ = Question();
                curQ.question = line;
                step = 1;
            } else if (step >= 1 && step <= 4) {
                curQ.options.push_back(line.substr(3));
                step++;
            } else if (line.rfind("ANSWER:", 0) == 0) {
                curQ.correct = stoi(line.substr(7));
                curQs.push_back(curQ);
                step = 0;
            }
        }
    }

    file.close();
    return true;
}

// ===============================
// HỌC NGỮ PHÁP 12 THÌ
// ===============================
void Dictionary::learnGrammars() {
    while (true) {
        int choice;
        cout << "\n=== HỌC NGỮ PHÁP 12 THÌ ===\n";
        cout << "1  Present Simple\n2  Present Continuous\n3  Present Perfect\n4  Present Perfect Continuous\n";
        cout << "5  Past Simple\n6  Past Continuous\n7  Past Perfect\n8  Past Perfect Continuous\n";
        cout << "9  Future Simple\n10 Future Continuous\n11 Future Perfect\n12 Future Perfect Continuous\n";
        cout << "0  Thoát\n";

        // --- Sửa lỗi nhập chữ ---
        while (true) {
            cout << "Chọn số (0-12): ";
            string line;
            getline(cin, line);
            try {
                choice = stoi(line);
                break;
            } catch (...) {
                cout << "Vui lòng nhập số hợp lệ.\n";
            }
        }

        if (choice == 0) break;

        string tense = getTenseName(choice);
        if (theories.find(tense) == theories.end()) {
            cout << "Không có dữ liệu lý thuyết.\n";
            continue;
        }
        showTheory(tense, theories[tense]);

        auto it = grammarQuestions.find(tense);
        if (it != grammarQuestions.end() && !it->second.empty())
            grammarQuiz(it->second);
        else
            cout << "Chưa có câu hỏi ôn tập cho thì này.\n";
    }
}

// ===============================
// HIỂN THỊ LÝ THUYẾT
// ===============================
void Dictionary::showTheory(const string& tense, const Theory& t) {
    cout << "\n===== " << tense << " =====\n";
    cout << "Công thức khẳng định: " << t.formulaAff << "\n";
    cout << "Công thức phủ định: " << t.formulaNeg << "\n";
    cout << "Công thức nghi vấn: " << t.formulaQ << "\n";
    cout << "Dấu hiệu nhận biết: " << t.signals << "\n";
    cout << "Ví dụ: " << t.example << "\n";
}

// ===============================
// 10 CÂU HỎI TRẮC NGHIỆM ÔN TẬP
// ===============================
void Dictionary::grammarQuiz(const vector<Question>& questions) {
    if (questions.empty()) return;

    vector<Question> qs = questions;
    shuffle(qs.begin(), qs.end(), mt19937(random_device{}()));
    int limit = min(10, (int)qs.size());
    int score = 0;

    cout << "\n--- LÀM 10 CÂU ÔN TẬP ---\n";
    for (int i = 0; i < limit; ++i) {
        cout << "\n" << i + 1 << ". " << qs[i].question << "\n";
        for (size_t j = 0; j < qs[i].options.size(); ++j)
            cout << "  " << j + 1 << ". " << qs[i].options[j] << "\n";

        // --- Sửa lỗi nhập chữ ---
        int ans = 0;
        while (true) {
            cout << "Chọn (1-4): ";
            string tmp; getline(cin, tmp);
            try {
                ans = stoi(tmp);
                if (ans >= 1 && ans <= 4) break;
            } catch (...) {}
            cout << "Vui lòng nhập số từ 1 đến 4.\n";
        }

        if (ans == qs[i].correct) {
            cout << "Đúng!\n";
            score++;
        } else {
            cout << "Sai. Đáp án đúng: " << qs[i].correct << ". "
                 << qs[i].options[qs[i].correct - 1] << "\n";
        }
    }
    cout << "\nKết quả: " << score << "/" << limit << " câu đúng.\n";
}

// ===============================
// HÀM ĐỌC FILE ÔN TẬP
// ===============================
bool Dictionary::loadPracticeFile(const string& filename) {
    ifstream f(filename);
    if (!f.is_open()) return false;

    string line;
    PracticeTest cur;
    enum State { OUT, IN_MCQ, IN_GAP, IN_REARR } state = OUT;

    auto trim = [](string &s) {
        while (!s.empty() && isspace((unsigned char)s.front())) s.erase(s.begin());
        while (!s.empty() && isspace((unsigned char)s.back())) s.pop_back();
    };

    while (getline(f, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        if (line.front() == '[' && line.back() == ']') {
            string tag = line.substr(1, line.size() - 2);
            if (tag == "END") {
                practiceTests.push_back(cur);
                cur = PracticeTest();
                state = OUT;
                continue;
            } else if (tag == "MCQ") { state = IN_MCQ; }
            else if (tag == "GAPFILL") { state = IN_GAP; }
            else if (tag == "REARRANGE") { state = IN_REARR; }
            else if (tag.rfind("TEST", 0) == 0) cur = PracticeTest();
            continue;
        }

        if (state == IN_MCQ) {
            static MCQ curq;
            static int step = 0;
            if (step == 0) {
                curq = MCQ();
                curq.question = line;
                step = 1;
            } else if (step >= 1 && step <= 4) {
                if (line.size() > 3 && line[1] == '.') curq.options.push_back(line.substr(3));
                else if (line.size() > 3) curq.options.push_back(line.substr(3));
                else curq.options.push_back(line);
                step++;
            } else if (line.rfind("ANSWER:", 0) == 0) {
                curq.answer = stoi(line.substr(7));
                cur.mcqs.push_back(curq);
                step = 0;
            }
        } else if (state == IN_GAP) {
            size_t p_eq = line.find('=');
            if (p_eq != string::npos) {
                GapFill g;
                string left = line.substr(0, p_eq);
                string right = line.substr(p_eq + 1);

                trim(left); trim(right);
                g.question = left;

                size_t p_bar = right.find('|');
                if (p_bar != string::npos) {
                    g.answer = right.substr(0, p_bar);
                    g.hint = right.substr(p_bar + 1);
                    trim(g.answer); trim(g.hint);
                } else {
                    size_t p_par = right.find('(');
                    if (p_par != string::npos) {
                        size_t p_par_end = right.find(')', p_par);
                        if (p_par_end != string::npos) {
                            g.answer = right.substr(0, p_par);
                            g.hint = right.substr(p_par + 1, p_par_end - p_par - 1);
                        } else {
                            g.answer = right.substr(0, p_par);
                            g.hint = right.substr(p_par + 1);
                        }
                        trim(g.answer); trim(g.hint);
                    } else {
                        g.answer = right;
                        trim(g.answer);
                        g.hint = "";
                    }
                }
                cur.gapfills.push_back(g);
            }
        } else if (state == IN_REARR) {
            size_t p = line.find('=');
            if (p != string::npos) {
                Rearrange r;
                string left = line.substr(0, p);
                string right = line.substr(p + 1);
                trim(left); trim(right);
                r.jumbled = left;
                r.correct = right;
                cur.rearranges.push_back(r);
            }
        }
    }

    f.close();
    return true;
}

// ===============================
// ÔN TẬP GRAMMAR TỔNG HỢP
// ===============================
void Dictionary::practiceTest() {
    if (practiceTests.empty()) {
        cout << "Không có dữ liệu ôn tập.\n";
        return;
    }
    random_device rd; mt19937 g(rd());
    uniform_int_distribution<int> dist(0, practiceTests.size() - 1);
    const PracticeTest& test = practiceTests[dist(g)];

    int score = 0, total = 0;
    cout << "\n=== ÔN TẬP GRAMMAR TỔNG HỢP ===\n";

    // --- MCQ ---
    for (auto& q : test.mcqs) {
        cout << "\n" << q.question << "\n";
        for (size_t i = 0; i < q.options.size(); ++i)
            cout << "  " << i + 1 << ". " << q.options[i] << "\n";

        int ans = 0;
        while (true) {
            cout << "Chọn (1-4): ";
            string tmp; getline(cin, tmp);
            try {
                ans = stoi(tmp);
                if (ans >= 1 && ans <= 4) break;
            } catch (...) {}
            cout << "Vui lòng nhập số từ 1 đến 4.\n";
        }

        if (ans == q.answer) { cout << "Đúng!\n"; score++; }
        else cout << "Sai! Đáp án đúng: " << q.answer << ". " << q.options[q.answer - 1] << "\n";
        total++;
    }

    // --- GAP FILL ---
    auto normalize = [](string s) {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        while (!s.empty() && isspace(s.front())) s.erase(s.begin());
        while (!s.empty() && isspace(s.back())) s.pop_back();
        return s;
    };

    for (auto& gq : test.gapfills) {
        cout << "\n" << gq.question << "\n→ ";
        string ans; getline(cin, ans);
        if (normalize(ans) == normalize(gq.answer)) {
            cout << "Chính xác!\n";
            score++;
        } else {
            cout << "Sai. Đáp án đúng: " << gq.answer << "\n";
        }
        total++;
    }

    // --- REARRANGE ---
    auto normalize2 = [](string s) {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
        return s;
    };

    for (auto& rq : test.rearranges) {
        cout << "\nSắp xếp câu: " << rq.jumbled << "\nNhập lại câu đúng: ";
        string ans; getline(cin, ans);
        if (normalize2(ans) == normalize2(rq.correct)) {
            cout << "Chính xác!\n";
            score++;
        } else {
            cout << "Sai. Đáp án đúng: " << rq.correct << "\n";
        }
        total++;
    }

    cout << "\nKết quả: " << score << "/" << total << " câu đúng.\n";
}

// ===============================
// TRỢ GIÚP HÀM LẤY TÊN THÌ
// ===============================
string Dictionary::getTenseName(int c) {
    switch (c) {
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

// ===============================
// HÀM MAIN
// ===============================
int main() {
    Dictionary dict;
    if (!dict.loadFromFile("grammar_data.txt")) {
        cerr << "Không đọc được file grammar_data.txt\n"; 
        return 1;
    }
    if (!dict.loadPracticeFile("practice_data.txt")) {
        cerr << "Không đọc được file practice_data.txt\n"; 
        return 1;
    }

    while (true) {
        cout << "\n========= MENU =========\n";
        cout << "1. Học 12 thì\n";
        cout << "2. Ôn tập Grammar tổng hợp\n";
        cout << "0. Thoát\n";

        int c;
        while (true) {
            cout << "Chọn: ";
            string line;
            getline(cin, line);
            try {
                c = stoi(line);
                break;
            } catch (...) {
                cout << "Vui lòng nhập số hợp lệ.\n";
            }
        }

        if (c == 1) dict.learnGrammars();
        else if (c == 2) dict.practiceTest();
        else if (c == 0) break;
        else cout << "Chọn sai!\n";
    }
    return 0;
}
