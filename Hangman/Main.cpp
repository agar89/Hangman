#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <clocale>


struct Player {
private:
	int guesses;
	char letterGuessed;
	std::vector<char> lettersGuessed;
public:
	Player() { guesses = 6; }
	bool checkedGuessed(Player& p, char c);
	int getGuessesLeft() { return guesses; }
	void wrongGuess() { --guesses; }
	void addLetter(char c) { lettersGuessed.push_back(c); }
};

struct Computer {
private:
	std::vector<std::string> wordList;
	int numberWords;
	std::string wordChosen;
	std::string hiddenWord;
	bool wordFound;
	int blanks;
public:
	Computer() { numberWords = 0, hiddenWord = "", wordFound = false, blanks = 0; }
	void selectWord(Computer& c);
	void populateList(Computer& c);
	int numberOfWords() { return numberWords; }
	std::string getWord() { return wordChosen; }
	std::string getHidden() { return hiddenWord; }
	void setHidden(std::string s) { hiddenWord = s; }
	void drawWord() { std::cout << '\n' << " " << hiddenWord << std::endl; }
	int getBlanks() { return blanks; }
	void removeBlank() { blanks--; }
	void setBlank(int i) { blanks = i; }
	bool checkWord(Player& p, Computer & c, char& ch);
};

bool Player::checkedGuessed(Player& p, char c) {
	for (std::vector<char>::iterator it = p.lettersGuessed.begin(); it != p.lettersGuessed.end(); ++it) {
		if (c == *it)
			return true;
	}
	return false;
}

void Computer::selectWord(Computer& c) {
	int random = rand() % c.wordList.size();
	c.wordChosen = c.wordList[random];
	std::string newHidden(c.wordChosen.size(), '_');
	c.setHidden(newHidden);
	c.setBlank(newHidden.size());
}

void Computer::populateList(Computer& c) {
	std::ifstream words("Hangman.txt");
	std::string temp;

	while (std::getline(words, temp)) {
		c.wordList.push_back(temp);
		c.numberWords++;
	}

	words.close();
}

void clearScreen() {
	std::cout << std::string(10, '\n');
}

void drawBoard(Player& p) {
	int x = p.getGuessesLeft();

	switch (x) {
	case 6:
		std::cout << " _____" << std::endl;
		std::cout << " |   |" << std::endl;
		std::cout << " |   " << std::endl;
		std::cout << " | " << std::endl;
		std::cout << " | " << std::endl;
		std::cout << " | " << std::endl;
		std::cout << " --------" << std::endl;
		break;
	case 5:
		std::cout << " _____" << std::endl;
		std::cout << " |   |" << std::endl;
		std::cout << " |   O " << std::endl;
		std::cout << " | " << std::endl;
		std::cout << " | " << std::endl;
		std::cout << " | " << std::endl;
		std::cout << " --------" << std::endl;
		break;
	case 4:
		std::cout << " _____" << std::endl;
		std::cout << " |   |" << std::endl;
		std::cout << " |   O " << std::endl;
		std::cout << " |   |" << std::endl;
		std::cout << " |   |" << std::endl;
		std::cout << " | " << std::endl;
		std::cout << " --------" << std::endl;
		break;
	case 3:
		std::cout << " _____" << std::endl;
		std::cout << " |   |" << std::endl;
		std::cout << " |   O " << std::endl;
		std::cout << " |  \\|" << std::endl;
		std::cout << " |   |" << std::endl;
		std::cout << " | " << std::endl;
		std::cout << " --------" << std::endl;
		break;
	case 2:
		std::cout << " _____" << std::endl;
		std::cout << " |   |" << std::endl;
		std::cout << " |   O " << std::endl;
		std::cout << " |  \\|/" << std::endl;
		std::cout << " |   |" << std::endl;
		std::cout << " | " << std::endl;
		std::cout << " --------" << std::endl;
		break;
	case 1:
		std::cout << " _____" << std::endl;
		std::cout << " |   |" << std::endl;
		std::cout << " |   O " << std::endl;
		std::cout << " |  \\|/" << std::endl;
		std::cout << " |   |" << std::endl;
		std::cout << " |  /" << std::endl;
		std::cout << " --------" << std::endl;
		break;
	}
}

char getGuess() {
	char guess;
	std::cin >> guess;
	return guess;
}

bool Computer::checkWord(Player& p, Computer& c, char& guess) {
	std::string word = c.getWord();
	std::string newHidden = c.getHidden();
	char nextGuess;
	bool letterFound = false;
	bool alreadyGuessed = p.checkedGuessed(p, guess);

	std::cout << std::endl;
	
	if (alreadyGuessed == true) {
		std::cout << "You already guessed the letter: " << guess << '\n';
		return false;
	}

	for (int i = 0; i < word.size(); ++i) {
		if (word[i] == guess) {
			removeBlank();
			newHidden.at(i) = guess;
			letterFound = true;
		}
		if (c.getBlanks() == 0)
			break;
	}

	if (c.getBlanks() == 0) {
		c.setHidden(newHidden);
		return true;
	}
	if (!letterFound) {
		p.addLetter(guess);
		p.wrongGuess();
		std::cout << "You have " << p.getGuessesLeft() << " guesses left!" << std::endl;
		return false;
	}
	if (p.getGuessesLeft() > 0) {
		p.addLetter(guess);
		c.setHidden(newHidden);
		c.drawWord();
		nextGuess = getGuess();
		checkWord(p, c, nextGuess);
		return false;
	}

	std::cout << '\n';

}



void beginGame() {
	char again;
	do {
		bool found = false;
		Computer comp;
		Player human;
		char guess;

		comp.populateList(comp);
		comp.selectWord(comp);
		std::cout << std::string(2, '\n');

		while (comp.getBlanks() > 0 && human.getGuessesLeft() > 0) {
			drawBoard(human);
			comp.drawWord();
			guess = getGuess();
			comp.checkWord(human, comp, guess);
		}

		if (human.getGuessesLeft() == 0) {
			std::cout << " _____" << std::endl;
			std::cout << " |   |" << std::endl;
			std::cout << " |   O " << std::endl;
			std::cout << " |  \\|/" << std::endl;
			std::cout << " |   |" << std::endl;
			std::cout << " |  / \\" << std::endl;
			std::cout << " --------" << std::endl;
			std::cout << "You lose." << " The word was " << comp.getWord() << '\n';
		}

		if (comp.getBlanks() == 0) {
			drawBoard(human);
			std::cout << "Congratulations, you win!" << " The word was " << comp.getWord() << '\n';
		}


		std::cout << "Would you like to play again? Y/N \n";
		std::cin >> again;
	} while (again == 'Y');
}

int intro() {
	int answer = 0;


	while (std::cin) {
		try {
			std::cout << "Select your option: \n"
				<< "\nPress \"1\" to play \n"
				<< "Press \"2\" to quit \n";

			std::cin >> answer;

			if (answer != 1 && answer != 2)
				throw  answer;
			if (answer == 2 || answer == 1)
				break;
		}
		catch (int i) {
			std::cout << "\n" << i << " is not a option.\n";
		}

	}
	if (answer == 2)
		return -1;

	if (answer == 1) {
		beginGame();

	}

}




int main() {
	srand(time(NULL));


	std::cout << "WELCOME TO HANGMAN!" << std::endl
		<< "\nLet's begin!" << std::endl;

	return intro();
}