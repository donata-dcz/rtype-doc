/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Loads every asset once and gives it back by name
*/

#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

#include <raylib.h>

namespace Client {

    //& Asset names are paths relative to the assets folder
    class ResourceManager
    {
    public:
        explicit ResourceManager(std::filesystem::path assetsDirectory);
        ~ResourceManager();

        ResourceManager(const ResourceManager &) = delete;
        ResourceManager &operator=(const ResourceManager &) = delete;

        const Texture2D &GetTexture(const std::string &name);
        void UnloadAll();

    private:
        std::filesystem::path _assetsDirectory;
        std::unordered_map<std::string, Texture2D> _textures;
    };
}
