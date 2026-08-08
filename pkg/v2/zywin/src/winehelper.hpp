#pragma once
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include "logo.hpp"

// needed for posix_spawn()
#include <spawn.h>
#include <sys/wait.h>
#include <string.h>



// Class Template dedicated to help anything to do with wine prefixes

class WineHandler {

public:

    // Class member that runs files with .exe extension
    void runWine(const std::filesystem::path &file);

    // Class member that runs files with .msi extension
    void runMsi(const std::filesystem::path &file);

private:

    // formatting file name into a valid prefix path
    std::string sanitizeFileName(const std::filesystem::path &file);

    // Generates a wine prefix
    std::filesystem::path getWinePrefix(const std::filesystem::path &file);

    // ensures that the wineprefix is generated
    void ensureWinePrefix(const std::filesystem::path &prefix);
};