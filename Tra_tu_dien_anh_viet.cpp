#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

// ===============================
// ?? Enum cho �m IPA
// ===============================
enum IPA {
    IPA_ae,
    //.....
};

// ===============================
// ?? Struct ch?a th�ng tin m?t t?
// ===============================
struct Word {
    string engLish;        // English word
    string vietNam;        // Vietnamese meaning
    IPA ipa;          // IPA symbol
    string example;   // Example sentence
};

// ===============================
// ?? Union h? tr? t�m ki?m
// ===============================
union SearchKey {
    const char* keyWord;
    int hash;
};

// ===============================
// ?? Class Dictionary (cha)
// ===============================
class Dictionary {
protected:
    vector<Word> words;

    // H�m t?o hash t? string
    int hashString(const string& s)
{
    int hash = 0;
    for (size_t i = 0; i < s.length(); ++i)
    {
        char c = s[i];
        hash = hash * 31 + tolower((unsigned char)c);
    }
    return hash;
}
	
	string getIPAString(IPA ipa) const {
        switch (ipa){
            case IPA_ae: return "/'�p.?l/";
            //.....
            default: return "(unknown)";
        }
    };
    public:
};

// ===============================
// ?? Class Topic k? th?a Dictionary
// ===============================
class Topic : public Dictionary {
private:
    string name;

public:
};

// ===============================
// ?? Class Account (��ng nh?p)
// ===============================
class Account {
private:
    string username;
    string password;

public:
    
};
