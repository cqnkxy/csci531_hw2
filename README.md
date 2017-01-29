# USC CSCI531 HW2

## Compile

type `make` or `make hw2` to generate executable.
type `make clean` to clear up *.o file and executable.

## Command

### stream cipher: ./hw2 stream -p=pphrase -l=len
The ASCII string pphrase contains the passphrase (which must not be empty) for key generation and len is the number of bytes to output.

### encrypt: ./hw2 encrypt -p=pphrase -out=name [pbmfile]
Encrypts an input PBM file with the simple stream cipher. This should create 2 output files having the same filename as name but one with .1.pbm appended and the other with .2.pbm appended to the filenames. For example, if name is foo, the output file names must be foo.1.pbm and foo.2.pbm. The relationship between an input pixel, a key bit, and pixels in the output files is specified in the [table](http://merlot.usc.edu/cs531-s17/homeworks/hw2/).

### merge 2 pbm: ./hw2 merge pbmfile1 pbmfile2
Create an output PBM file by merging the bits of PBM files pbmfile1 and pbmfile2. A bit value of 1 is treated as a black pixel and a bit value of 0 is treated as a white pixel. When merging a bit of 1 and a bit of 0, the output must be a 1. You must use 0 as the padding bit at the end of a row in the output PBM file.

### decrypt: ./hw2 decrypt [pbmfile]
Treats the input PBM file as the result of the merge command and perform the reverse of the encryption algorithm. The resulting image size should be 1/4 of the original input.
