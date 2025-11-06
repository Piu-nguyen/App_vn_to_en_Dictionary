#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

using namespace std;

// ===============================
// Enum cho âm IPA
// ===============================
enum Ipa {
    ipaAe, ipaO, ipaU, ipaE, ipaI, ipaUnknown
};

// ===============================
// Struct chứa thông tin một từ
// ===============================
struct Word {
    string english;
    string vietnamese;
    Ipa ipa;
    string ipaText;
    string example;
};

// ===============================
// Node cho cây BST
// ===============================
struct Node {
    Word data;
    Node* left;
    Node* right;
    Node(const Word& w) : data(w), left(NULL), right(NULL) {}
};

// ===============================
// Hàm loại khoảng trắng thừa
// ===============================
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == string::npos) return "";
    return s.substr(start, end - start + 1);
}

// ===============================
// Hàm tiện ích tách chuỗi
// ===============================
vector<string> split(const string& line, char delimiter) {
    vector<string> result;
    string part;
    stringstream ss(line);
    while (getline(ss, part, delimiter)) {
        result.push_back(trim(part));
    }
    return result;
}

// ===============================
// Hàm chuẩn hoá chuỗi
// ===============================
string normalize(const string& input) {
    static map<char, char> accentMap = {
        {'À','A'},{'Á','A'},{'Â','A'},{'Ã','A'},{'Ạ','A'},{'Ă','A'},{'Ắ','A'},{'Ằ','A'},{'Ặ','A'},
        {'à','a'},{'á','a'},{'â','a'},{'ã','a'},{'ạ','a'},{'ă','a'},{'ắ','a'},{'ằ','a'},{'ặ','a'},
        {'È','E'},{'É','E'},{'Ẹ','E'},{'Ẻ','E'},{'Ẽ','E'},{'Ê','E'},{'Ế','E'},{'Ề','E'},{'Ệ','E'},
        {'è','e'},{'é','e'},{'ẹ','e'},{'ẻ','e'},{'ẽ','e'},{'ê','e'},{'ế','e'},{'ề','e'},{'ệ','e'},
        {'Ì','I'},{'Í','I'},{'Ị','I'},{'Ỉ','I'},{'Ĩ','I'},
        {'ì','i'},{'í','i'},{'ị','i'},{'ỉ','i'},{'ĩ','i'},
        {'Ò','O'},{'Ó','O'},{'Ọ','O'},{'Ỏ','O'},{'Õ','O'},{'Ô','O'},{'Ố','O'},{'Ồ','O'},{'Ộ','O'},{'Ơ','O'},{'Ớ','O'},{'Ờ','O'},{'Ợ','O'},
        {'ò','o'},{'ó','o'},{'ọ','o'},{'ỏ','o'},{'õ','o'},{'ô','o'},{'ố','o'},{'ồ','o'},{'ộ','o'},{'ơ','o'},{'ớ','o'},{'ờ','o'},{'ợ','o'},
        {'Ù','U'},{'Ú','U'},{'Ụ','U'},{'Ủ','U'},{'Ũ','U'},{'Ư','U'},{'Ứ','U'},{'Ừ','U'},{'Ự','U'},
        {'ù','u'},{'ú','u'},{'ụ','u'},{'ủ','u'},{'ũ','u'},{'ư','u'},{'ứ','u'},{'ừ','u'},{'ự','u'},
        {'Ỳ','Y'},{'Ý','Y'},{'Ỵ','Y'},{'Ỷ','Y'},{'Ỹ','Y'},
        {'ỳ','y'},{'ý','y'},{'ỵ','y'},{'ỷ','y'},{'ỹ','y'},
        {'Đ','D'},{'đ','d'}
    };

    string result;
    for (unsigned char ch : input) {
        if (accentMap.find(ch) != accentMap.end())
            result += accentMap[ch];
        else
            result += ch;
    }

    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// ===============================
// IPA
// ===============================
Ipa getIpaFromText(const string& ipaStr) {
    if (ipaStr.find("æ") != string::npos) return ipaAe;
    if (ipaStr.find("ɒ") != string::npos) return ipaO;
    if (ipaStr.find("ʌ") != string::npos) return ipaU;
    if (ipaStr.find("e") != string::npos) return ipaE;
    if (ipaStr.find("ɪ") != string::npos) return ipaI;
    return ipaUnknown;
}

string ipaToString(Ipa ipa) {
    switch (ipa) {
        case ipaAe: return "/æ/";
        case ipaO:  return "/ɒ/";
        case ipaU:  return "/ʌ/";
        case ipaE:  return "/e/";
        case ipaI:  return "/ɪ/";
        default:    return "(unknown)";
    }
}

// ===============================
// Class Dictionary (BST)
// ===============================
class Dictionary {
private:
    Node* rootEng;
    Node* rootVie;
    vector<Word> wordList;

    Node* insert(Node* root, const Word& w, bool isEnglish) {
        string key = normalize(isEnglish ? w.english : w.vietnamese);
        if (root == NULL)
            return new Node(w);

        string rootKey = normalize(isEnglish ? root->data.english : root->data.vietnamese);
        if (key < rootKey)
            root->left = insert(root->left, w, isEnglish);
        else if (key > rootKey)
            root->right = insert(root->right, w, isEnglish);
        return root;
    }

    Word* search(Node* root, const string& key, bool isEnglish) {
        if (root == NULL) return NULL;
        string rootKey = normalize(isEnglish ? root->data.english : root->data.vietnamese);
        if (key == rootKey)
            return &root->data;
        else if (key < rootKey)
            return search(root->left, key, isEnglish);
        else
            return search(root->right, key, isEnglish);
    }

    void suggest(Node* root, const string& key, bool isEnglish, vector<string>& result, int limit) {
        if (root == NULL || (int)result.size() >= limit) return;
        string word = normalize(isEnglish ? root->data.english : root->data.vietnamese);
        if (word.find(key) != string::npos)
            result.push_back(isEnglish ? root->data.english : root->data.vietnamese);
        suggest(root->left, key, isEnglish, result, limit);
        suggest(root->right, key, isEnglish, result, limit);
    }

    void clear(Node* root) {
        if (root == NULL) return;
        clear(root->left);
        clear(root->right);
        delete root;
    }

public:
    Dictionary() {
        rootEng = NULL;
        rootVie = NULL;
    }

    ~Dictionary() {
        clear(rootEng);
        clear(rootVie);
    }

    bool loadFromFile(const string& fileName);
    Word* searchEnglishToVietnamese(const string& keyWord);
    Word* searchVietnameseToEnglish(const string& keyWord);
    vector<string> suggestWords(const string& keyWord, bool isEnglish);
};

// ===============================
// Load dữ liệu từ file
// ===============================
bool Dictionary::loadFromFile(const string& fileName) {
    ifstream file(fileName.c_str());
    if (!file.is_open()) {
        cerr << "Không thể mở file dữ liệu: " << fileName << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        // Xử lý BOM UTF-8 nếu có
        if (line.size() >= 3 &&
            (unsigned char)line[0] == 0xEF &&
            (unsigned char)line[1] == 0xBB &&
            (unsigned char)line[2] == 0xBF)
            line = line.substr(3);

        vector<string> parts = split(line, '|');
        if (parts.size() < 4) continue;

        Word w;
        w.english = trim(parts[0]);
        w.vietnamese = trim(parts[1]);
        w.ipaText = trim(parts[2]);
        w.example = trim(parts[3]);
        w.ipa = getIpaFromText(w.ipaText);

        wordList.push_back(w);
        rootEng = insert(rootEng, w, true);
        rootVie = insert(rootVie, w, false);
    }

    file.close();
    cout << "Đã nạp " << wordList.size() << " từ vào cây BST.\n";
    return true;
}

// ===============================
// Tìm kiếm BST
// ===============================
Word* Dictionary::searchEnglishToVietnamese(const string& keyWord) {
    string key = normalize(trim(keyWord));
    return search(rootEng, key, true);
}

Word* Dictionary::searchVietnameseToEnglish(const string& keyWord) {
    string key = normalize(trim(keyWord));
    return search(rootVie, key, false);
}

// ===============================
// Gợi ý
// ===============================
vector<string> Dictionary::suggestWords(const string& keyWord, bool isEnglish) {
    string key = normalize(keyWord);
    vector<string> suggestions;
    suggest(isEnglish ? rootEng : rootVie, key, isEnglish, suggestions, 10);
    return suggestions;
}

// ===============================
// Voice (Windows)
// ===============================
#ifdef _WIN32
void speakVoice(const string& text) {
    string safeText = text;
    replace(safeText.begin(), safeText.end(), '\'', ' ');
    string command =
        "powershell -Command \"" 
        "Add-Type -AssemblyName System.Speech; "
        "$speak = New-Object System.Speech.Synthesis.SpeechSynthesizer; "
        "$speak.SelectVoice('Microsoft Zira Desktop'); "
        "$speak.Speak('" + safeText + "');\"";
    system(command.c_str());
}
#endif

// === NEW: History feature ===
void saveHistory(const string& word) {
    ofstream f("history.txt", ios::app);
    if (f.is_open()) {
        f << word << endl;
        f.close();
    }
}

vector<string> loadHistory(int limit = 5) {
    vector<string> history;
    ifstream f("history.txt");
    if (!f.is_open()) return history;

    string line;
    vector<string> all;
    while (getline(f, line)) {
        line = trim(line);
        if (!line.empty()) all.push_back(line);
    }
    f.close();

    int start = max(0, (int)all.size() - limit);
    for (int i = start; i < (int)all.size(); ++i)
        history.push_back(all[i]);

    return history;
}
// === END NEW ===

// ===============================
// MAIN
// ===============================
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    Dictionary dict;
    if (!dict.loadFromFile("data.txt")) {
        cout << "Vui lòng đặt file data.txt cùng thư mục với chương trình.\n";
        return 1;
    }

    // === NEW: Hiển thị lịch sử ===
    auto history = loadHistory();
    if (!history.empty()) {
        cout << "\n===== Lịch sử tra gần đây =====\n";
        for (int i = (int)history.size() - 1; i >= 0; --i)
            cout << "- " << history[i] << endl;
        cout << "================================\n";
    }
    // === END NEW ===

    int choice = -1;
    while (true) {
        cout << "\n===== English - Vietnamese Dictionary =====\n";
        cout << "1. English -> Vietnamese\n";
        cout << "2. Vietnamese -> English\n";
        cout << "0. Thoát\n";

        string inputChoice;
        while (true) {
            cout << "Lựa chọn của bạn: ";
            cin >> inputChoice;
            bool isNumber = all_of(inputChoice.begin(), inputChoice.end(), ::isdigit);
            if (isNumber) {
                choice = stoi(inputChoice);
                if (choice >= 0 && choice <= 2) break;
            }
            cout << "Lỗi: chỉ được nhập 0, 1 hoặc 2. Nhập lại!\n";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                string input;
                cout << "\nNhập từ (English): ";
                getline(cin, input);

                Word* result = dict.searchEnglishToVietnamese(input);
                if (result != NULL) {
                    // === NEW: lưu lịch sử ===
                    saveHistory(result->english);
                    // === END NEW ===

                    cout << "\nKết quả:\n";
                    cout << "English   : " << result->english << "\n";
                    cout << "Vietnamese: " << result->vietnamese << "\n";
                    cout << "IPA       : " << result->ipaText << "\n";
                    cout << "Example   : " << result->example << "\n";
#ifdef _WIN32
                    int voiceChoice;
                    string voiceInput;

                    cout << "\nPhát âm:\n";
                    cout << "1. Voice tiếng Anh\n";
                    cout << "0. Bỏ qua\n";

                    while (true) {
                        cout << "Chọn: ";
                        cin >> voiceInput;

                        bool isNumber = all_of(voiceInput.begin(), voiceInput.end(), ::isdigit);
                        if (isNumber) {
                            voiceChoice = stoi(voiceInput);
                            if (voiceChoice == 0 || voiceChoice == 1) break;
                        }

                        cout << "Nhập sai! Chỉ được nhập số 0 hoặc 1.\n";
                        cin.clear();
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (voiceChoice == 1) {
                        speakVoice(result->english);

                        char repeat;
                        while (true) {
                            cout << "\nLặp lại phát âm liên tục? (y/n): ";
                            cin >> repeat;
                            repeat = tolower(repeat);

                            if (repeat == 'y' || repeat == 'n') break;
                            cout << "Lỗi: chỉ được nhập 'y' hoặc 'n'. Nhập lại!\n";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        if (repeat == 'y') {
                            cout << "\nĐang phát lại... (Nhấn Enter để dừng)\n";
                            while (true) {
                                if (_kbhit() && _getch() == '\r') break;
                                speakVoice(result->english);
                            }
                        }
                    } else {
                        cout << "Bỏ qua phát âm.\n";
                    }
#endif
                } else {
                    cout << "Không tìm thấy từ \"" << input << "\".\n";
                    auto s = dict.suggestWords(input, true);
                    if (!s.empty()) {
                        cout << "Gợi ý: ";
                        for (size_t i = 0; i < s.size(); ++i)
                            cout << s[i] << (i < s.size() - 1 ? ", " : "\n");
                    }
                }
                break;
            }

            case 2: {
                string input;
                cout << "\nNhập từ (Vietnamese): ";
                getline(cin, input);

                Word* result = dict.searchVietnameseToEnglish(input);
                if (result != NULL) {
                    cout << "\nKết quả:\n";
                    cout << "English   : " << result->english << "\n";
                    cout << "Vietnamese: " << result->vietnamese << "\n";
                    cout << "IPA       : " << result->ipaText << "\n";
                    cout << "Example   : " << result->example << "\n";

#ifdef _WIN32
                    int voiceChoice;
                    string voiceInput;

                    cout << "\nPhát âm:\n";
                    cout << "1. Voice tiếng Anh\n";
                    cout << "0. Bỏ qua\n";

                    while (true) {
                        cout << "Chọn: ";
                        cin >> voiceInput;

                        bool isNumber = all_of(voiceInput.begin(), voiceInput.end(), ::isdigit);
                        if (isNumber) {
                            voiceChoice = stoi(voiceInput);
                            if (voiceChoice == 0 || voiceChoice == 1) break;
                        }

                        cout << "Nhập sai! Chỉ được nhập số 0 hoặc 1.\n";
                        cin.clear();
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (voiceChoice == 1) {
                        speakVoice(result->english);

                        char repeat;
                        while (true) {
                            cout << "\nLặp lại phát âm liên tục? (y/n): ";
                            cin >> repeat;
                            repeat = tolower(repeat);

                            if (repeat == 'y' || repeat == 'n') break;
                            cout << "Lỗi: chỉ được nhập 'y' hoặc 'n'. Nhập lại!\n";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        if (repeat == 'y') {
                            cout << "\nĐang phát lại... (Nhấn Enter để dừng)\n";
                            while (true) {
                                if (_kbhit() && _getch() == '\r') break;
                                speakVoice(result->english);
                            }
                        }
                    } else {
                        cout << "Bỏ qua phát âm.\n";
                    }
#endif
                } else {
                    cout << "Không tìm thấy từ \"" << input << "\".\n";
                    auto s = dict.suggestWords(input, false);
                    if (!s.empty()) {
                        cout << "Gợi ý: ";
                        for (size_t i = 0; i < s.size(); ++i)
                            cout << s[i] << (i < s.size() - 1 ? ", " : "\n");
                    }
                }
                break;
            }

            case 0:
                cout << "Cảm ơn bạn đã sử dụng từ điển!\n";
                return 0;

            default:
                cout << "Lựa chọn không hợp lệ!\n";
                break;
        }
    }

    return 0;
}
