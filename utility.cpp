#include <iostream>
#include <fstream>
#include <vector>
#include "KeyGenerator.h"

using namespace::std;

const bool DEBUG = true;
const string MAGIC = "P4";

struct BitReader 
{
    BitReader(char const *p): cnt(7), kg(p) {
        cur_byte = kg.next();
    }
    bool next() {
        if (cnt == -1) {
            cur_byte = kg.next();
            cnt = 7;
        }
        return cur_byte & (1 << (cnt--));
    }
    int cnt;
    unsigned char cur_byte;
    KeyGenerator kg;
};

void key_stream(char const *p, int bytes) 
{
    KeyGenerator kg(p);
    for (int i = 0; i < bytes; ++i) {
        printf("%02x", kg.next());
    }
}

void key_bit_stream(char const *p, int bytes) {
    BitReader br(p);
    for (int i = 0; i < bytes*8; ++i) {
        cout << br.next();
    }
    cout << endl;
}

void fill_rows(bool pixel, bool key, vector<vector<bool> > &row1, vector<vector<bool> > &row2) 
{
    if (!pixel) {
        if (!key) {
            // pixel=0, key=0
            row1[0].push_back(1); row1[0].push_back(0);
            row2[0].push_back(0); row2[0].push_back(1);
            row1[1].push_back(1); row1[1].push_back(0);
            row2[1].push_back(0); row2[1].push_back(1);
        } else {
            // pixel=0, key=1
            row1[0].push_back(0); row1[0].push_back(1);
            row2[0].push_back(1); row2[0].push_back(0);
            row1[1].push_back(0); row1[1].push_back(1);
            row2[1].push_back(1); row2[1].push_back(0);
        }
    } else {
        if (!key) {
            // pixel=1, key=0
            row1[0].push_back(1); row1[0].push_back(0);
            row2[0].push_back(0); row2[0].push_back(1);
            row1[1].push_back(0); row1[1].push_back(1);
            row2[1].push_back(1); row2[1].push_back(0);
        } else {
            // pixel=1, key=1
            row1[0].push_back(0); row1[0].push_back(1);
            row2[0].push_back(1); row2[0].push_back(0);
            row1[1].push_back(1); row1[1].push_back(0);
            row2[1].push_back(0); row2[1].push_back(1);
        }
    }
}

// Transform bool vec to byte and output to out.
void output(ofstream &out, vector<bool> &row) 
{
    for (int i = 0; i < (int)row.size();) {
        unsigned char byte = 0;
        for (int j = 7; j > -1 && i < (int)row.size(); --j) {
            if (row[i++]){
                byte |= (1 << j);
            }
        }
        out << byte;
    }
} 

void encrypt(char const *p, string outfile, istream &in) 
{
    BitReader brdr(p);
    ofstream out0(outfile + ".1.pbm");
    ofstream out1(outfile + ".2.pbm");
    string cur;
    int row, col;
    if (!(in >> cur) || cur != MAGIC) {
        cerr << "Magic number mismatch!" << endl;
        exit(1);
    }
    // didn't take care of comments in pbm
    unsigned char byte;
    // also consume the new line character
    in >> col >> row >> noskipws >> byte;
    out0 << MAGIC << endl << col*2 << " " << row*2 << endl;
    out1 << MAGIC << endl << col*2 << " " << row*2 << endl;
    // row1[0] first row of out0
    // row1[1] first row of out1
    // row2[0] second row of out0
    // row2[1] second row of out1
    vector<vector<bool> > row1(2, vector<bool>()), row2(2, vector<bool>());
    for (int i = 0; i < row; ++i) {
        int col_cnt = 0;
        row1[0].clear(); row1[1].clear();
        row2[0].clear(); row2[1].clear();
        while (col_cnt < col) {
            in >> noskipws >> byte;
            for (int j = 7; j > -1 && col_cnt < col; --j, ++col_cnt) {
                fill_rows((byte >> j) & 0x1, brdr.next(), row1, row2);
            }
        }
        output(out0, row1[0]);
        output(out0, row2[0]);
        output(out1, row1[1]);
        output(out1, row2[1]);
    }
    out0.close();
    out1.close();
}

void merge_2_file(istream &in1, istream &in2)
{
    int row, col;
    string tmp;
    unsigned char byte1, byte2;
    // consume magic number
    in1 >> tmp; in2 >> tmp;
    in1 >> col >> row >> noskipws >> byte1;
    in2 >> col >> row >> noskipws >> byte2;
    cout << MAGIC << endl << col << " " << row << endl;
    while ((in1 >> byte1) && (in2 >> byte2)) {
        cout << (unsigned char)(byte1 | byte2);
    }
}

void read_row(istream &in, int col_cnt, vector<bool> &row)
{
    unsigned char byte;
    for (int i = 0; i < col_cnt;) {
        in >> noskipws >> byte;
        for (int j = 7; j >= 0 && i < col_cnt; --j, ++i) {
            row.push_back((byte >> j) & 0x1);
        }
    }
}


// In fact we only need one row to do the decryption.
vector<unsigned char> decrypt_2_row(vector<bool> &row1)
{
    vector<unsigned char> bytes;
    for (int i = 0; i < (int)row1.size();) {
        unsigned char byte = 0;
        for (int j = 7; j >= 0 && i < (int)row1.size(); --j, i += 2) {
            if (!row1[i] && !row1[i+1]) {
                cerr << "Bad input for merged file!" << endl;
                exit(1);
            }
            byte |= ((row1[i] & row1[i+1]) << j);
        }
        bytes.push_back(byte);
    }
    return bytes;
}

void decrypt(istream &in) 
{
    int row, col;
    string tmp;
    unsigned char byte;
    in >> tmp >> col >> row >> noskipws >> byte;
    vector<bool> row1, row2;
    cout << MAGIC << endl << (col >> 1)<< " " << (row >> 1) << endl;
    for (int i = 0; i < row; i += 2) {
        row1.clear();
        row2.clear();
        read_row(in, col, row1);
        read_row(in, col, row2);
        vector<unsigned char> tmp = decrypt_2_row(row1);
        for (int j = 0; j < (int)tmp.size(); ++j) {
            cout << tmp[j];
        }
    }
}
