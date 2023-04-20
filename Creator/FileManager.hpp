#pragma once
#include "pch.h"

struct Payload {
	char projectId[21];
	DWORD OEP;
};

class FileManager {
public:
	static pair<BYTE*, DWORD> ReadFileBinary(string pathToFile);
	static void WriteFileBinary(string pathToFile, BYTE* data, DWORD size);

	static DWORD GetOEPFromBYTES(const BYTE* payload);

	static void ReplaceDataPayloadStub(const string pathToStub, const vector<BYTE>* payload, const Payload OEP);

	static void InvisibleWatermark(pair<BYTE*, DWORD> file);

	static bool FileExists(const string& name);
};