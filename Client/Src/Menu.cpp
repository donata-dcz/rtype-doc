/*
** EPITECH PROJECT, 2026
** RType
** File description:
** Menu.cpp
*/

#include <Client/Menu.hpp>
#include <Client/MenuRenderer.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <utility>

int Client::Menu::NearestScaleIndex(float scale)
{
    int best = 0;

    for (int index = 1; index < static_cast<int>(Scales.size()); ++index) {
        const auto current = static_cast<std::size_t>(index);
        const auto selected = static_cast<std::size_t>(best);
        if (std::fabs(Scales[current] - scale) < std::fabs(Scales[selected] - scale))
            best = index;
    }
    return best;
}

Client::Menu::Input Client::Menu::ReadInput(bool clicked)
{
    Input input;

    input.enter = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) ||
        IsKeyPressed(KEY_SPACE);
    input.clicked = clicked;
    input.activate = input.enter || input.clicked;
    input.back = IsKeyPressed(KEY_ESCAPE);
    input.step = (MenuRenderer::KeyOrRepeat(KEY_RIGHT) ? 1 : 0) -
        (MenuRenderer::KeyOrRepeat(KEY_LEFT) ? 1 : 0);
    return input;
}

void Client::Menu::showSelectedScreen(Screen screen, int selected)
{
    this->screen = screen;
    this->selected = selected;
}

void Client::Menu::SetLogo(const ::Texture *logo)
{
    this->logo = logo;
    if (this->logo != nullptr && this->logo->id != 0)
        SetTextureFilter(*this->logo, TEXTURE_FILTER_BILINEAR);
}

void Client::Menu::SetConnectionStatus(std::string status)
{
    connectionStatus = std::move(status);
}

void Client::Menu::SetConnectionDefaults(std::string host, std::uint16_t port)
{
    request.host = std::move(host);
    request.port = port;
}

void Client::Menu::Reset()
{
    showSelectedScreen(Screen::Main, 0);
    connectionStatus.clear();
}

bool Client::Menu::UpdateSelection(const MenuRenderer::Layout &layout,
    int count)
{
    const bool shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    const bool tab = IsKeyPressed(KEY_TAB);
    const Vector2 delta = GetMouseDelta();
    const int hovered = renderer.RowUnderMouse(layout, count);
    const bool clicked = hovered >= 0 &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (MenuRenderer::KeyOrRepeat(KEY_DOWN) || (tab && !shift))
        selected = (selected + 1) % count;
    if (MenuRenderer::KeyOrRepeat(KEY_UP) || (tab && shift))
        selected = (selected + count - 1) % count;
    if (hovered >= 0 && (clicked || delta.x != 0.0F || delta.y != 0.0F))
        selected = hovered;
    return clicked;
}

Client::MenuResult Client::Menu::Update(int screenWidth, int screenHeight)
{
    MenuResult result;
    const MenuRenderer::Layout layout =
        renderer.MakeLayout(screenWidth, screenHeight, settings.uiScale);
    const int count = Info().rows;

    selected = std::clamp(selected, 0, count - 1);
    Dispatch(ReadInput(UpdateSelection(layout, count)), result);
    return result;
}

void Client::Menu::Dispatch(const Input &input, MenuResult &result)
{
    switch (screen) {
        case Screen::Main:
            HandleMain(input, result);
            break;
        case Screen::Options:
            HandleOptions(input);
            break;
    }
}

void Client::Menu::HandleMain(const Input &input, MenuResult &result)
{
    if (!input.activate)
        return;
    switch (selected) {
        case 0:
            result.action = MenuAction::Connect;
            result.request = request;
            break;
        case 1:
            showSelectedScreen(Screen::Options, 0);
            break;
        default:
            result.action = MenuAction::Quit;
            break;
    }
}

void Client::Menu::AdjustVolume(const Input &input)
{
    int &volume = settings.masterVolumePercent;

    if (input.step != 0)
        volume = std::clamp(volume + input.step * 10, 0, 100);
    else if (input.activate)
        volume = volume >= 100 ? 0 : volume + 10;
}

void Client::Menu::AdjustScale(const Input &input)
{
    if (!input.activate && input.step == 0)
        return;
    const int count = static_cast<int>(Scales.size());
    const int direction = input.step != 0 ? input.step : 1;
    const int current = NearestScaleIndex(settings.uiScale);
    const int next = (current + direction + count) % count;

    settings.uiScale = Scales[static_cast<std::size_t>(next)];
}

void Client::Menu::HandleOptions(const Input &input)
{
    if (input.back) {
        showSelectedScreen(Screen::Main, 1);
        return;
    }
    switch (selected) {
        case 0:
            AdjustVolume(input);
            break;
        case 1:
            AdjustScale(input);
            break;
        case 2:
            if (input.activate)
                showSelectedScreen(Screen::Main, 1);
            break;
        default:
            break;
    }
}

void Client::Menu::DrawMain(const MenuRenderer::Layout &layout) const
{
    renderer.DrawRow(layout, 0, "Play", selected == 0);
    renderer.DrawRow(layout, 1, "Options", selected == 1);
    renderer.DrawRow(layout, 2, "Quit", selected == 2);
}

void Client::Menu::DrawStatus(const MenuRenderer::Layout &layout,
    int screenWidth) const
{
    const int size = layout.fontSize * 4 / 5;

    if (connectionStatus.empty())
        return;
    renderer.DrawCentered(connectionStatus, screenWidth,
        layout.top + (Info().rows + 1) * layout.rowHeight, size,
        MenuRenderer::TextColor);
}

void Client::Menu::DrawOptions(const MenuRenderer::Layout &layout) const
{
    const std::string volume =
        std::to_string(settings.masterVolumePercent) + "%";
    const std::string scale = std::to_string(
        static_cast<int>(settings.uiScale * 100.0F + 0.5F)) + "%";

    renderer.DrawRow(layout, 0, "Volume", selected == 0, volume);
    renderer.DrawRow(layout, 1, "Text size", selected == 1, scale);
    renderer.DrawRow(layout, 2, "Back", selected == 2);
}

void Client::Menu::DrawTitle(const MenuRenderer::Layout &layout,
    int screenWidth, int screenHeight) const
{
    if (screen == Screen::Main && logo != nullptr && logo->id != 0)
        renderer.DrawLogo(*logo, screenWidth, screenHeight, layout);
    else
        renderer.DrawCentered(Info().title, screenWidth,
            screenHeight * 6 / 100, layout.fontSize * 2,
            MenuRenderer::TextColor);
}

void Client::Menu::Draw(int screenWidth, int screenHeight) const
{
    const MenuRenderer::Layout layout =
        renderer.MakeLayout(screenWidth, screenHeight, settings.uiScale);
    const int helpSize = std::max(12, layout.fontSize * 3 / 5);

    switch (screen) {
        case Screen::Main:
            DrawMain(layout);
            DrawStatus(layout, screenWidth);
            break;
        case Screen::Options:
            DrawOptions(layout);
            break;
    }
    DrawTitle(layout, screenWidth, screenHeight);
    renderer.DrawCentered(Info().help, screenWidth,
        screenHeight - layout.fontSize * 2, helpSize, MenuRenderer::DimColor);
}
