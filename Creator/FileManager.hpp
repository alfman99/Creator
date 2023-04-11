#pragma once
#include "pch.h"

class FileManager {
public:
	static pair<BYTE*, DWORD> ReadFileBinary(string pathToFile);
	static void WriteFileBinary(string pathToFile, BYTE* data, DWORD size);

	static DWORD GetOEPFromBYTES(const BYTE* payload);

	static void ReplaceDataPayloadStub(const string pathToStub, const vector<BYTE>* payload, const DWORD OEP);

	static void InvisibleWatermark(pair<BYTE*, DWORD> file);
};