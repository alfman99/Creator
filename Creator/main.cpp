#include "pch.h"

#include "Cryptography.hpp"
#include "FileManager.hpp"

int main(int argc, char** argv) {

    if (argc != 4) {
		cout << "Usage: Creator.exe <original PE> <stub path> <path output>" << endl;
		return 0;
	}

    string originalPEPath(argv[1]);
    string stubPath(argv[2]);
    string outputPath(argv[3]);

    // Generate mock key and iv
    // Same as mock key and iv in server
    vector<BYTE> keyCrypt(KEY_SIZE, 0);
    vector<BYTE> ivCrypt(IV_SIZE, 0);
    for(int i = 0; i < KEY_SIZE; i++) keyCrypt[i] = i;
    for(int i = 0; i < IV_SIZE; i++) ivCrypt[i] = IV_SIZE-(i+1);

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
    cout << system("dir") << endl;

    if (retCode < 0) {
		cout << "Error: Exe to dll" << endl;
		return -1;
	}

    // Create Cryptography object
    Cryptography crypt(keyCrypt, ivCrypt);

    // Read file to crypt; temp.dll is the dll created by ExeToDll.exe
    pair<BYTE*, DWORD> file = FileManager::ReadFileBinary("temp.dll");

    // Invisible watermark original file
    FileManager::InvisibleWatermark(file);
        
    // Crypt file
    vector<BYTE>* cryptedVector = crypt.Crypt(file.first, file.second);

    // Copy stub to output path
    pair<BYTE*, DWORD> stubFile = FileManager::ReadFileBinary(stubPath);
    FileManager::WriteFileBinary(outputPath, stubFile.first, stubFile.second);

    // Additional data to add to stub
    Payload addData;
    strcpy_s(addData.projectId, "12345678901234567890\0");
    addData.OEP = FileManager::GetOEPFromBYTES(file.first);

    // Replace payload on output stub and OEP 
    FileManager::ReplaceDataPayloadStub(outputPath, cryptedVector, addData);

    // Clear memory
    delete file.first;
    delete cryptedVector;

    system("del /f temp.dll");

    return 0;
}
