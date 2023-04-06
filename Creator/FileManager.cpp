#include "pch.h"
#include "FileManager.hpp"

#include <fstream>

#define RESOURCE_TO_MOD 101

pair<BYTE*,DWORD> FileManager::ReadFileBinary(string pathToFile) {
	ifstream file(pathToFile, ios::binary | ios::ate);
	
	// Get size of file
	streampos size = file.tellg();

	// Allocate memory for file
	BYTE* fileData = new BYTE[size]();

	file.seekg(0, ios::beg);
	file.read((char*)fileData, size);
	file.close();

	return make_pair(fileData, size);
}

void FileManager::WriteFileBinary(string pathToFile, BYTE* data, DWORD size) {
	ofstream file(pathToFile, ios::binary | ios::out);
	file.write((char*)data, size);
	file.close();
}

void FileManager::ReplacePayloadStub(const string pathToStub, const vector<BYTE>* payload) {
	HANDLE hRes = BeginUpdateResource(pathToStub.data(), TRUE);
	UpdateResource(hRes, "P_BIN", MAKEINTRESOURCE(RESOURCE_TO_MOD), NULL, (void*)payload->data(), payload->size());
	EndUpdateResource(hRes, FALSE);
}
