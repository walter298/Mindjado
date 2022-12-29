#include <vector>
#include <string>
#include <random>
#include <iostream>

import types;
import algorithms;

export module guesser;

export void guess(std::vector<std::string>& wordList) {
	std::vector<Guess> goodGuesses;
	std::vector<char> candidates;

	std::string guess;

	std::mt19937 generator;
	std::uniform_int_distribution<size_t> dist;

	int guessCount = 0;

	while (true) {
		guessCount++;

		if (wordList.size() == 0) {
			std::cout << "Error: all words exhausted from dictionary\n";
			return;
		}

		dist = std::uniform_int_distribution(0ull, wordList.size() - 1); //reset range of values for pruned dictionary
		size_t idx = dist(generator);
		guess = wordList[idx];

		std::cout << "Is " << guess << " your word? yes or no\n";

		std::string choiceInput; //whether it is the word or not
		std::cin >> choiceInput;

		if (choiceInput == "yes") {
			std::cout << "I guessed your word in " << guessCount << " guess(s)!\n";
			return;
		}

		std::cout << "how many letters from my guess are in your word?\n";

		size_t letterCount = 0; //how many correct letters
		std::cin >> letterCount;

		size_t originalDictionarySize = wordList.size(); 

		if (letterCount > 0) {
			Guess guessObj = { guess, letterCount };
			goodGuesses.push_back(guessObj);
			
			pruneNonIntersectingWords(wordList, guessObj);
			//prune all words that don't have a single letter of the guesses
			if (letterCount >= 3) {
				pruneCallousWords(wordList, guessObj);
			}

			//sort guesses in descending order of correct letters
			std::sort(goodGuesses.begin(), goodGuesses.end(),
				[&](const Guess& g1, const Guess& g2) {
					return g1.letterCount > g2.letterCount;
				}
			);
		}

		size_t subsetCount;
		if (letterCount == 5) {
			wordList.erase(std::find(wordList.begin(), wordList.end(), guess));
			continue;
		} else {
			subsetCount = letterCount + 1;
		}

		std::sort(guess.begin(), guess.end()); //guess must be sorted to get subsets of it!
		auto badLetterCombos = getBadSubsets(guess, subsetCount, candidates);

		if (candidates.size() > 5) {
			getDefiniteLetters(goodGuesses, candidates);
		}

		//prune all words containing letter combos that can't be in the word
		for (auto& combo : badLetterCombos) {
			//std::cout << "cannot be a word containing: " << combo << std::endl;
			std::sort(combo.begin(), combo.end());
			pruneBadCombos(wordList, combo);
		}
		
		std::cout << originalDictionarySize - wordList.size() << " eliminated words\n";
		std::cout << wordList.size() << " possible words.\n";
	}
}