#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <sstream>


int readTable(const std::string& file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
			std::cout << "Failed to open file for read: " << file_name << "!" << std::endl;
			return -1;
	}

    if (file.is_open()) {
        std::string line;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::cout << line << std::endl;
        }

        file.close();
    }

    return 0;
}



// Функция для чтения таблицы рекордов из файла
int readHighScores(const std::string& file_name, const std::string& user_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
			std::cout << "Failed to open file for read: " << file_name << "!" << std::endl;
			return -1;
	}

    int count = 0;

    if (file.is_open()) {
        std::string line, name, attempts;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            iss >> name >> attempts;

            std::cout << name << std::endl;
            std::cout << attempts << std::endl;
            
            if (name == user_name) {
                count = std::stoi(attempts);
            }
        }
        file.close();
    } else {
        std::cout << "Internal error" << std::endl;
		return -1;
    }

    return count;
}

// Функция для записи таблицы рекордов в файл
int writeNewRecord(const std::string& file_name, const std::string& user_name, int count) {
		// We should open the output file in the append mode - we don't want
		// to erase previous results.
		std::ofstream out_file{file_name, std::ios_base::app};
		if (!out_file.is_open()) {
			std::cout << "Failed to open file for write: " << file_name << "!" << std::endl;
			return -1;
		}

		// Append new results to the table:
		out_file << user_name << ' ';
		out_file << count;
		out_file << std::endl;

        return 0;
}
    
int main(int argc, char** argv) {
    const std::string file_name = "high_scores.txt";
    int max_value = 100; 
    bool max;
    int level = 0;

    if (argc > 1) {
        for (int i = 1; i < argc; i += 2) {
            std::string arg = argv[i];
            if (arg == "-max") {
                if (!(i + 1 < argc)) {
                    std::cout << "Wrong usage! The argument '-parameter' requires some value!" << std::endl;
				    return -1;
                }
                max = true;
                max_value = std::stoi(argv[i+1]);
            } else if (arg == "-table") {
                return readTable(file_name);
            } else if (arg == "-level") {
                if (!(i + 1 < argc)) {
                    std::cout << "Wrong usage! The argument '-parameter' requires some value!" << std::endl;
				    return -1;
                }

                level = std::stoi(argv[i+1]);
            } else {
                std::cout << "Wrong flag!" << std::endl;
		        return -1;
            }            
        }
    } 

    if (max && level != 0)  {
        std::cout << "Can't use max and level concurrently!" << std::endl;
		return -1;
    }

    // Ask about name
	std::cout << "Hi! Enter your name, please:" << std::endl;
	std::string user_name;
	std::cin >> user_name;

    int record = readHighScores(file_name, user_name);
    if (record == -1) {
        return -1;
    } else if (record == 0) {
        std::cout << user_name << ", you have no record" << std::endl;
    } else {
        std::cout << user_name << ", your record is " << record << std::endl;
    }

    switch (level)
    {
    case 1:
        max_value = 10;
        break;
    case 2:
        max_value = 50;
        break;
    }

    std::srand(std::time(nullptr));
    int random_value = std::rand() % max_value; 

    int attempts = 0;
    int guess;

    std::cout << "Enter your guess:\n";
    while (true) {
        std::cin >> guess;
        if (std::cin.fail()) {
            std::cout << "Bad value!" << std::endl;
            return -1;
        }

        attempts++;

        if (guess > random_value) {
            std::cout << "less than " << guess << std::endl;
        } else if (guess < random_value) {
            std::cout << "greater than " << guess << std::endl;
        } else {
            std::cout << "you win! attempts = " << attempts << std::endl;
            break;
        }
    }

    if (attempts < record || record == 0) {
        std::cout << user_name << ", congragulations! You have new record - " << attempts << std::endl;

        return writeNewRecord(file_name, user_name, attempts);
    }

    return 0;
}