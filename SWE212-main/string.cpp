#include <chrono>
#include <iostream>
#include <cmath>
#include <random>
#include<string>
#include <algorithm>
#include <vector>
using namespace std;
string d;
string e;
string n;

string add(string a, string b) {
    string result = "";
    int carry = 0;

    while (a.size() < b.size()) a = "0" + a;
    while (b.size() < a.size()) b = "0" + b;

    for (int i = a.size() - 1; i >= 0; i--) {
        int temp = (a[i] - '0') + (b[i] - '0') + carry;
        carry = (temp > 9);
        result = to_string(temp % 10) + result;
    }
    if (carry) result = "1" + result;
    return result;
}

string subtract(string a, string b) {
    string result = "";
    int carry = 0;

    while (a.size() < b.size()) a = "0" + a;
    while (b.size() < a.size()) b = "0" + b;

    for (int i = a.size() - 1; i >= 0; i--) {
        int temp = (a[i] - '0') - (b[i] - '0') - carry;
        carry = (temp < 0);
        result = to_string(temp + carry * 10) + result;
    }
    return result;
}

string multiply_single_digit(string a, char b) {
    string result = "0";
    int carry = 0;

    for (int i = a.size() - 1; i >= 0; i--) {
        int temp = (a[i] - '0') * (b - '0') + carry;
        carry = temp / 10;
        result = to_string(temp % 10) + result;
    }
    if (carry) result = to_string(carry) + result;
    return result;
}

string brute_force(string a, string b) {
    string result = "0";
    for (int i = b.size() - 1; i >= 0; i--) {
        string temp = multiply_single_digit(a, b[i]);
        temp += string(b.size() - 1 - i, '0');
        result = add(result, temp);
    }
    return result;
}

string multiply_ten(string a, int b) {
    return a + string(b, '0');
}

string karatsuba(string x, string y) {
    int n = max(x.size(), y.size());
    while (x.size() < n) x.insert(0, "0");
    while (y.size() < n) y.insert(0, "0");

    if (n == 1) return to_string((x[0] - '0') * (y[0] - '0'));

    string a = x.substr(0, n / 2);
    string b = x.substr(n / 2, n - n / 2);
    string c = y.substr(0, n / 2);
    string d = y.substr(n / 2, n - n / 2);
    string ac = karatsuba(a, c);
    string bd = karatsuba(b, d);
    string abcd = karatsuba(add(a, b), add(c, d));
    string adbc = subtract(abcd, add(ac, bd));
    for (int i = 0; i < 2 * (n - n / 2); i++)
        ac += '0';
    for (int i = 0; i < n - n / 2; i++)
        adbc += '0';

    string result = add(add(ac, adbc), bd);
    result.erase(0, min(result.find_first_not_of('0'), result.size()-1));
    return result;
}

typedef long long int ll;

ll gcd(ll a, ll b) {
    if (!a)
        return b;
    return gcd(b % a, a);
}

ll reduceB(ll a, string b) {
    ll mod = 0;
    for (int i = 0; i < b.size(); i++)
        mod = (mod * 10 + b[i] - '0') % a;
    return mod;
}

ll gcdLarge(ll a, string b) {
    ll num = reduceB(a, b);
    return gcd(a, num);
}

string power(string a, string n, string m) {
    if (n == "0")
        return "1";
    else if (n == "1")
        return a;
    else {
        string temp = power(a, to_string(stoll(n) / 2), m);
        if (stoll(n) % 2 == 0)
            return karatsuba(temp, temp);
        else
            return karatsuba(karatsuba(temp, temp), a);
    }
}

string mod(string number, int a) {
    int result = 0;

    // Process all digits of the number
    for (int i = 0; i < number.size(); i++)
        result = (result * 10 + (int)number[i] - '0') % a;

    return to_string(result);
}
string modInverse(string a, string m) {
    string aModM = mod(a, stoll(m));
    // cout << "aModM: " << aModM << endl;
    for (ll x = 1; x < stoll(m); x++){
        // cout << "x: " << x << endl;
       if (mod(karatsuba(aModM, to_string(x)), stoll(m)) == "1")
          return to_string(x);
    }
    return "1";
}
string encrypt(int message)
{
	long int ee = stoll(e);
	string encrpyted_text = "1";
	while (ee--) {//message^public_key % n
		encrpyted_text = karatsuba(encrpyted_text,to_string(message));
		encrpyted_text = mod(encrpyted_text, stoll(n));
	}

	return encrpyted_text;
}
vector<string> encoder(string message)
{
	vector<string> form;
	for (char letter : message) {
		form.push_back(encrypt((int)letter));
	}
	return form;
}
string findSmallestE(string phi) {
    string e = "2";
    while (gcdLarge(stoll(e), (char*)phi.c_str()) != 1) {
        e = to_string(stoll(e) + 1);
    }
    return e;
}
std::string decrypt(int encrpyted_text)
{
	long int dd = stoll(d);
	std::string decrypted = "1";
	while (dd--) {//encrpyted_text^private_key % n
        decrypted = karatsuba(to_string(encrpyted_text), decrypted);
        decrypted = mod(decrypted, stoll(n));
	}
	return decrypted;
}
string decoder(vector<string> encoded)
{
	string s;
	// calling the decrypting function decoding function
	for (string num : encoded)
		s += (char)stoi(decrypt(stoll(num)));
	return s;
}

void RSA(string p, string q) {
    n = karatsuba(p, q);
    cout << "p: " << p << endl;
    cout << "q: " << q << endl;
    cout << "n: " << n << endl;

    // Compute Euler's totient function
    string phi = karatsuba(subtract(p, "1"), subtract(q, "1"));
    cout << "fi: " << phi << endl;

    // Find a random encryption exponent e
    e = findSmallestE(phi);
    cout << "e "<<e << endl;
    // Compute the private key d
    d = modInverse(e, phi);

    // Output the public key (n, e) and private key (n, d)
    cout << "Public key: (" << n << ", " << e << ")" << endl;
    cout << "Private key: (" << n << ", " << d << ")" << endl;
    
	string message = "Test Message";
	cout << "Initial message:\n" << message;
	vector<string> coded = encoder(message);
	cout << "\n\nThe encoded message(encrypted by public key)\n";
	for (auto& p : coded)
		cout << p;
	cout << "\n\nThe decoded message(decrypted by private "
		"key)\n";
	cout << decoder(coded) << endl;


}

int main() {
    string a = "173";
    string b = "137";
    RSA(a, b);
    auto start = chrono::high_resolution_clock::now();
    string result_karatsuba = karatsuba(a, b);
    auto stop = chrono::high_resolution_clock::now();
    auto duration_karatsuba = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Karatsuba time: " << duration_karatsuba.count() << " microseconds" << endl;

    start = chrono::high_resolution_clock::now();
    string result_brute_force = brute_force(a, b);
    stop = chrono::high_resolution_clock::now();
    auto duration_brute_force = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Brute force time: " << duration_brute_force.count() << " microseconds" << endl;

    return 0;
    
}
/*
Karatsuba time: 558813 microseconds
Brute force time: 663341 microseconds
*/