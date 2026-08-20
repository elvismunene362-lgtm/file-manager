// handles printing current folder path,listing files & changing directions
#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct FileManager {
    fs::path current_path;

    std::string              getCurrentPath() const;
    std::vector<std::string> getEntries() const;
    void                     printcurrentpath() const;
    void                     listdirectory() const;
    void                     changedirectory(const std::string& target);
    void                     encryptfile(const std::string& filename, std::string key);
    void                     decryptfile(const std::string& filename, std::string& key);
};
