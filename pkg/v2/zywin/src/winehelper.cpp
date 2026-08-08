#include "winehelper.hpp"
#include "colors.hpp"


// This function has the same logic from it's former codebase i just placed it in a class
std::string 
WineHandler::sanitizeFileName(const std::filesystem::path &file)
{
    std::string name = file.stem().string();
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    for (char &c : name)
    {
        if (!std::isalnum(static_cast<unsigned char>(c)))
            c = '-';
    }

    while (name.find("--") != std::string::npos)
        name.replace(name.find("--"), 2, "-");

    if (!name.empty() && name.front() == '-')
        name.erase(0, 1);

    if (!name.empty() && name.back() == '-')
        name.pop_back();

    if (name.empty())
        name = "application";

    return name;
}


// Generates a wine prefix directory
std::filesystem::path WineHandler::getWinePrefix(const std::filesystem::path &file)
{
        
    // ! Gets the value of the HOME directory variable
    // ? i turned this into a lambda function since this is the only function that call getHomeDirectory
    auto getHomeDirectory = []() {

        const char *home = std::getenv("HOME");
        if (!home) {
            std::cerr << "Unable to determine HOME directory.\n";
            std::exit(1);
        }
        return home;
    };

    // return a path which a premade wineprefix is made
    return std::filesystem::path(getHomeDirectory()) / ".local/share/zywin/prefixes" / sanitizeFileName(file);
}


// Makes sure the the prefix exist by checking it's existence and then using wineboot if it's otherwise
void 
WineHandler::ensureWinePrefix(const std::filesystem::path &prefix)
{

    // Checking for prefix / "system.reg" verifies that the Wine prefix is actually initialized and valid!

    if ( std::filesystem::exists(prefix / "system.reg")) {

        std::cout << CYAN << "Prefix already exist: " RESET << prefix << "\n"; 
        return;
    }


    std::cout << CYAN "Creating Wine prefix: " RESET << prefix << "\n";

    std::string cmd = "WINEPREFIX=\"" + prefix.string() + "\" wineboot >/dev/null 2>&1";
    int status = system(cmd.c_str());

    // checks if the WINEBOOT prefix is actually initialized by wineboot
    if (status != 0) {
        std::cerr << "Error: Failed to initialize Wine prefix at " << prefix << "\n";
    }
}

// Revised runWine() to avoid shell injection by using posix_spawn() instead of system()
void 
WineHandler::runWine(const std::filesystem::path &file) {
    std::filesystem::path prefix = getWinePrefix(file);
    ensureWinePrefix(prefix);

    pid_t pid;

    // Build argv for wine
    std::vector<char*> cmd = {
        const_cast<char *>("wine"),
        const_cast<char *>(file.c_str()),
        nullptr
    };

    // Build environment with WINEPREFIX
    std::string winePrefix = "WINEPREFIX=" + prefix.string();
    std::vector<char*> env;
    
    // copy the contents of winePrefix variable to env to set as the 6th argument for posix_spawn()
    for (char **e = environ; *e != nullptr; ++e) {
        if (strncmp(*e, "WINEPREFIX=", 11) != 0) {  // Skip old WINEPREFIX
            env.push_back(*e);
        }
    }

    env.push_back(const_cast<char *>(winePrefix.c_str()));
    env.push_back(nullptr);

    std::cout << CYAN "Executing wine at prefix: " RESET << prefix << std::endl;
 
    if (posix_spawn(&pid, "/usr/bin/wine", nullptr, nullptr, cmd.data(), env.data()) != 0) {
        std::cerr << "Failed to spawn wine process.\n";
        return;
    }

    int status;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        std::cerr << "Wine execution failed.\n";
    }
}
