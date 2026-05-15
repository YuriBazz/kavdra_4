//
// Created by george on 15.05.2026.
//
#pragma once
#include "json.hpp"
#include "formats.hpp"
#include <filesystem>
#include <fstream>

namespace task {

struct finder {
    static void find(const std::filesystem::path&);
};
}