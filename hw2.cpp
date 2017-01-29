#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <openssl/md5.h>
#include "utility.h"

using namespace std;

const bool DEBUG = true;

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
	try {
		for (int i=2; i<argc; i++) {
			string opt(argv[i]);
			if (opt.substr(0, 3) == "-p=") {
				pphrase = opt.substr(3, -1);
			} else if (opt.substr(0, 3) == "-l=") {
				len = atoi(opt.substr(3, -1).c_str());
			} else {
				malformed_command();		
			}
		}
	} catch(...) {
		malformed_command();
	}
	if (len <= 0) {
		cerr << "Len must be positive!" << endl;
		exit(0);
	}
	key_stream(pphrase.c_str(), len);
}

void parse_encrypt(int argc, char *argv[]) {
	if (argc < 4 || argc > 5) {
		// cout << "hasdfj"<< endl;
		malformed_command();
	}
	string pphrase, pbmfile="", outfile;
	for (int i = 2; i < argc; i++) {
		string opt(argv[i]);
		if (opt.substr(0, 3) == "-p=") {
			pphrase = opt.substr(3, -1);
		} else if (opt.substr(0, 5) == "-out=") {
			outfile = opt.substr(5, -1);
		} else {
			pbmfile = opt;
		}
	}
	if (DEBUG) {
		cout << "pphrase: " << pphrase << ". outfile: " << outfile 
			 << ". pbmfile: " << pbmfile << endl;
	}
	if (pbmfile != "") {
		ifstream in(pbmfile);
		if (!in.is_open()) {
			cerr << "File not exists!" << endl;
			exit(0);
		}
		encrypt(pphrase.c_str(), outfile, in);
	} else {
		encrypt(pphrase.c_str(), outfile, cin);
	}
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
