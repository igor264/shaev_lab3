#pragma once
#include <iostream>
#include <string>
#include <limits>

#define INPUT_LINE(in, str) getline(in>>std::ws, str); \
						std::cerr << str << std::endl

#define PRINT_PARAM(out, x) out<< #x << "=" << x << std::endl

class redirect_output_wrapper
{
	std::ostream& stream;
	std::streambuf* const old_buf;
public:
	redirect_output_wrapper(std::ostream& src)
		:old_buf(src.rdbuf()), stream(src)
	{
	}

	~redirect_output_wrapper() {
		stream.rdbuf(old_buf);
	}
	void redirect(std::ostream& dest)
	{
		stream.rdbuf(dest.rdbuf());
	}
};

template <typename T> 
T read_input(const std::string& prompt, const T min_value = std::numeric_limits<T>::lowest(), const T max_value = std::numeric_limits<T>::max()) {
    T value;
    while (true) {
        std::cout << prompt;

        if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>) {
            if (std::cin >> value && std::cin.peek() == '\n' && value >= min_value && value <= max_value) {
                std::cerr <<  value << std::endl;  // Логирование введенного значения
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            else {
                std::cout << "Invalid input. Please enter a valid number between " << min_value << " and " << max_value << ".\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            std::getline(std::cin >> std::ws, value);
            if (!value.empty()) {
                std::cerr << value << std::endl;  // Логирование введенного значения
                return value;
            }
            else {
                std::cout << "Invalid input. Please enter a non-empty string.\n";
            }
        }
    }
}
