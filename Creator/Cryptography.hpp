#pragma once
#include "pch.h"

#define KEY_SIZE 16
#define IV_SIZE 16

class Cryptography {
private:
	vector<BYTE> key;
	vector<BYTE> iv;

public:
	Cryptography(vector<BYTE> key, vector<BYTE> iv);

	vector<BYTE>* Crypt(const BYTE* data, const DWORD size);
};

