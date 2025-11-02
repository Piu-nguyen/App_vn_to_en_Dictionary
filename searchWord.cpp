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
    string topic;
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
// Hàm chuẩn hoá chuỗi (bỏ dấu, chuyển thường)
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
// Xử lý IPA
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
// Class Dictionary
// ===============================
class Dictionary {
private:
    vector<Word> wordList;

public:
    bool loadFromFile(const string& fileName);
    Word* searchEnglishToVietnamese(const string& keyWord);
    Word* searchVietnameseToEnglish(const string& keyWord);
    vector<string> suggestWords(const string& keyWord, bool isEnglish);
};

// ===============================
// Load dữ liệu
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

        // Xử lý BOM nếu có
        if (line.size() >= 3 &&
    (unsigned char)line[0] == 0xEF &&
    (unsigned char)line[1] == 0xBB &&
    (unsigned char)line[2] == 0xBF)
    line = line.substr(3);


        vector<string> parts = split(line, '|');
        if (parts.size() < 5) continue;

        Word w;
        w.english = trim(parts[0]);
        w.vietnamese = trim(parts[1]);
        w.ipaText = trim(parts[2]);
        w.example = trim(parts[3]);
        w.topic = trim(parts[4]);
        w.ipa = getIpaFromText(w.ipaText);

        wordList.push_back(w);
    }

    file.close();
    cout << "Đã nạp " << wordList.size() << " từ vào từ điển.\n";
    return true;
}

// ===============================
// Tìm kiếm
// ===============================
Word* Dictionary::searchEnglishToVietnamese(const string& keyWord) {
    string key = normalize(trim(keyWord));
    for (size_t i = 0; i < wordList.size(); ++i) {
        if (normalize(wordList[i].english) == key)
            return &wordList[i];
    }
    return NULL;
}

Word* Dictionary::searchVietnameseToEnglish(const string& keyWord) {
    string key = normalize(trim(keyWord));
    for (size_t i = 0; i < wordList.size(); ++i) {
        if (normalize(wordList[i].vietnamese) == key)
            return &wordList[i];
    }
    return NULL;
}

// ===============================
// Gợi ý từ gần giống
// ===============================
vector<string> Dictionary::suggestWords(const string& keyWord, bool isEnglish) {
    string key = normalize(keyWord);
    vector<string> suggestions;
    for (size_t i = 0; i < wordList.size(); ++i) {
        string word = isEnglish ? normalize(wordList[i].english) : normalize(wordList[i].vietnamese);
        if (word.find(key) != string::npos) {
            suggestions.push_back(isEnglish ? wordList[i].english : wordList[i].vietnamese);
        }
    }
    return suggestions;
}

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

    cout << "\n===== English - Vietnamese Dictionary =====\n";

    while (true) {
        cout << "\nChọn chế độ:\n";
        cout << "1. English -> Vietnamese\n";
        cout << "2. Vietnamese -> English\n";
        cout << "0. Thoát\n";

        int choice;

        // vòng lặp kiểm tra nhập số hợp lệ
        while (true) {
            cout << "Lựa chọn của bạn: ";
            if (cin >> choice && (choice == 0 || choice == 1 || choice == 2)) {
                // bỏ phần còn lại trên dòng
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            } else {
                cout << "Vui lòng nhập số hợp lệ (0, 1 hoặc 2).\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        string input;
        Word* result = NULL;

        switch (choice) {
    case 1: {
        cout << "\nNhập từ cần tra (English): ";
        getline(cin, input);
        input = trim(input);
        if (input.empty()) {
            cout << "Bạn chưa nhập từ nào.\n";
            break;
        }
        result = dict.searchEnglishToVietnamese(input);
        if (result != NULL) {
            cout << "\nKết quả tìm kiếm:\n";
            cout << "English   : " << result->english << "\n";
            cout << "Vietnamese: " << result->vietnamese << "\n";
            cout << "IPA (enum): " << ipaToString(result->ipa) << "\n";
            cout << "IPA (text): " << result->ipaText << "\n";
            cout << "Example   : " << result->example << "\n";
            cout << "Topic     : " << result->topic << "\n";
        } else {
            cout << "Không tìm thấy từ \"" << input << "\".\n";
            auto s = dict.suggestWords(input, true);
            if (!s.empty()) {
                cout << "Gợi ý: ";
                for (size_t i = 0; i < s.size(); ++i)
                    cout << s[i] << (i < s.size() - 1 ? ", " : "\n");
            }
        }

        //  Hỏi người dùng có muốn tra tiếp không
        char again;
        cout << "\nTra từ khác? ('y' là đồng ý /'n' là không): ";
        cin >> again;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (again == 'n' || again == 'N') {
            cout << "Cảm ơn bạn đã sử dụng từ điển!\n";
            return 0;
        }

        break;
    }

    case 2: {
        cout << "\nNhập từ cần tra (Vietnamese): ";
        getline(cin, input);
        input = trim(input);
        if (input.empty()) {
            cout << "Bạn chưa nhập từ nào.\n";
            break;
        }
        result = dict.searchVietnameseToEnglish(input);
        if (result != NULL) {
            cout << "\nKết quả tìm kiếm:\n";
            cout << "English   : " << result->english << "\n";
            cout << "Vietnamese: " << result->vietnamese << "\n";
            cout << "IPA (enum): " << ipaToString(result->ipa) << "\n";
            cout << "IPA (text): " << result->ipaText << "\n";
            cout << "Example   : " << result->example << "\n";
            cout << "Topic     : " << result->topic << "\n";
        } else {
            cout << "Không tìm thấy từ \"" << input << "\".\n";
            auto s = dict.suggestWords(input, false);
            if (!s.empty()) {
                cout << "Gợi ý: ";
                for (size_t i = 0; i < s.size(); ++i)
                    cout << s[i] << (i < s.size() - 1 ? ", " : "\n");
            }
        }

        //  Hỏi người dùng có muốn tra tiếp không
        char again;
        cout << "\nTra từ khác? ('y' là đồng ý /'n' là không): ";
        cin >> again;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (again == 'n' || again == 'N') {
            cout << "Cảm ơn bạn đã sử dụng từ điển!\n";
            return 0;
        }

        break;
    }

    case 0: {
        cout << "Cảm ơn bạn đã sử dụng từ điển!\n";
        return 0;
    }
}

}
return 0;
}