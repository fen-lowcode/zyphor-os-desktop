#include <iostream>
#include <map>
#include "os_info_fetcher.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]]char* argv[]) {
   
   OS_INFO_FETCHER os_info_fetcher;
   std::map<std::string, std::string> os_info;

   os_info.insert(os_info_fetcher.get_os_info("PRETTY_NAME="));
   os_info.insert(os_info_fetcher.get_os_info("ID="));
   os_info.insert(os_info_fetcher.get_os_info("VERSION="));

   std::cout << os_info["PRETTY_NAME="] << '\n' << os_info["ID="] << '\n' << os_info["VERSION="];
   return 0;
}