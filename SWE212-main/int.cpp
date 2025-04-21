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
long long int brute_force(long int a, long int b) {
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
long long int karatsuba(long int x, long int y) {
	int n = max(to_string(x).size(), to_string(y).size());
	// base case
	if (n < 2)
		return brute_force(x, y);
	//divide 
	/*new n*/n = (n / 2) + (n % 2);
	long int multiplier = pow(10, n);
	long int b = x / multiplier;
	long int a = x - brute_force(b,multiplier);
	long int d = y / multiplier;
	long int c = y - brute_force(d,multiplier);
	long long int ac = karatsuba(a, c);
	long long int bd = karatsuba(b, d);
	long long int abcd = karatsuba((a + b), (c + d));
	//combine
	return ac + (brute_force((abcd - ac - bd), multiplier)) + brute_force(bd, (pow(10, 2 * n)));
}
long long int encryptB(double message)
{
	long int e = public_key;
	long long int encrpyted_text = 1;
	while (e--) {//message^public_key % n
		encrpyted_text = brute_force(encrpyted_text, message);
		encrpyted_text %= n;
	}
	return encrpyted_text;
}
long long int decryptB(int encrpyted_text)
{
	long int d = private_key;
	long long int decrypted = 1;
	while (d--) {//encrpyted_text^private_key % n
		decrypted *= encrpyted_text;
		decrypted %= n;
	}
	return decrypted;
}
long long int encryptK(double message)
{
	long int e = public_key;
	long long int encrpyted_text = 1;
	while (e--) {//message^public_key % n
		encrpyted_text *= message;
		encrpyted_text %= n;
	}
	return encrpyted_text;
}
long long int decryptK(int encrpyted_text)
{
	long int d = private_key;
	long long int decrypted = 1;
	while (d--) {//encrpyted_text^private_key % n
		decrypted *= encrpyted_text;
		decrypted %= n;
	}
	return decrypted;
}
vector<int> encoderB(string message)
{
	vector<int> form;
	for (char letter : message) {
		form.push_back(encryptB((int)letter));
	}
	return form;
}
vector<int> encoderK(string message)
{
	vector<int> form;
	for (char letter : message) {
		form.push_back(encryptK((int)letter));
	}
	return form;
}
string decoderB(vector<int> encoded)
{
	string s;
	// calling the decrypting function decoding function
	for (int num : encoded)
		s += decryptB(num);
	return s;
}
string decoderK(vector<int> encoded)
{
	string s;
	// calling the decrypting function decoding function
	for (int num : encoded)
		s += decryptK(num);
	return s;
}
void RSAB(int p, int q) {
	// Measure encoding time
	cout << "Brute Force\n";
	auto start_key = chrono::high_resolution_clock::now();
	cout << "p: " << p << endl;
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
	auto stop_key = chrono::high_resolution_clock::now();
	auto duration_key= chrono::duration_cast<chrono::microseconds>(stop_key - start_key);
	cout << "The encoded message (encrypted by public key)\n";
	cout << "Genarting keys time: " << duration_key.count() << " microseconds" << endl;
	string message = "Test Message";
	cout << "Initial message:\n" << message << endl;

	// Measure encoding time
	auto start_encode = chrono::high_resolution_clock::now();
	vector<int> coded = encoderB(message);
	auto stop_encode = chrono::high_resolution_clock::now();
	auto duration_encode = chrono::duration_cast<chrono::microseconds>(stop_encode - start_encode);
	cout << "The encoded message (encrypted by public key)\n";
	for (auto& p : coded)
		cout << p;
	cout << "\nEncoding time: " << duration_encode.count() << " microseconds" << endl;

	// Measure decoding time
	auto start_decode = chrono::high_resolution_clock::now();
	string decoded = decoderB(coded);
	auto stop_decode = chrono::high_resolution_clock::now();
	auto duration_decode = chrono::duration_cast<chrono::microseconds>(stop_decode - start_decode);
	cout << "The decoded message (decrypted by private key)\n";
	cout << decoded << endl;
	cout << "Decoding time By Brute Force is : " << duration_decode.count() << " microseconds" << endl;

}
void RSAK(int p, int q) {
	cout << "Karatsuba\n";
	auto start_key = chrono::high_resolution_clock::now();
	cout << "p: " << p << endl;
	cout << "q: " << q << endl;
	n = karatsuba(p,q);
	cout << "n " << n << endl;
	long long int fi = karatsuba((p - 1), (q - 1));
	cout << "fi: " << fi << endl;
	long long int e = 2;
	while (e < fi) { 
		if (gcd(e, fi) == 1)
			break;
		e++;
	}
	cout << "e: " << e<<endl;
	public_key = e;
	long long int d = 2;
	while (true) {
		if (karatsuba(d , e) % fi == 1)
			break;
		d++;
	}
	cout << "d: " << d << endl;
	private_key = d;
	auto stop_key = chrono::high_resolution_clock::now();
	auto duration_key= chrono::duration_cast<chrono::microseconds>(stop_key - start_key);
	cout << "The encoded message (encrypted by public key)\n";
	cout << "Genarting keys time: " << duration_key.count() << " microseconds" << endl;
	string message = "Test Message";
	cout << "Initial message:\n" << message;

	auto start_encode = chrono::high_resolution_clock::now();
	vector<int> coded = encoderK(message);
	auto stop_encode = chrono::high_resolution_clock::now();
	auto duration_encode = chrono::duration_cast<chrono::microseconds>(stop_encode - start_encode);
	cout << "The encoded message (encrypted by public key)\n";
	for (auto& p : coded)
		cout << p;
	cout << "\nEncoding time: " << duration_encode.count() << " microseconds" << endl;

	// Measure decoding time
	auto start_decode = chrono::high_resolution_clock::now();
	string decoded = decoderK(coded);
	auto stop_decode = chrono::high_resolution_clock::now();
	auto duration_decode = chrono::duration_cast<chrono::microseconds>(stop_decode - start_decode);
	cout << "The decoded message (decrypted by private key)\n";
	cout << decoded << endl;
	cout << "Decoding time By karatsuba is : " << duration_decode.count() << " microseconds" << endl;
}
int main()
{
	int p = 13;
	int q = 17;
	RSAB(p,q);
	RSAK(p,q);
	return 0;
}
