#include "pch.h"
#include "Cryptography.hpp"

Cryptography::Cryptography(ResponseRegisterProject data) {
	this->values = data;
}

vector<BYTE>* Cryptography::Crypt(const BYTE* data, const DWORD size) {
	// Crypt data
	const unsigned long encryptedSize = plusaes::get_padded_encrypted_size(size);
	vector<BYTE>* encrypted = new vector<BYTE>(encryptedSize);

	if (plusaes::Error::kErrorOk != plusaes::encrypt_cbc(data, size, this->values.key, KEY_SIZE, (const unsigned char(*)[16]) this->values.iv, encrypted->data(), encrypted->size(), true)) {
		cout << "Error while encrypting data" << endl;
	}
	
	// Return encrypted data
	return encrypted;
}
