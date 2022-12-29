#include <string>

export module types;

export struct Guess {
	std::string name;

	size_t letterCount = 0;
};