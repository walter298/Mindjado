#include <vector>
#include <fstream>
#include <iostream>
#include <string>

import guesser;

int main() {
	std::fstream wordFile;
	wordFile.open("words.txt");

	std::cout << "Loading dictionary...\n";

	std::vector<std::string> wordList;
	wordList.reserve(12972); //12,972 words in dictionary

	std::string line;
	while (std::getline(wordFile, line)) {
		wordList.push_back(line);
	}

	guess(wordList);
}