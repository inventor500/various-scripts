#include <string>
#include "passwd.hpp"

std::istream& operator>>(std::istream& is, PasswdLine& line) {
	std::string _line;
	const char d = ':';
	std::getline(is, _line);
	if (_line == "") {
		return is;
	}
	std::istringstream in{_line};
	// Get te username
	std::getline(in, line.username, d);
	// Get the password
	std::getline(in, line.password, d);
	// Get the uid
	std::string temp;
	std::getline(in, temp, d);		
	try {
		line.uid = std::stoi(temp);
	} catch (std::invalid_argument e) {
		std::cerr << "Unable to parse uid for user '" << line.username << '\'' << std::endl
				  << "Line: " << _line << std::endl;
	}
	// Get the gid
	std::getline(in, temp, d);
	try {
		line.gid = std::stoi(temp);
	} catch (std::invalid_argument e) {
		std::cerr << "Unable to parse gid for user '" << line.username << '\'' << std::endl
				  << "Line: " << _line << std::endl;
	}
	// Get the comment
	std::getline(in, line.comment, d);
	// Get the home directory
	std::getline(in, line.home, d);
	// Get the default shell
	std::getline(in, line.shell, d);
	return is;
}

std::ostream& operator<<(std::ostream& os, PasswdLine& line) {
	const char d = ':';
	os << line.username << d << line.getPasswdState() << d << line.uid << d
	   << line.gid << d << line.comment << d << line.home << d << line.shell;
	return os;
}

PasswdState PasswdLine::getPasswdState() {
	if (this->password == "x") {
		return PasswdState::IN_SHADOW;
	} else if (password == "" || password[0] == '!' || password[0] == '*') {
		return PasswdState::DISABLED;
	} else {
		return PasswdState::IN_PASSWD;
	}
}

AccountType PasswdLine::checkAccountType() {
	if (uid < 100) {
		return AccountType::SYSTEM_STATIC;
	} else if (uid < 500) {
		return AccountType::SYSTEM_DYNAMIC;
	} else if (uid >= 1000) {
		return AccountType::USER;
	} else {
		return AccountType::UNDEFINED;
	}
}

std::string to_string(PasswdState state) {
	switch (state) {
	case PasswdState::IN_PASSWD:
		return "In /etc/passwd";
	case PasswdState::IN_SHADOW:
		return "In /etc/shadow";
	case PasswdState::DISABLED:
		return "Password login disabled";
	}
	// This should never happen...
	throw std::invalid_argument("No such password sate " + std::to_string((int)state));
}

std::ostream& operator<<(std::ostream& os, const PasswdState& state) {
	os << to_string(state);
	return os;
}

std::string to_string(AccountType state) {
	switch (state) {
	case AccountType::SYSTEM_STATIC:
		return "System Static";
	case AccountType::SYSTEM_DYNAMIC:
		return "System Dynamic";
	case AccountType::USER:
		return "User";
	case AccountType::UNDEFINED:
		return "Undefined";
	}
	// This should never happen...
	throw std::invalid_argument("No such account type " + std::to_string((int)state));
}

std::ostream& operator<<(std::ostream& os, const AccountType& type) {
	os << to_string(type);
	return os;
}
