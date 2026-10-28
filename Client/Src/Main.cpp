/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** main client
*/

#include <charconv>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <system_error>

#include <raylib.h>

#include <Client/Audio.hpp>
#include <Client/Keyboard.hpp>
#include <Client/Menu.hpp>
#include <Client/ResourceManager.hpp>
#include <Client/ServerConnection.hpp>
#include <Client/Menu.hpp>
#include <Client/SnapshotRenderer.hpp>
#include <Client/Starfield.hpp>
#include <Client/StarfieldRenderer.hpp>
#include <Network/Protocol.hpp>

namespace {

    //^ same seed for every player -> same starfield
    constexpr std::uint32_t StarfieldSeed = 42;
    constexpr float ShootSoundDelay = 0.15F;

    //^ r-type_client -h <host> -p <port> pre-fills an online connection.
    struct Options {
        std::string host{"127.0.0.1"};
        std::uint16_t port = Common::Network::DefaultPort;
    };

    bool ParsePort(std::string_view text, std::uint16_t &port)
    {
        unsigned int value = 0;
        const char *const last = text.data() + text.size();
        const auto parsed = std::from_chars(text.data(), last, value);

        if (parsed.ec != std::errc{} || parsed.ptr != last || value < 1 || value > 65535)
            return false;
        port = static_cast<std::uint16_t>(value);
        return true;
    }

    std::optional<Options> ParseOptions(int argc, char **argv)
    {
        Options options;

        for (int i = 1; i < argc; i += 2) {
            const std::string_view flag = argv[i];
            const bool hasValue = i + 1 < argc;
            bool valid = false;

            if (hasValue && flag == "-h") {
                options.host = argv[i + 1];
                valid = !options.host.empty();
            } else if (hasValue && flag == "-p") {
                valid = ParsePort(argv[i + 1], options.port);
            }
            if (!valid) {
                std::cerr << "Usage: " << argv[0] << " [-h <host>] [-p <port>] (port: 1-65535)\n";
                return std::nullopt;
            }
        }
        return options;
    }

    const char *StatusText(const Client::ServerConnection &server)
    {
        switch (server.GetStatus()) {
            case Client::ConnectionStatus::Connecting:
                return "Connecting...";
            case Client::ConnectionStatus::Connected:
                return TextFormat("Connected as player %d", static_cast<int>(server.GetPlayerId()));
            case Client::ConnectionStatus::Rejected:
                if (server.GetRejectReason() == rtype::Network::RejectReason::ServerFull)
                    return "Rejected: server is full (4 players max)";
                return "Rejected: wrong protocol version";
            case Client::ConnectionStatus::Unreachable:
                return "The server does not answer";
            case Client::ConnectionStatus::Lost:
                return "Connection Lost";
        }
        return "";
    }

    bool IsPlayerDead(const Client::ServerConnection &server)
    {
        const auto &snapshot = server.GetSnapshot();

        if (!snapshot.has_value())
            return false;
        const auto myId = static_cast<std::uint8_t>(server.GetPlayerId());

        for (const rtype::SnapshotPlayer &player : snapshot->players) {
            if (player.id == myId)
                return !player.alive && player.lives == 0;
        }
        return false;
    }

    void PlayShootSound(Audio::AudioSystem &audio, bool shooting, float &cooldown)
    {
        if (!shooting || cooldown > 0.0F)
            return;
        audio.Play(Audio::Effect::Shoot);
        cooldown = ShootSoundDelay;
    }
}

int main(int argc, char **argv)
{
    const std::optional<Options> options = ParseOptions(argc, argv);
    std::unique_ptr<Client::ServerConnection> server;

    if (!options.has_value())
        return 84;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "R-Type");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    Client::Starfield starfield(static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()), Client::Starfield::DefaultLayers(), StarfieldSeed);
    Client::ResourceManager resources(std::filesystem::path(GetApplicationDirectory()) / "Assets");
    Client::SnapshotRenderer renderer(resources);
    Client::Settings settings;
    Client::Menu menu(settings);
    menu.SetLogo(&resources.GetTexture("Title/RTypeTitle.png"));
    menu.SetConnectionDefaults(options->host, options->port);

    Audio::AudioSystem audio;
    audio.Init((std::filesystem::path(GetApplicationDirectory()) / "Assets" / "Sounds").string());
    SetAudioStreamBufferSizeDefault(8192);
    audio.SetEffectsVolume(0.1F);

    bool playing = false;
    bool wasDead = false;
    float shootCooldown = 0.0F;

    while (!WindowShouldClose()) {
        const float deltaTime = GetFrameTime();
        starfield.Update(deltaTime);
        if (server != nullptr)
            server->Update(deltaTime);
        audio.SetMasterVolume(static_cast<float>(settings.masterVolumePercent) / 100.0F);
        audio.Update();
        shootCooldown -= deltaTime;

        if (!playing) {
            audio.PlayTitle();
            const Client::MenuResult result = menu.Update(GetScreenWidth(), GetScreenHeight());
            if (result.action == Client::MenuAction::Quit)
                break;
            if (result.action == Client::MenuAction::Connect) {
                server.reset();
                try {
                    server = std::make_unique<Client::ServerConnection>(
                        result.request.host, result.request.port, result.request.playerName);
                } catch (const std::system_error &error) {
                    menu.SetConnectionStatus(std::string("Connection failed: ") + error.what());
                }
            }
            if (server != nullptr) {
                menu.SetConnectionStatus(StatusText(*server));
                if (server->GetStatus() == Client::ConnectionStatus::Connected) {
                    playing = true;
                    wasDead = false;
                    audio.PlayStage(0);
                }
            }
        } else if (IsKeyPressed(KEY_ESCAPE) || server->GetStatus() == Client::ConnectionStatus::Lost) {
            const bool lost = server->GetStatus() == Client::ConnectionStatus::Lost;

            playing = false;
            server.reset();
            menu.Reset();
            if (lost)
                menu.SetConnectionStatus("Connection Lost");
        } else {
            const auto keyboard = Client::ReadKeyboard();

            server->SendInput(keyboard);
            PlayShootSound(audio, keyboard.IsActive(rtype::Action::Shoot), shootCooldown);
            const bool dead = IsPlayerDead(*server);
            if (dead && !wasDead)
                audio.PlayGameOver();
            wasDead = dead;
        }
        BeginDrawing();
        ClearBackground(BLACK);
        Client::DrawStarfield(starfield);
        if (playing && server != nullptr && server->GetSnapshot().has_value()) {
            renderer.Draw(*server->GetSnapshot(), server->GetPlayerId(), deltaTime);
        } else {
            menu.Draw(GetScreenWidth(), GetScreenHeight());
        }
        EndDrawing();
    }
    resources.UnloadAll();
    CloseWindow();
    return 0;
}
