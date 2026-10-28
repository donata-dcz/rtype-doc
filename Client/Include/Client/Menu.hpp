/*
** EPITECH PROJECT, 2026
** RType
** File description:
** Menu.hpp
*/

#pragma once

#include <raylib.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

#include "MenuRenderer.hpp"

namespace Client {

    constexpr std::array<float, 3> Scales{{1.0F, 1.25F, 1.50F}};

    struct Settings {
        int masterVolumePercent{80};
        float uiScale{1.25F};
    };

    enum class MenuAction {
        None,
        Connect,
        Quit,
    };

    struct ConnectRequest {
        std::string host;
        std::uint16_t port{4242};
        std::string playerName{"Player"};
    };

    struct MenuResult {
        MenuAction action{MenuAction::None};
        ConnectRequest request{};
    };

    class Menu {
    public:
        explicit Menu(Settings &settings) : settings(settings) {}
        [[nodiscard]] MenuResult Update(int screenWidth, int screenHeight);
        void Draw(int screenWidth, int screenHeight) const;
        void Reset();
        void SetLogo(const ::Texture *logo);
        void SetConnectionStatus(std::string status);
        void SetConnectionDefaults(std::string host, std::uint16_t port);

    private:
        enum class Screen { Main, Options };
        struct Input {
            bool enter{false};
            bool clicked{false};
            bool activate{false};
            bool back{false};
            int step{0};
        };

        //^ What each screen shows, indexed by Screen (Main, Options).
        struct ScreenInfo {
            int rows;
            const char *title;
            const char *help;
        };
        static constexpr std::array<ScreenInfo, 2> Screens{{
            {3, "R-TYPE", "Up/Down: choose     Enter: select"},
            {3, "OPTIONS",
                "Up/Down: choose     Left/Right: change     Esc: back"}}};
        static int NearestScaleIndex(float scale);
        static Input ReadInput(bool clicked);
        const ScreenInfo &Info() const
        {
            return Screens[static_cast<std::size_t>(screen)];
        }

        void showSelectedScreen(Screen screen, int selected);
        bool UpdateSelection(const MenuRenderer::Layout &layout, int count);
        void Dispatch(const Input &input, MenuResult &result);
        void HandleMain(const Input &input, MenuResult &result);
        void HandleOptions(const Input &input);
        void AdjustVolume(const Input &input);
        void AdjustScale(const Input &input);
        void DrawMain(const MenuRenderer::Layout &layout) const;
        void DrawStatus(const MenuRenderer::Layout &layout, int screenWidth) const;
        void DrawOptions(const MenuRenderer::Layout &layout) const;
        void DrawTitle(const MenuRenderer::Layout &layout, int screenWidth, int screenHeight) const;
        Settings &settings;
        const ::Texture *logo{nullptr};
        Screen screen{Screen::Main};
        int selected{0};
        ConnectRequest request;
        std::string connectionStatus;
        MenuRenderer renderer;
    };

} //* namespace Client
