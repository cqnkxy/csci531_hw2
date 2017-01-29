#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <openssl/md5.h>
#include "utility.h"

using namespace std;

const bool DEBUG = false;

void malformed_command() 
{
	const string USAGE = "\n\thw2 stream -p=pphrase -l=len\n\thw2 encrypt -p=pphrase -out=name [pbmfile]\n\thw2 merge pbmfile1 pbmfile2\n\thw2 decrypt [pbmfile]\n";
	cerr << "Malformed command. Use it as:" << endl << USAGE << endl;
	exit(1);
}

void parse_stream(int argc, char *argv[]) 
{
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
		exit(1);
	}
	key_stream(pphrase.c_str(), len);
}

void parse_encrypt(int argc, char *argv[]) 
{
	if (argc < 4 || argc > 5) {
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
		ifstream in(pbmfile.c_str());
		if (!in.is_open()) {
			cerr << "File not exists!" << endl;
			exit(1);
		}
		encrypt(pphrase.c_str(), outfile, in);
		in.close();
	} else {
		encrypt(pphrase.c_str(), outfile, cin);
	}
}

void parse_merge(int argc, char *argv[]) 
{
	if (argc != 4) {
		malformed_command();
	}
	if (strncmp(argv[2], "-", 1) == 0 || strncmp(argv[3], "-", 1) == 0) {
		malformed_command();
	}
	ifstream in1(argv[2]), in2(argv[3]);
	if (!in1.is_open()) {
		cerr << "File " << argv[2] << " not exists" << endl;
		exit(1);
	}
	if (!in2.is_open()) {
		cerr << "File " << argv[3] << " not exists" << endl;
		exit(1);	
	}
	merge_2_file(in1, in2);
	in1.close();
	in2.close();
}

void parse_decrypt(int argc, char *argv[]) 
{
	if (argc == 2) {
		decrypt(cin);
	} else if (argc == 3) {
		ifstream in(argv[2]);
		if (!in.is_open()) {
			cerr << "File not exists" << endl;
			exit(1);
		}
		decrypt(in);
		in.close();
	} else {
		malformed_command();
	}
}

void parse_cmd_run(int argc, char *argv[]) 
{
	if (argc < 2) {
		malformed_command();
	} else if (strcmp(argv[1], "stream") == 0) {
		parse_stream(argc, argv);		
	} else if (strcmp(argv[1], "encrypt") == 0) {
		parse_encrypt(argc, argv);
	} else if (strcmp(argv[1], "merge") == 0) {
		parse_merge(argc, argv);
	} else if (strcmp(argv[1], "decrypt") == 0) {
		parse_decrypt(argc, argv);
	} else {
		malformed_command();
	}
}

int main(int argc, char *argv[]) 
{
	parse_cmd_run(argc, argv);
	return 0;
}
