#include <iostream>
#include <fstream>
#include "KeyGenerator.h"

using namespace::std;

void key_stream(char const *p, int bytes) {
	KeyGenerator kg(p);
    for (int i = 0; i < bytes; ++i) {
        printf("%x02", kg.next());
    }
}

void encrypt(char const *p, std::string outfile, std::istream &in) {
    KeyGenerator kg(p);
    string magic = "P4";
    ofstream out1(outfile + ".1.pbm");
    ofstream out2(outfile + ".2.pbm");
}
