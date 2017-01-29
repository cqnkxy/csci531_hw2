#ifndef __UTILITY_H_
#define __UTILITY_H_

#include <iostream>
#include <string>

void key_stream(char const*, int);
void encrypt(char const *p, std::string outfile, std::istream &in);
void merge_2_file(std::istream &, std::istream &);

#endif
