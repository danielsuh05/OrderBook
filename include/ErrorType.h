//
// Created by Daniel Suh on 3/28/25.
//

#ifndef SIMPLEORDERBOOK_ERRORTYPE_H
#define SIMPLEORDERBOOK_ERRORTYPE_H
#include <iostream>

enum class ErrorType {
	Parsing,
};

inline std::ostream &operator<<(std::ostream &os, ErrorType e) {
	switch (e) {
		case ErrorType::Parsing: return (os << "PARSING");
	}
	return os;
}

#endif //SIMPLEORDERBOOK_ERRORTYPE_H
