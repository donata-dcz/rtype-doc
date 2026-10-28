/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Loads every asset once and gives it back by name
*/

#include <utility>

#include <Client/ResourceManager.hpp>

namespace Client {

    ResourceManager::ResourceManager(std::filesystem::path assetsDirectory)
        : _assetsDirectory(std::move(assetsDirectory))
    {
    }

    ResourceManager::~ResourceManager()
    {
        UnloadAll();
    }

    const Texture2D &ResourceManager::GetTexture(const std::string &name)
    {
        const auto found = _textures.find(name);

        if (found != _textures.end())
            return found->second;
        const std::filesystem::path path = _assetsDirectory / name;
        const Texture2D texture = LoadTexture(path.string().c_str());

        if (texture.id == 0)
            TraceLog(LOG_ERROR, "RESOURCES: cannot load texture %s", path.string().c_str());
        else
            TraceLog(LOG_INFO, "RESOURCES: texture %s loaded", name.c_str());
        // even if failed its still loaded to avoir infinite loading
        return _textures.emplace(name, texture).first->second;
    }

    void ResourceManager::UnloadAll()
    {
        for (const auto &[name, texture] : _textures)
            UnloadTexture(texture);
        _textures.clear();
    }

} //* namespace Client
