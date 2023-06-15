#include "pch.h"

#include "Cryptography.hpp"
#include "FileManager.hpp"
#include "ServerRequests.hpp"

enum ReturnCodes {
    SUCCESS = 0,
	ERROR_ORIGINAL_PE_NOT_FOUND = -1,
    ERROR_STUB_NOT_FOUND = -2,
    ERROR_EXE_TO_DLL = -3,
    ERROR_REGISTER_PROJECT = -4,
};

string getFullPathToExecutable() {
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	return string(buffer).substr(0, pos);
}

int main(int argc, char** argv) {

    if (argc != 5) {
		cout << "Usage: Creator.exe <original PE> <stub path> <path output> <API_KEY>" << endl;
		return 0;
	}

    string originalPEPath(argv[1]);
    string stubPath(argv[2]);
    string outputPath(argv[3]);
    string API_KEY(argv[4]);

    // Check if original PE exists
    if (!FileManager::FileExists(originalPEPath)) {
		cout << "Error: Original PE not found" << endl;
		return ERROR_ORIGINAL_PE_NOT_FOUND;
	}

    // Check if stub exists
    if (!FileManager::FileExists(stubPath)) {
        cout << "Error: Stub not found" << endl;
        return ERROR_STUB_NOT_FOUND;
    }

    // Exe to dll
    // Run program to convert exe to dll
    // Path of program: Creator\ExeToDll\ExeToDll.exe
    string command;
#ifdef _DEBUG
    command += "X:\\Carrera\\__TFG\\development\\Creator\\Release\\exe_to_dll.exe ";
#else
    command += getFullPathToExecutable() + "\\exe_to_dll.exe ";
#endif
    command += originalPEPath + " temp.dll";

    int retCode = system(command.data());

    if (retCode < 0) {
		cout << "Error: Exe to dll" << endl;
		return ERROR_EXE_TO_DLL;
	}

    // Register project on server
    ResponseRegisterProject* registerResponse = ServerRequests::RegisterProject(API_KEY);

    if (registerResponse == nullptr) {
        cout << "Error: Register project" << endl;
        // Delete temp.dll
        system("del /f temp.dll");
        return ERROR_REGISTER_PROJECT;
    }

    // Original EXE, need his OEP
    pair<BYTE*, DWORD> originalPE = FileManager::ReadFileBinary(originalPEPath);

    // Additional data to add to stub
    Payload addData;
    strcpy_s(addData.projectId, registerResponse->projectID);
    addData.OEP = FileManager::GetOEPFromBYTES(originalPE.first);

    // Create Cryptography object
    Cryptography crypt(*registerResponse);

    // Read file to crypt; temp.dll is the dll created by ExeToDll.exe
    pair<BYTE*, DWORD> file = FileManager::ReadFileBinary("temp.dll");

    // Invisible watermark original file
    FileManager::InvisibleWatermark(file, addData);
        
    // Crypt file
    vector<BYTE>* cryptedVector = crypt.Crypt(file.first, file.second);

    // Copy stub to output path
    pair<BYTE*, DWORD> stubFile = FileManager::ReadFileBinary(stubPath);
    FileManager::WriteFileBinary(outputPath, stubFile.first, stubFile.second);

    // Replace payload on output stub and OEP 
    FileManager::ReplaceDataPayloadStub(outputPath, cryptedVector, addData);

    // Clear memory
    delete file.first;
    delete cryptedVector;

    // Delete temp.dll
    system("del /f temp.dll");

    return SUCCESS;
}
