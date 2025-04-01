#include <iostream>
#include <boost/spirit/include/qi.hpp>

int main() {
	std::string input = "1234";
	auto it = input.begin();
	int result;

	bool success = boost::spirit::qi::parse(it, input.end(), boost::spirit::qi::int_, result);

	if (success && it == input.end()) {
		std::cout << "Parsed integer: " << result << '\n';
	} else {
		std::cout << "Parsing failed.\n";
	}

	return 0;
}
