//
// Created by Daniel Suh on 4/3/25.
//

#ifndef SIMPLEORDERBOOK_INCLUDE_ITCHPARSER_H_
#define SIMPLEORDERBOOK_INCLUDE_ITCHPARSER_H_

#include "Buffer.h"

class ITCHParser {
public:
	ITCHParser(Buffer& buffer);

	~ITCHParser();

	void start();

	void stop();

private:
	void parseLoop();

	Buffer& buffer_;
	bool stopParsing_;

	std::thread parserThread_;
	std::mutex mutex_;
	std::condition_variable cv_;
};

#endif //SIMPLEORDERBOOK_INCLUDE_ITCHPARSER_H_
