#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include "file_manager.hpp"
#endif

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

void FileManager::printcurrentpath() const
{
    std::cout << "\nCurrent Directory: " << current_path.string() << "\n>";
}

void FileManager::listdirectory() const
{
    std::cout << "\n--- Files and Folders ---\n";
    for (const auto& entry : fs::directory_iterator(current_path)) {
        std::string type = entry.is_directory() ? "[DIR] " : "[FILE] ";
        std::cout << type << entry.path().filename().string() << "\n";
    }
}

void FileManager::changedirectory(const std::string& target)
{
    fs::path new_path = current_path / target;
    if (target == "..") {
        current_path = current_path.parent_path();
    } else if (fs::exists(new_path) && fs::is_directory(new_path)) {
        current_path = fs::canonical(new_path);
    } else {
        std::cout << "Error: Directory does not exist.\n";
    }
}

void FileManager::encryptfile(const std::string& filename, std::string key)
{
    if (key.empty()) return;

    fs::path filepath = current_path / filename;
    if (!fs::exists(filepath) || fs::is_directory(filepath)) {
        std::cout << "Error: File not found.\n";
        return;
    }

    std::fstream in;
    std::fstream out;
    in.open(filepath, std::fstream::in);
    fs::path outpath = filepath;
    outpath += fs::path(".enc");
    out.open(outpath, std::fstream::out);

    char c;
    int  k = 0;
    while (in >> std::noskipws >> c) {
        int tmp = (c + key[k % key.length()]);
        out << (char) tmp;
        k++;
    }
    in.close();
    out.close();
    std::cout << "File successfully encrypted!\n";
}

void FileManager::decryptfile(const std::string& filename, std::string& key)
{
    if (key.empty()) return;

    fs::path filepath = current_path / filename;
    if (!fs::exists(filepath) || fs::is_directory(filepath)) {
        std::cout << "Error: File not found.\n";
        return;
    }

    std::fstream in;
    std::fstream out;
    in.open(filepath, std::fstream::in);
    fs::path outpath = filepath.stem(); // strip ".enc" extension
    out.open(outpath, std::fstream::out);

    char c;
    int  k = 0;
    while (in >> std::noskipws >> c) {
        int tmp = (c - key[k % key.length()]);
        out << (char) tmp;
        k++;
    }
    in.close();
    out.close();
    std::cout << "File successfully decrypted!\n";
}

std::string FileManager::getCurrentPath() const { return current_path.string(); }

std::vector<std::string> FileManager::getEntries() const
{
    std::vector<std::string> entries;
    entries.push_back("..");

    try {
        for (const auto& entry : fs::directory_iterator(current_path)) {
            entries.push_back(entry.path().filename().string());
        }
    } catch (const fs::filesystem_error&) {
    }
    std::sort(entries.begin() + 1, entries.end(),
              [this](const std::string& a, const std::string& b) {
                  bool a_is_dir = fs::is_directory(current_path / a);
                  bool b_is_dir = fs::is_directory(current_path / b);
                  if (a_is_dir != b_is_dir) return a_is_dir > b_is_dir;
                  return a < b;
              });
    return entries;
}
