//
// Created by george on 15.05.2026.
//
#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

namespace task {

enum file_type : uint8_t {
    undefined,
    video,
    audio,
    image,
};

struct extension_to_format {
    static std::unordered_map<std::string, file_type> extension_to_type_map;
    static std::string type_to_string(file_type);
};
}
