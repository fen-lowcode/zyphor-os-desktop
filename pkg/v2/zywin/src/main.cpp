#include "logo.hpp"
#include "winehelper.hpp"

const std::string TMP_PREFIX = "zywin-installer-iso-extract-";

// clean up tool for cleaning /tmp directory
inline void cleanupExtracted() {
   
    if (!std::filesystem::exists("/tmp"))
        return;

    int removed = 0;

    for (const auto &entry : std::filesystem::directory_iterator("/tmp"))
    {
        if (!entry.is_directory())
            continue;

        std::string name = entry.path().filename().string();

        if (name.rfind(TMP_PREFIX, 0) == 0)
        {
            std::error_code ec;
            std::filesystem::remove_all(entry.path(), ec);

            if (!ec)
                removed++;
        }
    }

    if (removed > 0)
    {
        std::cout << "Removed "
                  << removed
                  << " extracted ISO director"
                  << (removed == 1 ? "y" : "ies")
                  << ".\n";
    }
}

// formatted print helper to guide users on how to use zywin!!
// this one looks dope !!
inline void printHelper() {

    std::cerr << ZYWIN_LOGO << "\n";
    std::cerr << BOLD CYAN "Zyphor Wine Wrapper (zywin)" RESET " - Windows compatibility layer for Zyphor OS\n\n";
    
    std::cerr << BOLD "USAGE:" RESET "\n";
    std::cerr << "  zywin " CYAN "<target>" RESET "\n\n";

    std::cerr << BOLD "COMMANDS & FILE TYPES:" RESET "\n";
    std::cerr << "  " CYAN "<file.exe>" RESET "\tExecute a Windows binary\n";
    std::cerr << "  " CYAN "<file.msi>" RESET "\tRun a Windows installer package\n";
    std::cerr << "  " CYAN "<file.iso>" RESET "\tMount and run a disc image\n";
    std::cerr << "  " CYAN "cleanup"    RESET "\tPurge temporary wrapper prefixes and runtime caches\n\n";

    std::cerr << BOLD "EXAMPLES:" RESET "\n";
    std::cerr << "  zywin app.exe\n";
    std::cerr << "  zywin installer.msi\n";
    std::cerr << "  zywin cleanup\n\n";
  
} 

int main(int argc, char *argv[]) {

    WineHandler wh;

    // Checks if zywin is execuuted without arguments if not it prints a helper
    if (argc != 2)
    {
        printHelper();
        return 1;
    }

    std::string arg = argv[1];

    if (arg == "cleanup")
    {
        cleanupExtracted();
        return 0;
    }

    std::filesystem::path file(arg);

    if (!std::filesystem::exists(file))
    {
        std::cerr << "File not found.\n";
        return 1;
    }

    // extracts the extention from the input file
    std::string ext = file.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == ".exe") {
        wh.runWine(file);
    } else {
        std::cout << "Work in Progress\n" << std::endl;
    }

    return 0;
}