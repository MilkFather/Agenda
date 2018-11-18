#include "CSV.hpp"
#include <fstream>

using std::ifstream;
using std::ofstream;
using std::endl;

CSVRow CSV::parseLine(string l) {
    CSVRow v = split(l, ',');
    for (int i = 0; i < v.size(); i++) {
        v[i] = v[i].substr(1, v[i].length() - 2);
    }
    return v;
}

vector<string> CSV::split(string s, char c) {
    vector<string> v;
    string tmp;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == c) {
            v.push_back(tmp);
            tmp = "";
        } else {
            tmp += s[i];
        }
    }
    if (tmp != "") {
        v.push_back(tmp);
    }
    return v;
}

string CSV::unlines(vector<string> f, string c) {
    string s;
    if (f.size() <= 0)
        return s;
    s += f[0];
    for (int i = 1; i < f.size(); i++) {
        s += c;
        s += f[i];
    }
    return s;
}

string CSV::makeLine(CSVRow r) {
    return "\"" + unlines(r, "\",\"") + "\"";
}

vector<CSVRow> CSV::openFile(string filename) {
    vector<CSVRow> table;
    ifstream f(filename);
    if (!f.is_open()) {
        return table;
    } else {
        string l;
        while (getline(f, l)) {
            CSVRow r = parseLine(l);
            table.push_back(r);
        }
        f.close();
        return table;
    }
}

void CSV::writeFile(vector<CSVRow> table, string filename) {
    ofstream f(filename);
    if (f.is_open()) {
        for (int i = 0; i < table.size(); i++) {
            f << makeLine(table[i]) << endl;
        }
        f.close();
    }
}
