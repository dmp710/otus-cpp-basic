#include <iostream>
#include <ctime>

int main() {
	const std::string filename = "high_scores.txt";
    const int max_value = 100;

    // Инициализация генератора случайных чисел
    std::srand(std::time(nullptr));
    int random_value = std::rand() % max_value;

    std::cout << "Hi! Enter your name, please:\n";
    std::string username;
    std::cin >> username;

	const int target_value = 54;
	int current_value = 0;
	bool not_win = true;

	std::cout << "Enter your guess:" << std::endl;

	do {
		std::cin >> current_value;

		if (current_value < target_value) {
			std::cout << "less than " << current_value << std::endl;
		}
		else if (current_value > target_value) {
			std::cout << "greater than " << current_value << std::endl;
		}
		else {
			std::cout << "you win!" << std::endl;
			break;
		}

	} while(true);

	return 0;
}