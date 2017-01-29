#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "utility.h"

using namespace std;

void malformed_command() {
	cerr << "Malformed command" << endl;
	exit(1);
}

void parse_stream(int argc, char *argv[]) {
	if (argc != 4) {
		malformed_command();
	}
	string pphrase;
	int len;
	for (int i=2; i<4; i++) {
		string opt(argv[i]);
		if (opt.substr(0, 3) == "-p=") {
			pphrase = opt.substr(3, -1);
		} else if (opt.substr(0, 3) == "-l=") {
			len = atoi(opt.substr(3, -1).c_str());
		} else {
			malformed_command();		
		}
	}
	key_stream(pphrase.c_str(), len);
}

void parse_encrypt(int argc, char *argv[]) {
	if (argc < 4 || argc > 5) {
		malformed_command();
	}
	string pphrase, pbmfile;

}

void parse_cmd_run(int argc, char *argv[]) {
	if (argc < 2) {
		malformed_command();
	} else if (strcmp(argv[1], "stream") == 0) {
		parse_stream(argc, argv);		
	} else if (strcmp(argv[1], "encrypt") == 0) {
		parse_encrypt(argc, argv);
	} else if (strcmp(argv[1], "dec-base64") == 0) {

	} else {
		malformed_command();
	}
}

int main(int argc, char *argv[]) 
{
	parse_cmd_run(argc, argv);
	return 0;
}
