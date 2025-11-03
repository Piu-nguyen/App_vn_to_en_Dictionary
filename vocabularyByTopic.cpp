#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <algorithm>
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
// ===============================
class Topic : public Dictionary {
private:
    string name;

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
    return 0;
}
