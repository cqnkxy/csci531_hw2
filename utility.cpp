#include <iostream>
#include "KeyGenerator.h"

using namespace::std;

void key_stream(char const *p, int bytes) {
	KeyGenerator kg(p);
    for (int i = 0; i < bytes; ++i) {
        cout << kg.next();
    }
}
