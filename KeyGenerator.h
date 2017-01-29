#ifndef __UTILITY_H_
#define __UTILITY_H_

#include <openssl/md5.h>

class KeyGenerator {
public:
	KeyGenerator(char const*p);
	~KeyGenerator() {}
	unsigned char next();
private:
	void calMd5();
private:
	unsigned char md5_buf[MD5_DIGEST_LENGTH];
    int i, cnt;
    std::string pphrase;
};

#endif
