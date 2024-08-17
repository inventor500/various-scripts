#include <iostream>
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;
#pragma once

enum class PasswdState {IN_PASSWD, IN_SHADOW, DISABLED};
std::string to_string(const PasswdState& state);
std::ostream& operator<<(std::ostream& os, const PasswdState& state);

class PasswdLine {
public:
	std::string username;
	std::string password;
	unsigned int uid;
	unsigned int gid;
	std::string comment;
	std::string home;
	std::string shell;

	// Get the password state
	PasswdState getPasswdState();

	// IO
	friend std::istream& operator>>(std::istream& is, PasswdLine& line);
	friend std::ostream& operator<<(std::ostream& os, PasswdLine& line);
};

const fs::path Passwd = fs::path("/etc/passwd");
