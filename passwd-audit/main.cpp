#include <iostream>
#include <filesystem>
#include <fstream>
#include <format>
#include <cstring>
#include "passwd.hpp"
namespace fs = std::filesystem;

// Contains arguments that have been parsed
struct Args {
	bool help;
	char* unrecognized;
};

// Parse the arguments
// This could be made to return a pointer, but with
// as few arguments as are supported, the extra copy
// should not be an issue.
Args parseArgs(int argc, char *argv[]);

// Print help for the program
// "name" is the name of the program as it appears in argv[0]
void printHelpMenu(char *name);

int main(int argc, char *argv[]) {
	Args args = parseArgs(argc, argv);
	if (args.help) {
		printHelpMenu(argv[0]);
		return 0;
	} else if (args.unrecognized != nullptr) {
		std::cerr << "Received an unrecognized parameter: " << args.unrecognized << std::endl;
		return 1;
	}
	if (!fs::exists(Passwd)) {
		std::cerr << "Unable to open passwd file: " << Passwd << " does not exist!" << std::endl;
		return 1;
	}
	std::ifstream opened{Passwd.c_str()};
	std::vector<PasswdLine> passwd;
	try {
		PasswdLine temp_p;
		while (opened >> temp_p) {
			passwd.push_back(temp_p);
		}
	} catch (std::exception e) { // Catch everything
		std::cerr << "Unable to read the passwd file: " << e.what() << std::endl;
	}
	opened.close();
	std::cout << std::format("{:20s}{:15s}Account Type", "Username", "Password") << std::endl;
	for (size_t x = 0; x < passwd.size(); x++) {
		std::cout << std::format("{:20s}", passwd.at(x).username)
				  << std::format("{:15s}", to_string(passwd.at(x).getPasswdState()))
				  << passwd.at(x).checkAccountType()
				  << std::endl;
	}
}

Args parseArgs(int argc, char *argv[]) {
	Args args{false, nullptr};
	// x=0 is the name of the executable
	for (int x = 1; x < argc; x++) {
		if (strcmp(argv[x], "--help") == 0) {
			args.help = true;
		} else {
			args.unrecognized = argv[x];
			break;
		}
	}
	return args;
}

void printHelpMenu(char *name) {
	std::cout << "Usage for " << name << ':' << std::endl
			  << "--help:     Print this menu and exit" << std::endl
			  << "No options: Scan /etc/passwd and print some useful information" << std::endl;
}
