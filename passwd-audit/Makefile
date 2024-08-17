CPPFLAGS := -g -std=c++20

passwd-audit: passwd.o main.cpp
	g++ $(CPPFLAGS) -o passwd-audit main.cpp passwd.o
passwd.o: passwd.h passwd.cpp
	g++ $(CPPFLAGS) passwd.cpp
