#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include <filesystem>

int FileReadToBuffer(std::vector<uint8_t>& outBuffer, std::string const& filename)
{
	FILE* file = nullptr;

	// Open the file
	if (fopen_s(&file, filename.c_str(), "rb") != 0 || file == nullptr)
	{
		ERROR_RECOVERABLE("Failed to open file " + filename);
		return FILE_OPEN_ERROR;
	}

	// Seek to the end of the file
	if (fseek(file, 0, SEEK_END) != 0)
	{
		ERROR_RECOVERABLE("Failed to reach end of file " + filename);
		fclose(file);
		return FILE_SEEK_ERROR;
	}

	// Tell file size
	long fileSize = ftell(file);
	if (fileSize < 0)
	{
		ERROR_RECOVERABLE("Failed to tell file size " + filename);
		fclose(file);
		return FILE_TELL_ERROR;
	}
	outBuffer.resize(fileSize);

	// Seek back to the beginning of the file
	if (fseek(file, 0, SEEK_SET) != 0)
	{
		ERROR_RECOVERABLE("Failed to seek back to beginning of the file " + filename);
		fclose(file);
		return FILE_SEEK_ERROR;
	}

	// Read file to buffer
	size_t readBytes = fread(outBuffer.data(), 1, fileSize, file);
	if (readBytes != static_cast<size_t>(fileSize))
	{
		ERROR_RECOVERABLE("Failed to read the file " + filename);
		fclose(file);
		return FILE_READ_ERROR;
	}
	fclose(file);

	return FILE_SUCCESS;
}

int FileReadToString(std::string& out_string, std::string const& filename)
{
	std::vector<uint8_t> buffer;
	int fileReadResult = FileReadToBuffer(buffer, filename);

	// Check for success
	if (fileReadResult != FILE_SUCCESS)
	{
		return fileReadResult;
	}

	buffer.push_back(0);
	out_string = std::string(reinterpret_cast<const char*>(buffer.data()), buffer.size());

	return FILE_SUCCESS;
}

int WriteBufferToFile(std::vector<uint8_t>& inBuffer, std::string const& filename)
{
	FILE* file = nullptr;

	// Opening the file
	if (fopen_s(&file, filename.c_str(), "wb") != 0 || file == nullptr)
	{
		ERROR_RECOVERABLE("Failed to open file " + filename);
		return FILE_OPEN_ERROR;
	}

	// Writing the bytes to the file
	size_t writeBytes = fwrite(inBuffer.data(), 1, inBuffer.size(), file);
	if (writeBytes != inBuffer.size())
	{
		ERROR_RECOVERABLE("Failed to write the file " + filename);
		fclose(file);
		return FILE_WRITE_ERROR;
	}

	// Closing the file and return success
	fclose(file);
	return FILE_SUCCESS;
}

bool DoesFileExist(std::string const& filename)
{
	return std::filesystem::exists(filename);
}

bool DoesFolderExist(std::string const& folderName)
{
	return std::filesystem::is_directory(folderName);
}

bool CreateFolder(std::string const& folderName)
{
	if (!std::filesystem::create_directory(folderName))
	{
		ERROR_RECOVERABLE(Stringf("Could not create folder: \"s\"", folderName.c_str()));
		return false;
	}
	return true;
}

