#include <iostream>
#include <filesystem>
#include <fstream>
#include <format>
#include "passwd.hpp"
namespace fs = std::filesystem;

int main(void) {
	if (!fs::exists(Passwd)) {
		std::cerr << "Unable to open passwd file: " << Passwd << " does not exist!" << std::endl;
		return 1;
	}
	std::ifstream opened{Passwd.c_str()};
	std::vector<PasswdLine> passwd;
	PasswdLine temp_p;
	while (opened >> temp_p) {
		passwd.push_back(temp_p);
	}
	std::cout << std::format("{:20s}Password", "Username") << std::endl;
	for (size_t x = 0; x < passwd.size(); x++) {
		std::cout << std::format("{:20s}", passwd.at(x).username)
				  << to_string(passwd.at(x).getPasswdState())
				  << std::endl;
	}
}
