#include <iostream>
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;
#pragma once

const fs::path Passwd = fs::path("/etc/passwd");

enum class PasswdState {IN_PASSWD, IN_SHADOW, DISABLED};
std::string to_string(PasswdState state);
std::ostream& operator<<(std::ostream& os, const PasswdState& state);

enum class AccountType {SYSTEM_STATIC, SYSTEM_DYNAMIC, USER, UNDEFINED};
std::string to_string(AccountType account);
std::ostream& operator<<(std::ostream& os, const AccountType& type);

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

	// Check if this is a system user
	// Note that this only checks if UID is < 1000, and does
	// not check if login.defs (or PAM) have been changed from the default.
	AccountType checkAccountType();

	// IO
	friend std::istream& operator>>(std::istream& is, PasswdLine& line);
	friend std::ostream& operator<<(std::ostream& os, PasswdLine& line);
};
