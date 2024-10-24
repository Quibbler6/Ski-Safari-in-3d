
#include "Grammar.h"

Grammar::Grammar() {
	w = "F[@/L][*@L][+%L]";
	level = 3;

	generations.push_back("F[+@L][/*!L][!-L]");
	generations.push_back("F[*@L][*-L][@/-L]");
	generations.push_back("F[/-L][+@L][-!L]");
	generations.push_back("F[*+L][-*L][/!L]");
}

string Grammar::getResult() {
	Iteration();
	return result;
}

Grammar::~Grammar() {
	generations.clear();
}


void Grammar::Iteration() {
	srand((unsigned)time(0));
	result = w;
	for (int i = 0; i < level; i++) {
		string tmp = "";
		for (int j = 0; j < result.size(); j++) {
			tmp += search(result[j]);
		}
		result = tmp;
	}
}


string Grammar::search(char c) {
	if (c == 'L') {
		
		int index = rand() % generations.size();
		return generations[index];
	}
	else {
		
		string tmp(1, c);
		return tmp;
	}
}

