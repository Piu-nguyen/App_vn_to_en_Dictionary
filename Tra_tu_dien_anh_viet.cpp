#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <algorithm>
#include <random>
#include <fstream>
#include <map>
#include <sstream>
using namespace std;


// dua du lieu file txt vào phan chu de khoang 400 tu theo cau truc nhu tren(20 chu de)
// xuat danh sach tu da luu vao file ra mot file txt 

// ===============================
// Enum for IPA symbols
// ===============================
enum IPA {
    // Vowels (20)
    IPA_i, IPA_i_long, IPA_e, IPA_ae, IPA_a, IPA_a_long, IPA_o, IPA_o_long,
    IPA_u, IPA_u_long, IPA_uh, IPA_er,
    IPA_ai, IPA_au, IPA_oi, IPA_ei, IPA_ou, IPA_iu, IPA_ea, IPA_ua,

    // Consonants (24)
    IPA_p, IPA_b, IPA_t, IPA_d, IPA_k, IPA_g,
    IPA_f, IPA_v, IPA_th, IPA_dh, IPA_s, IPA_z,
    IPA_sh, IPA_zh, IPA_h, IPA_ch, IPA_j, IPA_m,
    IPA_n, IPA_ng, IPA_l, IPA_r, IPA_y, IPA_w
};


// ===============================
// Struct to store a word
// ===============================
struct Word {
    string english;     // English word
    string meaning;     // Meaning
    IPA ipa;            // IPA enum
    string example;     // Example sentence
    bool learned = false;   // Whether the word is learned
};

// ===============================
// Struct to store review info
// ===============================
struct ReviewWord {
    string word;
    string timeMarked;
};

// ===============================
// Union to support flexible searching
// ===============================
union SearchKey {
    const char* keyword;
    int hash;
};

// ===============================
// Base class Dictionary
// ===============================
class Dictionary {
protected:
    vector<Word> words;

    int hashString(const string& s) {
        int hash = 0;
        for (size_t i = 0; i < s.length(); ++i) {
            char c = s[i];
            hash = hash * 31 + tolower((unsigned char)c);
        }
        return hash;
    }

    string getIPAString(IPA ipa) const {
    switch (ipa) {
        case IPA_ae: return "/ae/";
        case IPA_i: return "/i/";
        case IPA_i_long: return "/i:/";
        case IPA_e: return "/e/";
        case IPA_a: return "/a/";
        case IPA_o: return "/o/";
        case IPA_u: return "/u/";
        case IPA_uh: return "/?/";
        case IPA_er: return "/?:/";
        case IPA_ai: return "/ai/";
        case IPA_au: return "/au/";
        case IPA_oi: return "/oi/";
        case IPA_ei: return "/ei/";
        case IPA_ou: return "/ou/";
        case IPA_sh: return "/?/";
        case IPA_th: return "/?/";
        case IPA_dh: return "/?/";
        case IPA_ch: return "/t?/";
        case IPA_j: return "/d?/";
        default: return "(unknown)";
    }
}


public:
    void addWord(const Word& w) {
        words.push_back(w);
    }

    void showAllWords() const {
        cout << "\n=== Word List ===\n";
        for (const auto& w : words) {
            cout << "Word: " << w.english
                 << "\nMeaning: " << w.meaning
                 << "\nIPA: " << getIPAString(w.ipa)
                 << "\nExample: " << w.example
                 << "\nStatus: " << (w.learned ? "Learned" : "Not learned")
                 << "\n---------------------\n";
        }
    }

    void searchWord(const string& keyword) const {
        cout << "\nSearching for: " << keyword << "\n";
        bool found = false;
        for (const auto& w : words) {
            if (w.english == keyword) {
                cout << "Found!\n";
                cout << "Meaning: " << w.meaning << "\n";
                cout << "IPA: " << getIPAString(w.ipa) << "\n";
                cout << "Example: " << w.example << "\n";
                found = true;
                break;
            }
        }
        if (!found) cout << "No word found.\n";
    }

    int countLearned() const {
        int count = 0;
        for (const auto& w : words)
            if (w.learned) count++;
        return count;
    }

    int totalWords() const {
        return words.size();
    }

    void showUnlearnedWords() const {
        cout << "\nUnlearned words:\n";
        for (const auto& w : words)
            if (!w.learned)
                cout << "- " << w.english << "\n";
    }

    void markWordLearned(const string& word) {
        for (auto& w : words)
            if (w.english == word)
                w.learned = true;
    }
};

// ===============================
// Class Topic inherits from Dictionary
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

public:
    Topic(const string& n) : name(n) {}

    string getName() const { return name; }
};

// ===============================
// Class VocabularyApp
// ===============================
class VocabularyApp {
private:
    vector<Topic> topics;
    vector<ReviewWord> reviewList;

public:
	void buildSampleData() {
    // ===== Topic 1: Animals =====
	    Topic animals("Animals");
	
	    Word w1;
	    w1.english = "cat";
	    w1.meaning = "a small domesticated animal";
	    w1.ipa = IPA_ae;  // /ae/ for "cat"
	    w1.example = "The cat is sleeping on the sofa.";
	    w1.learned = false;
	    animals.addWord(w1);
	
	    Word w2;
	    w2.english = "dog";
	    w2.meaning = "a loyal animal often kept as a pet";
	    w2.ipa = IPA_o;   // /o/ for "dog"
	    w2.example = "My dog barks at strangers.";
	    w2.learned = false;
	    animals.addWord(w2);
	
	    // ===== Topic 2: Fruits =====
	    Topic fruits("Fruits");
	
	    Word w3;
	    w3.english = "apple";
	    w3.meaning = "a sweet red or green fruit";
	    w3.ipa = IPA_ae;  // /ae/ for "apple"
	    w3.example = "I eat an apple every morning.";
	    w3.learned = false;
	    fruits.addWord(w3);
	
	    Word w4;
	    w4.english = "orange";
	    w4.meaning = "a citrus fruit rich in vitamin C";
	    w4.ipa = IPA_o;  // /o/ for "orange"
	    w4.example = "He peeled an orange for breakfast.";
	    w4.learned = false;
	    fruits.addWord(w4);
	
	    topics.push_back(animals);
	    topics.push_back(fruits);
}


    void showTopics() const {
        cout << "\n=== Topics ===\n";
        for (size_t i = 0; i < topics.size(); ++i)
            cout << i + 1 << ". " << topics[i].getName() << "\n";
    }

    void viewTopicWords() {
        showTopics();
        cout << "\nEnter topic number: ";
        int choice;
        cin >> choice;
        if (choice < 1 || choice > (int)topics.size()) {
            cout << "Invalid topic number.\n";
            return;
        }
        topics[choice - 1].showAllWords();
    }

    void showProgress() {
        int total = 0, learned = 0;
        for (const auto& t : topics) {
            total += t.totalWords();
            learned += t.countLearned();
        }
        double percent = total == 0 ? 0.0 : (double)learned / total * 100;
        cout << "\nProgress:\n";
        cout << "Learned words: " << learned << "/" << total << " (" << fixed << setprecision(1) << percent << "%)\n";
        cout << "Words not learned yet:\n";
        for (const auto& t : topics)
            t.showUnlearnedWords();
    }

    void markForReview() {
        cout << "\nEnter word to mark for review: ";
        string word;
        cin >> word;
        time_t now = time(nullptr);
        string timeStr = ctime(&now);
        timeStr.pop_back(); // remove '\n'
        reviewList.push_back({word, timeStr});
        cout << "Word marked for review at " << timeStr << "\n";
    }

    void suggestNextTopic() {
        cout << "\nSuggested next topic: ";
        if (!topics.empty())
            cout << topics[rand() % topics.size()].getName() << "\n";
        else
            cout << "No topics available.\n";
    }

    void menu() {
        int choice;
        do {
            cout << "\n====== Vocabulary App ======\n";
            cout << "1. Show topics\n";
            cout << "2. View words in topic\n";
            cout << "3. Show progress\n";
            cout << "4. Mark word for review\n";
            cout << "5. Suggest next topic\n";
            cout << "0. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;
			
			
			
			cin.ignore(10000, '\n');
            switch (choice) {
                case 1: showTopics(); break;
                case 2: viewTopicWords(); break;
                case 3: showProgress(); break;
                case 4: markForReview(); break;
                case 5: suggestNextTopic(); break;
                case 0: cout << "Goodbye!\n"; break;
                default: cout << "Invalid option.\n";
            }
        } while (choice != 0);
    }
};

// ===============================
// Main function
// ===============================
int main() {
    srand((unsigned)time(0));
    VocabularyApp app;
    app.buildSampleData();
    app.menu();
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
