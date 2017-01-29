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
	// i range from 00 to 99 as required by this assignment
	// cnt indicates which byte of md5_buf to return
    int i, cnt;	
    std::string pphrase;
};

#endif
