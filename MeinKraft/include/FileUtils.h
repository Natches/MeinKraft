#pragma once

class fstream;
class stringstream;

void CheckIfFolderExist(const char* path);
void ResetCursorPos(std::fstream& file);
void ResetCursorPos(std::stringstream& file);
void JumpToNext(std::stringstream& fileStream);
void FindSTR(std::stringstream& sstr, std::string& str);
