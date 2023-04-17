#include "pch.h"
#include "FileManager.hpp"

#include <fstream>

#define RESOURCE_TO_MOD 101
#define RESOURCE_TO_MOD2 102

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

DWORD FileManager::GetOEPFromBYTES(const BYTE* payload) {
	// Get PE header
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)payload;
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)(payload + dosHeader->e_lfanew);

	// Get OEP
	DWORD OEP = ntHeader->OptionalHeader.AddressOfEntryPoint;

	return OEP;
}

void FileManager::ReplaceDataPayloadStub(const string pathToStub, const vector<BYTE>* payload, const Payload addData) {
	HANDLE hRes = BeginUpdateResource(pathToStub.data(), TRUE);
	UpdateResource(hRes, "P_BIN", MAKEINTRESOURCE(RESOURCE_TO_MOD), NULL, (void*)payload->data(), payload->size());
	UpdateResource(hRes, "P_BIN", MAKEINTRESOURCE(RESOURCE_TO_MOD2), NULL, (void*)&addData, sizeof(Payload));
	EndUpdateResource(hRes, FALSE);
}

void FileManager::InvisibleWatermark(pair<BYTE*, DWORD> file) {
	// Create an invisible watermark in the file

	
}
