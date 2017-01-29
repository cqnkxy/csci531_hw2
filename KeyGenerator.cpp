#include <iostream>
#include <openssl/md5.h>
#include "KeyGenerator.h"

using namespace::std;

KeyGenerator::KeyGenerator(char const *p): i(0), cnt(0), pphrase(p) {
    MD5((const unsigned char *)p, strlen(p), md5_buf);
}

unsigned char KeyGenerator::next() {
    if (cnt == (MD5_DIGEST_LENGTH >> 1)) {
        char const *p = pphrase.c_str();
        cnt = 0;
        int len = strlen(p)+2+MD5_DIGEST_LENGTH;
        char *s = new char[len+1];
        sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, p);
        memcpy(s, md5_buf, MD5_DIGEST_LENGTH);
        MD5((const unsigned char*)s, len, md5_buf);
        i = (i+1) % 100;
        delete []s;
    }
    return md5_buf[cnt++];
}
