#include "os_info_fetcher.hpp"


std::pair<std::string, std::string> 
OS_INFO_FETCHER::get_os_info(std::string key)  {

    const std::string OS_INFO = "/etc/os-release";

    std::ifstream file(OS_INFO);
    if(!file.is_open()) {
        std::cerr << "Failed to open " << OS_INFO;
        exit(-1);
    }
    
    
    std::stringstream ss; ss << file.rdbuf();
    std::string buffer = ss.str();


    size_t start_buffer = buffer.find(key);

    
    std::stringstream output_stream;
    for (char c: buffer.substr(start_buffer + key.size()) ) {
        if (c == '\n' || c == '\r' || c == '\0') {
            output_stream << c;
            break;
        } 
        output_stream << c;
    }

    auto result = std::pair<std::string, std::string>( {key, remove_quotes(output_stream.str())});
    return result;
};