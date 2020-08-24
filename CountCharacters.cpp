#include <cstring>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
//
// Created by root on 2020/8/23.
//

#include "CountCharacters.h"
#include <iostream>
#include "zhc.h"

//must be larger than 12
#define BUFFER_SIZE 30

using namespace std;

CharacterCounter::CharacterCounter() {
	data = new map<int, int64_t>;
}

CharacterCounter::~CharacterCounter() {
	delete data;
}

int CharacterCounter::countCharacters(const char *u8Bytes, int size) {
	if (size == -1) size = strlen(u8Bytes);
	int offset = 0;
	bczhc::utf8::SolvedUTF8Properties solvedProperties{};
	int64_t c = 0;
	while (offset < size) {
		bczhc::utf8::solveUTF8Bytes(solvedProperties, u8Bytes + offset);
		offset += solvedProperties.bytesLength;
		int codepoint = solvedProperties.codepoint;
		if (data->find(codepoint) == data->end()) {
			(*data)[codepoint] = 1;
		} else {
			(*data)[codepoint] = data->find(codepoint)->second + 1;
		}
		++c;
	}
	::cout << c <<::endl;
	return 0;
}

#pragma clang diagnostic pop
