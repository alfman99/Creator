#include "pch.h"

#include "Cryptography.hpp"
#include "FileManager.hpp"
#include "ServerRequests.hpp"

int main(int argc, char** argv) {

    if (argc != 4) {
		cout << "Usage: Creator.exe <original PE> <stub path> <path output>" << endl;
		return 0;
	}

    string originalPEPath(argv[1]);
    string stubPath(argv[2]);
    string outputPath(argv[3]);

    // Exe to dll
    // Run program to convert exe to dll
    // Path of program: Creator\ExeToDll\ExeToDll.exe
    string command;
#ifdef _DEBUG
    command += "X:\\Carrera\\__TFG\\development\\__Protector\\Creator\\Release\\exe_to_dll.exe ";
#else
    command += "exe_to_dll.exe ";
#endif
    command += originalPEPath + " temp.dll";

    int retCode = system(command.data());

    if (retCode < 0) {
		cout << "Error: Exe to dll" << endl;
		return -1;
	}

    // Get API_KEY from client
    string API_KEY;

    cout << "Enter API_KEY: ";
    cin >> API_KEY;

    // Register project on server
    ResponseRegisterProject* registerResponse = ServerRequests::RegisterProject(API_KEY);

    if (registerResponse == nullptr) {
        cout << "Error: Register project" << endl;
        return -1;
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
    FileManager::InvisibleWatermark(file);
        
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

    system("del /f temp.dll");

    return 0;
}
