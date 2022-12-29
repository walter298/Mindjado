#include <algorithm>
#include <vector>
#include <string>

import types;

export module algorithms;

std::vector<char> charIntersection(const std::string& s1, const std::string& s2) {
	std::vector<char> ret;

	for (auto& c : s1) {
		if (s2.contains(c)) {
			ret.push_back(c);
		}
	}

	return ret;
}

export void pruneNonIntersectingWords(std::vector<std::string>& words, const Guess& guess) {
	words.erase(std::remove_if(words.begin(), words.end(),
		[&](const std::string& str) {
			bool intersected = false;
			
			auto intersection = charIntersection(str, guess.name);
			return intersection.size() == 0;
		}),
		words.end());
}

//eliminate words that don't share at x letters with guess where x = guess.letterCount
export void pruneCallousWords(std::vector<std::string>& words, const Guess& guess) {
	words.erase(std::remove_if(words.begin(), words.end(),
		[&](const std::string& str) {
			auto intersection = charIntersection(str, guess.name);
			return (intersection.size() != guess.letterCount);
		}),
		words.end());
}

//badLetters must be sorted
export void pruneBadCombos(std::vector<std::string>& words, std::string& badLetters) {
	words.erase(std::remove_if(words.begin(), words.end(),
		[&](const std::string& str) {
			std::string sorted = str;
			std::sort(sorted.begin(), sorted.end());

			return sorted.find(badLetters) != std::string::npos;
		}),
		words.end());
}

//guesses must be sorted in descending order of correct letters
export void getDefiniteLetters(const std::vector<Guess>& guesses, std::vector<char>& letters) {
	std::vector<char> goodLetters; //letters that must be in the word

	/*go through each string in guess list and compare it with each string with the same letterCount. 
	if the letters overlap, add them to definiteLetters*/
	for (auto it = guesses.begin(); std::next(it) != guesses.end(); it++) {
		for (auto nextIt = std::next(it); nextIt != guesses.end(); nextIt++) { //compare with each other string
			if (nextIt->letterCount != it->letterCount) {
				break;
			}
			auto intersection = charIntersection(it->name, nextIt->name);
			if (intersection.size() == it->letterCount) {
				goodLetters.insert(goodLetters.begin(), intersection.begin(), intersection.end());
			}
		}
	}

	letters = goodLetters;
}

std::vector<std::string> generateSubsets(const std::string& set, std::string subset, int index = 0) {
	std::vector<std::string> result;

	if (index == set.size()) {
		result.push_back(subset);
		return result;
	}

	auto excluded = generateSubsets(set, subset, index + 1);
	result.insert(result.end(), excluded.begin(), excluded.end());

	subset.push_back(set[index]);
	auto included = generateSubsets(set, subset, index + 1);
	result.insert(result.end(), included.begin(), included.end());

	return result;
}

export std::vector<std::string> getBadSubsets(const std::string& word, size_t size, std::vector<char>& candidates) {
	std::string subset;
	std::vector<std::string> subsets = generateSubsets(word, subset);

	subsets.erase(std::remove_if(subsets.begin(), subsets.end(),
		[&](const std::string& element) {
			if (element.size() < size) {
				for (auto& c : element) {
					candidates.push_back(c);
				}
			}
			return element.size() != size;
		}), subsets.end());

	std::sort(candidates.begin(), candidates.end());
	candidates.erase(std::unique(candidates.begin(), candidates.end()), candidates.end());

	std::sort(subsets.begin(), subsets.end());
	subsets.erase(std::unique(subsets.begin(), subsets.end()), subsets.end());

	return subsets;
}