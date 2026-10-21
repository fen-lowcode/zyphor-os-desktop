// zylearn - Zyphor OS learning CLI
// Guided Linux lessons, progress tracking and a one-command practice environment.

#include <algorithm>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <pwd.h>
#include <spawn.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

// libc already defines `environ`; we only declare it. Defining our own
// (`char **environ;`) creates a separate NULL variable and the child
// process would start with an empty environment.
extern char **environ;

namespace fs = std::filesystem;
using std::cerr;
using std::cout;
using std::string;
using std::vector;

namespace {

constexpr const char *VERSION = "1.2.0";
constexpr const char *REPO_URL = "https://github.com/zyphor-os/zyphor-os-minimal";
constexpr const char *REPO_DIR = "zyphor-os-minimal";
constexpr const char *DEFAULT_LESSONS_DIR = "/usr/share/zylearn/lessons";

// ---------------------------------------------------------------- paths ----

fs::path homeDir()
{
    if (const char *h = std::getenv("HOME"); h && *h) return h;
    if (const passwd *pw = getpwuid(getuid())) return pw->pw_dir;
    return {};
}

// Per-user state (XDG spec) instead of a shared, predictable file in /tmp.
fs::path stateDir()
{
    if (const char *x = std::getenv("XDG_STATE_HOME"); x && *x)
        return fs::path(x) / "zylearn";
    const fs::path home = homeDir();
    if (home.empty()) return {};
    return home / ".local" / "state" / "zylearn";
}

fs::path lessonsDir()
{
    if (const char *e = std::getenv("ZYLEARN_LESSONS_DIR"); e && *e) return e;
    return DEFAULT_LESSONS_DIR;
}

bool ensureStateDir(const fs::path &dir)
{
    std::error_code ec;
    fs::create_directories(dir, ec);
    if (ec) return false;
    fs::permissions(dir, fs::perms::owner_all, ec); // 0700: nobody else reads your history
    return true;
}

// -------------------------------------------------------------- history ----

void logCommand(const vector<string> &args)
{
    const fs::path dir = stateDir();
    if (dir.empty() || !ensureStateDir(dir)) return; // logging must never break the tool

    std::ofstream file(dir / "history.log", std::ios::app);
    if (!file) return;

    char stamp[32] = {};
    const std::time_t now = std::time(nullptr);
    std::tm tmv = {};
    localtime_r(&now, &tmv);
    std::strftime(stamp, sizeof stamp, "%Y-%m-%d %H:%M:%S", &tmv);

    file << stamp << "  zylearn";
    for (size_t i = 1; i < args.size(); ++i) file << ' ' << args[i];
    file << '\n';
}

// ------------------------------------------------------------ processes ----

bool commandExists(const string &name)
{
    const char *path = std::getenv("PATH");
    if (!path) return false;

    const string p = path;
    size_t start = 0;
    while (start <= p.size()) {
        size_t end = p.find(':', start);
        if (end == string::npos) end = p.size();
        if (end > start) {
            const fs::path cand = fs::path(p.substr(start, end - start)) / name;
            std::error_code ec;
            if (access(cand.c_str(), X_OK) == 0 && fs::is_regular_file(cand, ec)) return true;
        }
        start = end + 1;
    }
    return false;
}

// Runs a program (no shell involved) and returns its exit code, or -1 on failure.
int runProcess(const vector<string> &args)
{
    vector<char *> argv;
    for (const string &a : args) argv.push_back(const_cast<char *>(a.c_str()));
    argv.push_back(nullptr);

    pid_t pid = 0;
    const int rc = posix_spawnp(&pid, argv[0], nullptr, nullptr, argv.data(), environ);
    if (rc != 0) {
        cerr << "zylearn: cannot run '" << args[0] << "': " << std::strerror(rc) << "\n";
        return -1;
    }

    int status = 0;
    if (waitpid(pid, &status, 0) < 0) return -1;
    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
}

// -------------------------------------------------------------- lessons ----

struct Lesson {
    string name;
    string title;
    fs::path path;
};

bool validLessonName(const string &s)
{
    if (s.empty() || s.size() > 64) return false;
    return std::all_of(s.begin(), s.end(), [](char c) {
        return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-' || c == '_';
    });
}

vector<Lesson> loadLessons()
{
    vector<Lesson> out;
    std::error_code ec;
    for (fs::directory_iterator it(lessonsDir(), ec), end; !ec && it != end; it.increment(ec)) {
        if (!it->is_regular_file(ec) || it->path().extension() != ".txt") continue;

        Lesson l;
        l.name = it->path().stem().string();
        l.path = it->path();

        std::ifstream f(l.path);
        string first;
        std::getline(f, first);
        while (!first.empty() && (first[0] == '#' || first[0] == ' ')) first.erase(0, 1);
        l.title = first.empty() ? l.name : first;
        out.push_back(l);
    }
    std::sort(out.begin(), out.end(), [](const Lesson &a, const Lesson &b) { return a.name < b.name; });
    return out;
}

const Lesson *findLesson(const vector<Lesson> &all, const string &name)
{
    for (const Lesson &l : all)
        if (l.name == name) return &l;
    return nullptr;
}

fs::path progressFile()
{
    const fs::path dir = stateDir();
    return dir.empty() ? fs::path{} : dir / "progress.txt";
}

std::set<string> loadProgress()
{
    std::set<string> done;
    const fs::path file = progressFile();
    if (file.empty()) return done;
    std::ifstream f(file);
    string line;
    while (std::getline(f, line))
        if (!line.empty()) done.insert(line);
    return done;
}

bool saveProgress(const string &name)
{
    const fs::path dir = stateDir();
    if (dir.empty() || !ensureStateDir(dir)) return false;
    std::ofstream f(progressFile(), std::ios::app);
    if (!f) return false;
    f << name << '\n';
    return true;
}

// --------------------------------------------------------------- checks ----
//
// A lesson "03-processes.txt" may ship a "03-processes.check" file. One check
// per line, fields separated by "|":
//
//   description | type | arg ... | hint: optional help text
//
// Checks only READ the filesystem. They never run shell commands, so a lesson
// file cannot execute anything on a student's machine.
//
// Types: exists <path>            dir <path>          nonempty <path>
//        mode <path> <octal>      executable <path>   contains <path> <text>
//        lines-at-least <path> <n>  symlink <path> <target>  command <name>
// A leading "~/" in a path means the student's home directory.

struct Check {
    string description;
    string type;
    vector<string> args;
    string hint;
};

string trim(const string &s)
{
    const char *ws = " \t\r\n";
    const size_t b = s.find_first_not_of(ws);
    if (b == string::npos) return "";
    const size_t e = s.find_last_not_of(ws);
    return s.substr(b, e - b + 1);
}

fs::path expandPath(const string &p)
{
    if (p == "~") return homeDir();
    if (p.rfind("~/", 0) == 0) return homeDir() / p.substr(2);
    return p;
}

fs::path checkFileFor(const Lesson &l)
{
    fs::path p = l.path;
    p.replace_extension(".check");
    return p;
}

vector<Check> loadChecks(const fs::path &file)
{
    vector<Check> out;
    std::ifstream f(file);
    string line;
    while (std::getline(f, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        // Split on "|". Once a field starts with "hint:", the REST of the line
        // is the hint, verbatim, so hints may contain "|" (pipes are the point
        // of some lessons).
        vector<string> parts;
        Check c;
        size_t start = 0;
        while (true) {
            const size_t bar = line.find('|', start);
            const string field = trim(line.substr(start, bar == string::npos ? string::npos : bar - start));
            if (parts.size() >= 2 && field.rfind("hint:", 0) == 0) {
                c.hint = trim(trim(line.substr(start)).substr(5));
                break;
            }
            parts.push_back(field);
            if (bar == string::npos) break;
            start = bar + 1;
        }

        c.description = parts[0];
        c.type = parts.size() > 1 ? parts[1] : "";
        for (size_t i = 2; i < parts.size(); ++i) c.args.push_back(parts[i]);
        out.push_back(c);
    }
    return out;
}

bool parseUnsigned(const string &s, int base, unsigned long &out)
{
    if (s.empty()) return false;
    char *end = nullptr;
    errno = 0;
    out = std::strtoul(s.c_str(), &end, base);
    return errno == 0 && end && *end == '\0';
}

string toOctal(unsigned long v)
{
    std::ostringstream os;
    os << std::oct << v;
    return os.str();
}

// Reads at most 1 MiB of a file (checks never need more).
bool readSmallFile(const fs::path &p, string &data)
{
    std::ifstream f(p, std::ios::binary);
    if (!f) return false;
    data.assign(1U << 20, '\0');
    f.read(&data[0], static_cast<std::streamsize>(data.size()));
    data.resize(static_cast<size_t>(f.gcount()));
    return true;
}

bool runCheck(const Check &c, string &why)
{
    const vector<string> &a = c.args;
    std::error_code ec;

    auto needArgs = [&](size_t n) {
        if (a.size() == n) return true;
        why = "lesson bug: '" + c.type + "' expects " + std::to_string(n) + " argument(s)";
        return false;
    };
    auto missing = [&]() {
        why = "not found: " + a[0];
        return false;
    };

    if (c.type == "exists") {
        if (!needArgs(1)) return false;
        return fs::exists(expandPath(a[0]), ec) || missing();
    }
    if (c.type == "dir") {
        if (!needArgs(1)) return false;
        if (fs::is_directory(expandPath(a[0]), ec)) return true;
        why = "folder not found: " + a[0];
        return false;
    }
    if (c.type == "nonempty") {
        if (!needArgs(1)) return false;
        const fs::path p = expandPath(a[0]);
        if (!fs::is_regular_file(p, ec)) return missing();
        if (fs::file_size(p, ec) > 0) return true;
        why = "file is empty: " + a[0];
        return false;
    }
    if (c.type == "mode" || c.type == "executable") {
        const bool isMode = c.type == "mode";
        if (!needArgs(isMode ? 2 : 1)) return false;

        unsigned long want = 0;
        if (isMode && !parseUnsigned(a[1], 8, want)) {
            why = "lesson bug: bad mode '" + a[1] + "'";
            return false;
        }

        struct stat st = {};
        if (stat(expandPath(a[0]).c_str(), &st) != 0) return missing();

        if (!isMode) {
            if (st.st_mode & S_IXUSR) return true;
            why = a[0] + " is not executable";
            return false;
        }
        const unsigned long have = st.st_mode & 07777U;
        if (have == want) return true;
        why = "mode is " + toOctal(have) + ", expected " + a[1];
        return false;
    }
    if (c.type == "contains") {
        if (!needArgs(2)) return false;
        string data;
        if (!readSmallFile(expandPath(a[0]), data)) return missing();
        if (data.find(a[1]) != string::npos) return true;
        why = a[0] + " does not contain the expected text";
        return false;
    }
    if (c.type == "lines-at-least") {
        if (!needArgs(2)) return false;
        unsigned long want = 0;
        if (!parseUnsigned(a[1], 10, want)) {
            why = "lesson bug: bad number '" + a[1] + "'";
            return false;
        }
        std::ifstream f(expandPath(a[0]));
        if (!f) return missing();
        unsigned long lines = 0;
        string tmp;
        while (std::getline(f, tmp)) ++lines;
        if (lines >= want) return true;
        why = a[0] + " has " + std::to_string(lines) + " line(s), expected at least " + a[1];
        return false;
    }
    if (c.type == "symlink") {
        if (!needArgs(2)) return false;
        const fs::path p = expandPath(a[0]);
        if (!fs::is_symlink(p, ec)) {
            why = "not a symbolic link: " + a[0];
            return false;
        }
        if (fs::read_symlink(p, ec).string() == a[1]) return true;
        why = a[0] + " points to the wrong target";
        return false;
    }
    if (c.type == "command") {
        if (!needArgs(1)) return false;
        if (commandExists(a[0])) return true;
        why = "'" + a[0] + "' is not installed";
        return false;
    }

    why = "lesson bug: unknown check type '" + c.type + "'";
    return false;
}

// ------------------------------------------------------------- commands ----

void showHelp()
{
    cout << "zylearn v" << VERSION << " - learn Linux on Zyphor OS\n\n"
         << "Usage:\n"
         << "  zylearn <command> [options]\n\n"
         << "Learning:\n"
         << "  lesson list         List lessons and show which ones you finished.\n"
         << "  lesson show <name>  Read a lesson.\n"
         << "  lesson done <name>  Mark a lesson as completed.\n"
         << "  check <name>        Automatically verify your work for a lesson.\n"
         << "  progress            Show your learning progress.\n"
         << "  setup skeleton      Download the Zyphor OS learning skeleton (needs git).\n\n"
         << "Maintenance:\n"
         << "  doctor              Check that your system is ready for zylearn.\n"
         << "  show logs           Display your command history.\n"
         << "  clear logs          Delete your command history.\n\n"
         << "Options:\n"
         << "  -h, --help          Show this help message.\n"
         << "  -v, --version       Show version information.\n\n"
         << "Start here:\n"
         << "  zylearn lesson list\n";
}

int unknownOption(const string &cmd, const string &sub)
{
    if (sub.empty())
        cerr << "zylearn: '" << cmd << "' needs an option. Try 'zylearn --help'.\n";
    else
        cerr << "zylearn: unknown option '" << sub << "' for '" << cmd << "'. Try 'zylearn --help'.\n";
    return 1;
}

int cmdSetup(const string &sub)
{
    if (sub != "skeleton") return unknownOption("setup", sub);

    std::error_code ec;
    if (fs::exists(REPO_DIR, ec)) {
        cout << "zylearn: '" << REPO_DIR << "' already exists in this directory.\n";
        return 0;
    }
    if (!commandExists("git")) {
        cerr << "zylearn: git is not installed. Install it with: sudo apt install git\n";
        return 1;
    }

    cout << "\nDownloading the Zyphor Minimal learning skeleton...\n\n";
    const int rc = runProcess({"git", "clone", REPO_URL});
    if (rc != 0) {
        cerr << "zylearn: git clone failed (exit code " << rc << "). Check your internet connection.\n";
        return 1;
    }
    cout << "\nDone! Next step:  cd " << REPO_DIR << "\n";
    return 0;
}

int cmdShowLogs()
{
    const fs::path dir = stateDir();
    std::ifstream file(dir.empty() ? fs::path{} : dir / "history.log");
    if (!file) {
        cout << "zylearn: no logs found\n";
        return 1;
    }
    string line;
    while (std::getline(file, line)) cout << line << "\n";
    return 0;
}

int cmdClearLogs()
{
    const fs::path dir = stateDir();
    std::error_code ec;
    if (!dir.empty()) fs::remove(dir / "history.log", ec);
    if (ec) {
        cerr << "zylearn: could not clear history: " << ec.message() << "\n";
        return 1;
    }
    cout << "zylearn history cleared.\n";
    return 0;
}

int cmdLesson(const vector<string> &args)
{
    const string sub = args.size() > 2 ? args[2] : "";
    const vector<Lesson> lessons = loadLessons();

    if (sub == "list") {
        if (lessons.empty()) {
            cout << "zylearn: no lessons found in " << lessonsDir().string() << "\n";
            return 1;
        }
        const std::set<string> done = loadProgress();
        for (const Lesson &l : lessons)
            cout << (done.count(l.name) ? "[x] " : "[ ] ") << l.name << "  -  " << l.title << "\n";
        cout << "\nRead one with:  zylearn lesson show <name>\n";
        return 0;
    }

    if (sub == "show" || sub == "done") {
        const string name = args.size() > 3 ? args[3] : "";
        if (!validLessonName(name)) {
            cerr << "zylearn: usage: zylearn lesson " << sub << " <name>   (see 'zylearn lesson list')\n";
            return 1;
        }
        const Lesson *l = findLesson(lessons, name);
        if (!l) {
            cerr << "zylearn: no lesson called '" << name << "'. See 'zylearn lesson list'.\n";
            return 1;
        }

        if (sub == "show") {
            std::ifstream f(l->path);
            cout << f.rdbuf() << "\n";
            std::error_code ec;
            if (fs::is_regular_file(checkFileFor(*l), ec))
                cout << "Verify your work:  zylearn check " << l->name << "\n";
            else
                cout << "When you're done:  zylearn lesson done " << l->name << "\n";
            return 0;
        }

        if (loadProgress().count(name)) {
            cout << "You already completed '" << name << "'.\n";
            return 0;
        }
        if (!saveProgress(name)) {
            cerr << "zylearn: could not save progress.\n";
            return 1;
        }
        cout << "Nice work! '" << name << "' marked as completed.\n";
        return 0;
    }

    return unknownOption("lesson", sub);
}

int cmdCheck(const vector<string> &args)
{
    const string name = args.size() > 2 ? args[2] : "";
    if (!validLessonName(name)) {
        cerr << "zylearn: usage: zylearn check <name>   (see 'zylearn lesson list')\n";
        return 1;
    }
    const vector<Lesson> lessons = loadLessons();
    const Lesson *l = findLesson(lessons, name);
    if (!l) {
        cerr << "zylearn: no lesson called '" << name << "'. See 'zylearn lesson list'.\n";
        return 1;
    }

    std::error_code ec;
    const fs::path file = checkFileFor(*l);
    const vector<Check> checks = fs::is_regular_file(file, ec) ? loadChecks(file) : vector<Check>{};
    if (checks.empty()) {
        cout << "'" << name << "' has no automatic checks. When you're ready:  zylearn lesson done " << name << "\n";
        return 0;
    }

    cout << "Checking: " << l->title << "\n\n";
    size_t passed = 0;
    for (const Check &c : checks) {
        string why;
        if (runCheck(c, why)) {
            cout << "  [ ok ] " << c.description << "\n";
            ++passed;
            continue;
        }
        cout << "  [FAIL] " << c.description << "\n";
        if (!why.empty()) cout << "         " << why << "\n";
        if (!c.hint.empty()) cout << "         hint: " << c.hint << "\n";
    }

    cout << "\n" << passed << "/" << checks.size() << " checks passed.\n";
    if (passed != checks.size()) {
        cout << "Fix the items above, then run:  zylearn check " << name << "\n";
        return 1;
    }

    if (loadProgress().count(name)) {
        cout << "Lesson already marked as completed.\n";
    } else if (saveProgress(name)) {
        cout << "All done! '" << name << "' marked as completed.\n";
    } else {
        cerr << "zylearn: all checks passed, but progress could not be saved.\n";
        return 1;
    }
    return 0;
}

int cmdProgress()
{
    const vector<Lesson> lessons = loadLessons();
    if (lessons.empty()) {
        cout << "zylearn: no lessons found in " << lessonsDir().string() << "\n";
        return 1;
    }
    const std::set<string> done = loadProgress();
    size_t finished = 0;
    for (const Lesson &l : lessons)
        if (done.count(l.name)) ++finished;

    const size_t total = lessons.size();
    const size_t filled = finished * 20 / total;
    cout << "Progress: [" << string(filled, '#') << string(20 - filled, '.') << "] "
         << finished << "/" << total << " lessons\n";
    if (finished == total) cout << "You finished every lesson. Time to build something!\n";
    return 0;
}

int cmdDoctor()
{
    int problems = 0;
    auto check = [&](bool ok, const string &what, const string &fix) {
        cout << (ok ? "[ ok ] " : "[FAIL] ") << what << "\n";
        if (!ok) {
            cout << "       fix: " << fix << "\n";
            ++problems;
        }
    };

    const passwd *pw = getpwuid(getuid());
    cout << "user: " << (pw ? pw->pw_name : "unknown") << "\n\n";

    check(commandExists("git"), "git is installed", "sudo apt install git");

    std::error_code ec;
    check(fs::is_directory(lessonsDir(), ec), "lessons found in " + lessonsDir().string(),
          "reinstall zylearn, or set ZYLEARN_LESSONS_DIR");

    const fs::path dir = stateDir();
    check(!dir.empty() && ensureStateDir(dir), "progress can be saved in " + dir.string(),
          "check that your home directory is writable");

    cout << (problems ? "\nSome checks failed.\n" : "\nEverything looks good.\n");
    return problems ? 1 : 0;
}

} // namespace

int main(int argc, char *argv[])
{
    const vector<string> args(argv, argv + argc);

    if (args.size() < 2) {
        cerr << "zylearn: missing command. Try 'zylearn --help'.\n";
        return 1;
    }

    const string &cmd = args[1];
    const string sub = args.size() > 2 ? args[2] : "";

    if (cmd == "--version" || cmd == "-v") {
        cout << "zylearn version v" << VERSION << "\n";
        return 0;
    }
    if (cmd == "--help" || cmd == "-h" || cmd == "help") {
        showHelp();
        return 0;
    }

    logCommand(args);

    if (cmd == "setup") return cmdSetup(sub);
    if (cmd == "lesson") return cmdLesson(args);
    if (cmd == "check") return cmdCheck(args);
    if (cmd == "progress") return cmdProgress();
    if (cmd == "doctor") return cmdDoctor();
    if (cmd == "show") return sub == "logs" ? cmdShowLogs() : unknownOption(cmd, sub);
    if (cmd == "clear") return sub == "logs" ? cmdClearLogs() : unknownOption(cmd, sub);

    cerr << "zylearn: unknown command '" << cmd << "'. Type zylearn --help for more information.\n";
    return 1;
}
