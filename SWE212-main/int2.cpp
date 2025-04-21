#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <time.h>
#include <math.h>
#include <chrono>
using namespace std;

long int public_key;
long int private_key;
long long int n;
long long int gcd(long long int a, long long int h)
{
	long long int temp;
	while (1) {
		temp = a % h;
		if (temp == 0)
			return h;
		a = h;
		h = temp;
	}
}
int brute_force(long int a, long int b) {
	long int result = 0;
	long int multiplier = 1;
	while (b > 0) {
		long int digit = b % 10;
		result += a * digit * multiplier;
		b /= 10;
		multiplier *= 10;
	}
	return result;
}
long long int encrypt(double message)
{
	long int e = public_key;
	long long int encrpyted_text = 1;
	while (e--) {//message^public_key % n
		encrpyted_text *= message;
		encrpyted_text %= n;
	}
	return encrpyted_text;
}
long long int decrypt(int encrpyted_text)
{
	long int d = private_key;
	long long int decrypted = 1;
	while (d--) {//encrpyted_text^private_key % n
		decrypted *= encrpyted_text;
		decrypted %= n;
	}
	return decrypted;
}
vector<int> encoder(string message)
{
	vector<int> form;
	for (char letter : message) {
		form.push_back(encrypt((int)letter));
	}
	return form;
}
string decoder(vector<int> encoded)
{
	string s;
	// calling the decrypting function decoding function
	for (int num : encoded)
		s += decrypt(num);
	return s;
}
int main()
{
	long int p = 353;
	cout << "p: " << p << endl;
	long int q = 3;
	cout << "q: " << q << endl;
	n = brute_force(p, q);
	cout << "n " << n << endl;
	long long int fi = brute_force((p - 1), (q - 1));
	cout << "fi: " << fi << endl;
	long long int e = 2;
	while (e < fi) {
		if (gcd(e, fi) == 1)
			break;
		e++;
	}
	cout << "e: " << e << endl;
	public_key = e;
	long long int d = 2;
	while (true) {
		if (brute_force(d, e) % fi == 1)
			break;
		d++;
	}
	cout << "d: " << d << endl;
	private_key = d;
	string message = "Test Message";
	cout << "Initial message:\n" << message;

	// Measure encoding time
	auto start_encode = chrono::high_resolution_clock::now();
	vector<int> coded = encoder(message);
	auto stop_encode = chrono::high_resolution_clock::now();
	auto duration_encode = chrono::duration_cast<chrono::microseconds>(stop_encode - start_encode);
	cout << "\n\nThe encoded message (encrypted by public key)\n";
	for (auto& p : coded)
		cout << p;
	cout << "\nEncoding time: " << duration_encode.count() << " microseconds" << endl;

	// Measure decoding time
	auto start_decode = chrono::high_resolution_clock::now();
	string decoded = decoder(coded);
	auto stop_decode = chrono::high_resolution_clock::now();
	auto duration_decode = chrono::duration_cast<chrono::microseconds>(stop_decode - start_decode);
	cout << "\n\nThe decoded message (decrypted by private key)\n";
	cout << decoded << endl;
	cout << "Decoding time By Brute Force is : " << duration_decode.count() << " microseconds" << endl;

	return 0;
}
