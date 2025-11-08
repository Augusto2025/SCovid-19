#pragma once
#include <string>
#include <vector>

using namespace std;

namespace FileUtils
{
    string trim(const string& s);
    string join(const vector<string>& parts, const string& sep);
    vector<string> split(const string& s, char sep);

    bool ensureDir(const string& path);
    bool readAlllines(const string& file, vector<string>& out);
    bool appendLine(const string& file, const string& line);
    bool writeLines(const string& file, const vector<string>& lines);
    bool fileExists(const string& file);
} // namespace name
