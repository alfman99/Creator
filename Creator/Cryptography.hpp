#pragma once
#include "pch.h"

#define PROJECT_ID_SIZE 21
#define KEY_SIZE 16
#define IV_SIZE 16

struct ResponseRegisterProject {
	char projectID[PROJECT_ID_SIZE];
	BYTE key[KEY_SIZE];
	BYTE iv[IV_SIZE];
};

class Cryptography {
private:
	ResponseRegisterProject values;

public:
	Cryptography(ResponseRegisterProject data);

	vector<BYTE>* Crypt(const BYTE* data, const DWORD size);
};

