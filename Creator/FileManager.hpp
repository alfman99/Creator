#pragma once
#include "pch.h"

class FileManager {
public:
	static pair<BYTE*, DWORD> ReadFileBinary(string pathToFile);
	static void WriteFileBinary(string pathToFile, BYTE* data, DWORD size);
	static void ReplacePayloadStub(const string pathToStub, const vector<BYTE>* payload);
};