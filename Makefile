hw2: hw2.o utility.o KeyGenerator.o
	g++ -W hw2.o utility.o KeyGenerator.o -o hw2 -lssl -lcrypto -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include

KeyGenerator.o: KeyGenerator.h KeyGenerator.cpp
	g++ -W -c KeyGenerator.cpp -I/usr/local/opt/openssl/include

utility.o: utility.h utility.cpp
	g++ -W -c utility.cpp -I/usr/local/opt/openssl/include

hw2.o: hw2.cpp
	g++ -W -c hw2.cpp

clean:
	rm -f *.o *.gch hw2
