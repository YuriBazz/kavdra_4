//
// Created by george on 15.05.2026.
//
#include "finder.hpp"
void task::finder::find(const std::filesystem::path &path) {
    namespace fs =  std::filesystem;
    using js = nlohmann::json;

    if (!fs::exists(path)) {
        throw std::runtime_error("Path doesn't exists");
    }

    if (!fs::is_directory(path)) {
        throw std::runtime_error("Given path is not the path to a directory");
    }

    fs::path home = std::getenv("HOME");
    fs::path media_files = home / ".media_files";
    std::fstream file(media_files, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        throw std::runtime_error("File \".media_files\" could not be created/opened");
    }

    js json;
    for (const auto &dir_iter : fs::directory_iterator(path)) {
        // т.к. нигде не сказано, что нужно обрабатывать симлинки
        auto status = dir_iter.status();
        if (!fs::is_regular_file(status)) continue;
        auto type = extension_to_format::extension_to_type_map.find(dir_iter.path().extension());
        if (type == extension_to_format::extension_to_type_map.end()) continue;
        json[extension_to_format::type_to_string(type->second)].push_back(dir_iter.path().filename());
    }

    file << json << std::endl;
}
