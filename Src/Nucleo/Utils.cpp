#include "Utils.h"

#if defined(__has_include)
  #if __has_include(<filesystem>)
    #include <filesystem>
    namespace fs_impl = std::filesystem;
    #define USE_STD_FS 1
  #elif __has_include(<experimental/filesystem>)
    #include <experimental/filesystem>
    namespace fs_impl = std::experimental::filesystem;
    #define USE_STD_FS 1
  #else
    #define USE_STD_FS 0
  #endif
#else
  #define USE_STD_FS 0
#endif

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

#if !USE_STD_FS
  #include <sys/stat.h>
  #ifdef _WIN32
    #include <direct.h>
    #define MKDIR(p) _mkdir(p)
    #define STAT _stat
  #else
    #include <unistd.h>
    #include <sys/types.h>
    #define MKDIR(p) mkdir((p), 0755)
    #define STAT stat
  #endif
#endif

using namespace std;

namespace FileUtils
{
    static inline string ltrim(const string &s) {
        size_t start = 0;
        while (start < s.size() && isspace(static_cast<unsigned char>(s[start]))) ++start;
        return s.substr(start);
    }

    static inline string rtrim(const string &s) {
        if (s.empty()) return s;
        size_t end = s.size();
        while (end > 0 && isspace(static_cast<unsigned char>(s[end - 1]))) --end;
        return s.substr(0, end);
    }

    string trim(const string& s) {
        return rtrim(ltrim(s));
    }

    string join(const vector<string>& parts, const string& sep) {
        ostringstream oss;
        for (size_t i = 0; i < parts.size(); ++i) {
            if (i) oss << sep;
            oss << parts[i];
        }
        return oss.str();
    }

    vector<string> split(const string& s, char sep) {
        vector<string> out;
        string item;
        istringstream iss(s);
        while (getline(iss, item, sep)) out.push_back(item);
        return out;
    }

#if USE_STD_FS

    bool ensureDir(const string& path) {
        try {
            if (path.empty()) return false;
            fs_impl::path p(path);
            if (fs_impl::exists(p)) return fs_impl::is_directory(p);
            return fs_impl::create_directories(p);
        } catch (...) {
            return false;
        }
    }

    bool readAlllines(const string& file, vector<string>& out) {
        out.clear();
        try {
            ifstream ifs(file);
            if (!ifs.is_open()) return false;
            string line;
            while (getline(ifs, line)) out.push_back(line);
            return true;
        } catch (...) {
            return false;
        }
    }

    bool appendLine(const string& file, const string& line) {
        try {
            fs_impl::path p = file;
            if (p.has_parent_path()) ensureDir(p.parent_path().string());
            ofstream ofs(file, ios::app);
            if (!ofs.is_open()) return false;
            ofs << line << '\n';
            return true;
        } catch (...) {
            return false;
        }
    }

    bool writeLines(const string& file, const vector<string>& lines) {
        try {
            fs_impl::path p = file;
            if (p.has_parent_path()) ensureDir(p.parent_path().string());
            ofstream ofs(file, ios::trunc);
            if (!ofs.is_open()) return false;
            for (const auto& l : lines) ofs << l << '\n';
            return true;
        } catch (...) {
            return false;
        }
    }

    bool fileExists(const string& file) {
        try {
            fs_impl::path p(file);
            return fs_impl::exists(p) && fs_impl::is_regular_file(p);
        } catch (...) {
            return false;
        }
    }

#else // fallback without filesystem

    static bool dirExists(const string& path) {
        if (path.empty()) return false;
        struct STAT st;
        if (STAT(path.c_str(), &st) != 0) return false;
    #ifdef _WIN32
        return (st.st_mode & S_IFDIR) != 0;
    #else
        return S_ISDIR(st.st_mode);
    #endif
    }

    bool ensureDir(const string& path) {
        if (path.empty()) return false;
        string cur;
        cur.reserve(path.size());
        size_t i = 0;
        if (path.size() >= 2 && isalpha((unsigned char)path[0]) && path[1] == ':') {
            cur = path.substr(0, 2);
            i = 2;
            if (path.size() > 2 && (path[2] == '\\' || path[2] == '/')) { cur.push_back('\\'); i = 3; }
        } else if (!path.empty() && (path[0] == '\\' || path[0] == '/')) {
            cur.push_back(path[0]); i = 1;
        }

        for (; i <= path.size(); ++i) {
            if (i == path.size() || path[i] == '/' || path[i] == '\\') {
                string candidate = cur;
                while (!candidate.empty() && (candidate.back() == '/' || candidate.back() == '\\')) candidate.pop_back();
                if (!candidate.empty() && !dirExists(candidate)) {
                    if (MKDIR(candidate.c_str()) != 0) {
                        if (!dirExists(candidate)) return false;
                    }
                }
                if (i < path.size()) cur.push_back(path[i]);
            } else {
                cur.push_back(path[i]);
            }
        }
        return dirExists(path);
    }

    bool readAlllines(const string& file, vector<string>& out) {
        out.clear();
        try {
            ifstream ifs(file);
            if (!ifs.is_open()) return false;
            string line;
            while (getline(ifs, line)) out.push_back(line);
            return true;
        } catch (...) { return false; }
    }

    bool appendLine(const string& file, const string& line) {
        try {
            size_t pos = file.find_last_of("/\\");
            if (pos != string::npos) ensureDir(file.substr(0, pos));
            ofstream ofs(file, ios::app);
            if (!ofs.is_open()) return false;
            ofs << line << '\n';
            return true;
        } catch (...) { return false; }
    }

    bool writeLines(const string& file, const vector<string>& lines) {
        try {
            size_t pos = file.find_last_of("/\\");
            if (pos != string::npos) ensureDir(file.substr(0, pos));
            ofstream ofs(file, ios::trunc);
            if (!ofs.is_open()) return false;
            for (const auto& l : lines) ofs << l << '\n';
            return true;
        } catch (...) { return false; }
    }

    bool fileExists(const string& file) {
        try {
            struct STAT st;
            if (STAT(file.c_str(), &st) != 0) return false;
        #ifdef _WIN32
            return (st.st_mode & S_IFREG) != 0;
        #else
            return S_ISREG(st.st_mode);
        #endif
        } catch (...) { return false; }
    }

#endif

} // namespace FileUtils