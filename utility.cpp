#include <iostream>
#include <fstream>
#include <vector>
#include "KeyGenerator.h"

using namespace::std;

bool DEBUG = true;

struct BitReader {
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

void key_stream(char const *p, int bytes) {
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

void fill_rows(bool pixel, bool key, vector<vector<bool> > &row1, vector<vector<bool> > &row2) {
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

void output(ofstream &out, vector<bool> &row) {
    for (int i = 0; i < (int)row.size();) {
        unsigned char byte = 0, bit = 1;
        for (int j = 7; j > -1 && i < (int)row.size(); --j) {
            if (row[i++]){
                byte |= (bit << j);
            }
        }
        out << byte;
    }
} 

void encrypt(char const *p, string outfile, istream &in) {
    BitReader brdr(p);
    const string MAGIC = "P4";
    ofstream out0(outfile + ".1.pbm");
    ofstream out1(outfile + ".2.pbm");
    string cur;
    int row, col;
    if (!(in >> cur) || cur != MAGIC) {
        cerr << "Magic number mismatch!" << endl;
        exit(1);
    }
    // didn't take care of the comments
    unsigned char byte;
    in >> col >> row;
    // consume the new line character
    in >> noskipws >> byte;
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
