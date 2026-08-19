//handles printing current folder path,listing files & changing directions
#pragma once
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;
 class FileManager{
private:
    fs::path current_path;
public:
    FileManager();
    std::string getCurrentPath() const;
    std::vector<std::string> getEntries() const;
    void printcurrentpath() const;
    void listdirectory() const;
    void changedirectory(const std::string& target);
    void encryptfile(const std::string& filename, std::string key);
    void decryptfile(const std::string& filename, std::string& key);
 };