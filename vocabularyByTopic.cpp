#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream>    
#include <dirent.h>
using namespace std;

// trong phan chon muc so th́ phai bat nhap lai khi nhap chu
// chuc nang so 1 se bo vi no bi thua 
// su ly phan IPA
// show tien do lï¿½ chuc nang them(chinh sua lai phan trang thai)
// dua du lieu file txt vï¿½o phan chu de khoang 400 tu theo cau truc nhu tren(20 chu de)
// xuat danh sach tu da luu vao file ra mot file txt(su ly luu cĂ¹ng má»™t tá»« 2 láº§n)
// su ly static khi chon vo lï¿½ thanh da hoc

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

public:
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
	const vector<Word>& getWords() const {
        return words;
    }
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
	    string folderPath = "D:\\List of subjects by section\\Object-oriented programming\\largeExercise\\Data";

	    topics.clear();              // Clear existing topics first
	
	    DIR *dir;
	    struct dirent *ent;
	
	    // Try to open directory
	    if ((dir = opendir(folderPath.c_str())) != NULL) {
	        while ((ent = readdir(dir)) != NULL) {
	            string fileName = ent->d_name;
	
	            // Skip "." and ".." and only process CSV files
	            if (fileName.find(".csv") != string::npos) {
	                string topicName = fileName.substr(0, fileName.find(".csv"));
	                Topic topic(topicName);
	
	                string fullPath = folderPath + "/" + fileName;
	                ifstream file(fullPath.c_str());  // use .c_str() for Dev-C++
	                if (!file.is_open()) {
	                    cout << "Cannot open file: " << fullPath << endl;
	                    continue;
	                }
	
	                string line;
	                getline(file, line); // skip header line
	
	                while (getline(file, line)) {
	                    stringstream ss(line);
	                    string english, meaning, ipaStr, example;
	
	                    getline(ss, english, ',');
	                    getline(ss, meaning, ',');
	                    getline(ss, ipaStr, ',');
	                    getline(ss, example);
	
	                    Word w;
	                    w.english = english;
	                    w.meaning = meaning;
	                    w.ipa = IPA_i;
	                    w.example = example;
	                    w.learned = false;
	
	                    topic.addWord(w);
	                }
	
	                topics.push_back(topic);
	                file.close();
	            }
	        }
	        closedir(dir);
	    } else {
	        cout << "Failed to open data folder!" << endl;
	    }
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

        Topic& selected = topics[choice - 1];
        const vector<Word>& words = selected.getWords();

        if (words.empty()) {
            cout << "No words in this topic.\n";
            return;
        }

        cout << "\n=== Words in topic: " << selected.getName() << " ===\n";
        for (size_t i = 0; i < words.size(); ++i)
            cout << i + 1 << ". " << words[i].english << "\n";

        cout << "\nEnter word number to view details: ";
        int wordChoice;
        cin >> wordChoice;
        if (wordChoice < 1 || wordChoice > (int)words.size()) {
            cout << "Invalid word number.\n";
            return;
        }

        const Word& w = words[wordChoice - 1];
        cout << "\n=== Word Details ===\n";
		cout<< "\nWord: " << w.english
		    << "\nMeaning: " << w.meaning
		    << "\nIPA: " << selected.getIPAString(w.ipa)
		    << "\nExample: " << w.example
		    << "\nStatus: " << (w.learned ? "Learned" : "Not learned")
		    << "\n---------------------\n";
    }

 	void showProgress() {
        int total = 0, learned = 0;
        for (const auto& t : topics) {
            total += t.totalWords();
            learned += t.countLearned();
        }

        double overallPercent = total == 0 ? 0.0 : (double)learned / total * 100;
        cout << "\n=== Overall Progress ===\n";
        cout << "Learned words: " << learned << "/" << total
             << " (" << fixed << setprecision(1) << overallPercent << "%)\n";
    }

	void markForReview() {
	    cout << "\nEnter word to mark for review: ";
	    string word;
	    cin >> word;
	
	    // Get current time
	    time_t now = time(nullptr);
	    string timeStr = ctime(&now);
	    if (!timeStr.empty() && timeStr.back() == '\n') timeStr.pop_back();
	
	    string meaning = "N/A";
	    string ipaStr = "N/A";
	    string example = "N/A";
	    bool found = false;
	
	    // Search for the word in all topics
	    for (const auto& topic : topics) {
	        for (const auto& w : topic.getWords()) {
	            if (w.english == word) {
	                meaning = w.meaning;
	                ipaStr = topic.getIPAString(w.ipa);
	                example = w.example;
	                found = true;
	                break;
	            }
	        }
	        if (found) break;
	    }
	
	    if (!found) {
	        cout << "Word not found in any topic.\n";
	        return;
	    }
	
	    string filePath = "ReviewList.csv";
	
	    // CHECK IF WORD ALREADY EXISTS IN CSV FILE
	    bool wordExists = false;
	    ifstream checkFile(filePath);
	    if (checkFile.is_open()) {
	        string line;
	        // Skip header
	        if (getline(checkFile, line)) {
	            while (getline(checkFile, line)) {
	                if (!line.empty()) {
	                    // Extract word from CSV line (second column)
	                    size_t firstComma = line.find(',');
	                    if (firstComma != string::npos) {
	                        size_t secondComma = line.find(',', firstComma + 1);
	                        if (secondComma != string::npos) {
	                            string existingWord = line.substr(firstComma + 1, secondComma - firstComma - 1);
	                            // Remove extra spaces
	                            existingWord.erase(0, existingWord.find_first_not_of(" \t"));
	                            existingWord.erase(existingWord.find_last_not_of(" \t") + 1);
	                            
	                            if (existingWord == word) {
	                                wordExists = true;
	                                break;
	                            }
	                        }
	                    }
	                }
	            }
	        }
	        checkFile.close();
	    }
	
	    // PREVENT DUPLICATE SAVING
	    if (wordExists) {
	        cout << "This word '" << word << "' is already in your review list.\n";
	        cout << "Recommendation: You can review it in your existing list instead of adding it again.\n";
	        return;
	    }
	
	    // Check if file exists
	    bool fileExists = false;
	    ifstream checkFile2(filePath);
	    if (checkFile2.is_open()) {
	        fileExists = true;
	        checkFile2.close();
	    }
	
	    // Count existing lines for index
	    int index = 1;
	    ifstream fin(filePath);
	    if (fin.is_open()) {
	        string line;
	        if (getline(fin, line)) {
	            while (getline(fin, line)) {
	                if (!line.empty()) index++;
	            }
	        }
	        fin.close();
	    }
	
	    // Open file for appending
	    ofstream fout(filePath, ios::app);
	    if (!fout.is_open()) {
	        cout << "Error: Cannot create or open ReviewList.csv\n";
	        return;
	    }
	
	    // Write header if new file with width adjustment
	    if (!fileExists) {
	        fout << setw(4) << "No" << ","
	             << setw(15) << left << "Word" << ","
	             << setw(20) << left << "Meaning" << ","
	             << setw(10) << left << "IPA" << ","
	             << setw(50) << left << "Example" << ","  // Reduced to reasonable width
	             << setw(25) << left << "Time" << "\n";
	        index = 1;
	    }
	
	    // Function to escape and limit length
	    auto formatField = [](const string& s, int maxLength = 0) {
	        string r = s;
	        // Replace commas to avoid breaking CSV structure
	        for (auto& ch : r) if (ch == ',') ch = ';';
	        
	        // Limit length if needed
	        if (maxLength > 0 && r.length() > maxLength) {
	            r = r.substr(0, maxLength - 3) + "...";
	        }
	        return r;
	    };
	
	    // Write data with width adjustment
	    fout << setw(4) << index << ","
	         << setw(15) << left << formatField(word, 15) << ","
	         << setw(20) << left << formatField(meaning, 20) << ","
	         << setw(10) << left << formatField(ipaStr, 10) << ","
	         << setw(50) << left << formatField(example, 50) << ","  // Example width 50 characters
	         << setw(25) << left << formatField(timeStr, 25) << "\n";
	
	    fout.close();
	
	    cout << "SUCCESS: Word '" << word << "' has been added to your review list.\n";
	    cout << "File saved as: ReviewList.csv in your program directory.\n";
	    cout << "Recommendation: Review these words regularly for better retention.\n";
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
            cout << "1. View words in topic\n";
            cout << "2. Show progress\n";
            cout << "3. Mark word for review\n";
            cout << "4. Suggest next topic\n";
            cout << "0. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;
			
			
			
			cin.ignore(10000, '\n');
            switch (choice) {
                case 1: viewTopicWords(); break;
                case 2: showProgress(); break;
                case 3: markForReview(); break;
                case 4: suggestNextTopic(); break;
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
