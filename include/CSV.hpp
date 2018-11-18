#ifndef CSV_HPP
#define CSV_HPP

#include <vector>
#include <string>

using std::vector;
using std::string;

typedef vector<string> CSVRow;

class CSV {
public:
    static vector<CSVRow> openFile(string filename);
    static void writeFile(vector<CSVRow> table, string filename);
    static vector<string> split(string s, char c);
    static string unlines(vector<string> f, string c);

private:
    static CSVRow parseLine(string l);
    static string makeLine(CSVRow r);
};

#endif
