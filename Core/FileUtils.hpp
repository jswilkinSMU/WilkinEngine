#pragma once
#include <vector>
#include <string>
// -----------------------------------------------------------------------------
// Defining error constants for readability
const int FILE_SUCCESS = 0;
const int FILE_OPEN_ERROR = 1;
const int FILE_SEEK_ERROR = 2;
const int FILE_READ_ERROR = 3;
const int FILE_TELL_ERROR = 4;
const int FILE_WRITE_ERROR = 5;
// -----------------------------------------------------------------------------
int FileReadToBuffer(std::vector<uint8_t>& outBuffer, std::string const& filename);
int FileReadToString(std::string& out_string, std::string const& filename);
int WriteBufferToFile(std::vector<uint8_t>& inBuffer, std::string const& filename);
bool DoesFileExist(std::string const& filename);
bool DoesFolderExist(std::string const& folderName);
bool CreateFolder(std::string const& folderName);
// -----------------------------------------------------------------------------