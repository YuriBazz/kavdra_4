//
// Created by george on 15.05.2026.
//

#include "formats.hpp"

std::unordered_map<std::string, task::file_type> task::extension_to_format::extension_to_type_map =
{
    // IMAGES
    {".jpg", image},
    {".png", image},
    {".jpeg", image},
    {".webp", image},
    {".gif", image},
    {".tif", image},
    {".bmp", image},
    {".eps", image},

    // AUDIO
    {".mp3", audio},
    {".wma", audio},
    {".snd", audio},
    {".wav", audio},
    {".ra", audio},
    {".au", audio},
    {".aac", audio},

    //VIDEOS
    {".mp4", video},
    {".3gp", video},
    {".avi", video},
    {".mpg", video},
    {".mov", video},
    {".wmv", video}

};
std::string task::extension_to_format::type_to_string(file_type type) {
#define TO_STRING(name) case name : return #name;
    switch (type) {
        TO_STRING(video)
        TO_STRING(audio)
    case image : return "images"; // не консистентно как-то получается
        default: return "undefined";
    }
#undef TO_STRING
}
