#include "winehelper.hpp"
#include "colors.hpp"


extern char **environ;

// This function has the same logic from it's former codebase i just placed it in a class
std::string  WineHandler::sanitizeFileName(const std::filesystem::path &file)
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
void WineHandler::ensureWinePrefix(const std::filesystem::path &prefix)
{

    // Checking for prefix / "system.reg" verifies that the Wine prefix is actually initialized and valid!

    if ( std::filesystem::exists(prefix / "system.reg")) {

        std::cout << BRIGHT_YELLOW << "Prefix already exist: " RESET << prefix << "\n"; 
        return;
    }


    std::cout << BRIGHT_YELLOW "Creating Wine prefix: " RESET << prefix << "\n";

    std::string cmd = "WINEPREFIX=\"" + prefix.string() + "\" wineboot >/dev/null 2>&1";
    int status = system(cmd.c_str());

    // checks if the WINEBOOT prefix is actually initialized by wineboot
    if (status != 0) {
        std::cerr << "Error: Failed to initialize Wine prefix at " << prefix << "\n";
    }
}


// Class member responsible of traversing an entire directory after extracting from an iso to find executables
std::vector<std::filesystem::path> 
WineHandler::findExecutables(const std::filesystem::path &dir) {

    std::vector<std::filesystem::path> executables;

    for(const auto &file : std::filesystem::recursive_directory_iterator(dir)) {

        // skips if a file is not a normal file like if it's a . or .. diectory traversal linux thingy
        if(!file.is_regular_file()) { continue; }

         // extracts the extention from the input file
        std::string ext = file.path().extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        if(ext == ".exe") {
            executables.push_back(file.path());
        }
    }

    return executables;
}


// Revised runWine() into execExe() to avoid shell injection by using posix_spawn() instead of system()
void  WineHandler::execExe(const std::filesystem::path &file) {

    std::cout << BRIGHT_YELLOW "Executing a Portable Executable file (.exe) via wine..\n";

    std::filesystem::path prefix = getWinePrefix(file);
    ensureWinePrefix(prefix);

    // Build argv for wine argumnt
    std::vector<char*> cmd = {
        const_cast<char *>("wine"),
        const_cast<char *>(file.c_str()),
        nullptr
    };

    // Build environment with WINEPREFIX
    std::string winePrefix = "WINEPREFIX=" + prefix.string();
    std::vector<char*> env;
    
    // loop throught the entire enviroment and skips over WINEPREFIX= to diregard old prefix
    for (char **e = environ; *e != nullptr; ++e) {
        if (strncmp(*e, "WINEPREFIX=", 11) != 0) {  // Skip old WINEPREFIX
            env.push_back(*e);
        }
    }

      // place the new WINEPREFIX to environ
    env.push_back(const_cast<char *>(winePrefix.c_str()));
    env.push_back(nullptr);

    std::cout << BRIGHT_YELLOW "Executing wine at prefix: " RESET << prefix << std::endl;
 
    pid_t pid;
    if (posix_spawn(&pid, "/usr/bin/wine", nullptr, nullptr, cmd.data(), env.data()) != 0) {
        std::cerr << BRIGHT_RED "Failed to spawn wine process, please make sure wine software exists in your software\n";
        return;
    }

    int status;
    waitpid(pid, &status, 0);

    // ? checks if the child process (which is posix_spawn() that executes wine) did not exit normally 
    // ? or if the child process exited normally but retruned a failure
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        std::cerr << BRIGHT_RED "Wine execution failed, Please make sure the file exists and a valid windows application\n";
    }
}


// Revised runMsi() into execMSi() to avoid shell injection
void WineHandler::execMsi(const std::filesystem::path &file) {

    std::cout << BRIGHT_YELLOW "Executing a Microsoft Installer (.msi) via wine..\n";
    
    std::filesystem::path prefix = getWinePrefix(file);
    ensureWinePrefix(prefix);

    
     // Build argv for wine argumnt this is for running msi softwares utilizing msiexec argument
    std::vector<char*> cmd = {
        const_cast<char *>("wine"),
        const_cast<char *>("msiexec"),
        const_cast<char *>("/i"),
        const_cast<char *>(file.c_str()),
        nullptr
    };

    std::string winePrefix = "WINEPREFIX=" + prefix.string();
    std::vector<char*> env;

    // loop throught the entire enviroment and skips over WINEPREFIX= to diregard old prefix
    for (char **e = environ; *e != nullptr; ++e) {
        if (strncmp(*e, "WINEPREFIX=", 11) != 0) {  
            env.push_back(*e);
        }
    }

    // place the new WINEPREFIX to environ
    env.push_back(const_cast<char *>(winePrefix.c_str()));
    env.push_back(nullptr);

    std::cout << BRIGHT_YELLOW "Executing wine at prefix: " RESET << prefix << std::endl;
    pid_t pid;

    if ( posix_spawn(&pid, "/usr/bin/wine", nullptr, nullptr, cmd.data(), env.data()) != 0) {
        std::cerr << BRIGHT_RED "Failed to spawn wine process, please make sure wine software exists in your software\n";
        return;
    }

    int status;
    waitpid(pid, &status, 0);

    // ? checks if the child process (which is posix_spawn() that executes wine) did not exit normally 
    // ? or if the child process exited normally but retruned a failure
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        std::cerr << BRIGHT_RED "Wine execution failed, Please make sure the file exists and a valid windows application\n";
    }
}

void WineHandler::execIso(const std::filesystem::path &file) {
    const std::string TMP_PREFIX = "zywin-installer-iso-extract-";

    // gets the current time from the system and then convert to local time structure
    auto time_now = std::time(nullptr);

    std::filesystem::path tmp = "/tmp/" + TMP_PREFIX + std::to_string(time_now);
    std::filesystem::create_directories(tmp);
    std::cout << "reached here\n";
}