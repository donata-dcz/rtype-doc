/*
** EPITECH PROJECT, 2026
** RType
** File description:
** MenuRenderer.cpp
*/

#include <Client/MenuRenderer.hpp>
#include <algorithm>

Client::MenuRenderer::Layout Client::MenuRenderer::MakeLayout(
    int screenWidth, int screenHeight, float scale) const
{
    Layout layout{};

    layout.fontSize = std::max(14, static_cast<int>(static_cast<float>(screenHeight) / 24.0F * scale));
    layout.rowHeight = layout.fontSize * 9 / 5;
    layout.rowWidth = std::min(static_cast<int>(static_cast<float>(screenWidth) * 0.9F), layout.fontSize * 22);
    layout.left = (screenWidth - layout.rowWidth) / 2;
    layout.top = screenHeight * 22 / 100;
    layout.padding = layout.fontSize / 2;
    return layout;
}

Rectangle Client::MenuRenderer::RowRect(const Layout &layout, int index)
{
    return Rectangle{
        static_cast<float>(layout.left),
        static_cast<float>(layout.top + index * layout.rowHeight),
        static_cast<float>(layout.rowWidth),
        static_cast<float>(layout.rowHeight - 6)
    };
}

int Client::MenuRenderer::RowUnderMouse(const Layout &layout, int count) const
{
    const Vector2 mouse = GetMousePosition();

    for (int index = 0; index < count; ++index) {
        if (CheckCollisionPointRec(mouse, RowRect(layout, index)))
            return index;
    }
    return -1;
}

bool Client::MenuRenderer::KeyOrRepeat(int key)
{
    return IsKeyPressed(key) || IsKeyPressedRepeat(key);
}

void Client::MenuRenderer::DrawCentered(const std::string &text, int screenWidth,
    int y, int size, Color color) const
{
    DrawText(text.c_str(), (screenWidth - MeasureText(text.c_str(), size)) / 2, y, size, color);
}

void Client::MenuRenderer::DrawRow(const Layout &layout, int index,
    const std::string &label, bool selected, const std::string &value) const
{
    const Rectangle rect = RowRect(layout, index);
    const int x = static_cast<int>(rect.x) + layout.padding;
    const int textY = static_cast<int>(rect.y) +
        (static_cast<int>(rect.height) - layout.fontSize) / 2;

    if (selected) {
        DrawRectangleRec(rect, HighlightColor);
        DrawRectangleLinesEx(rect, 2.0F, AccentColor);
        DrawText(">", x, textY, layout.fontSize, AccentColor);
    }
    DrawText(label.c_str(), x + layout.fontSize, textY, layout.fontSize,
        selected ? TextColor : DimColor);
    if (value.empty())
        return;
    const std::string shown = "< " + value + " >";
    DrawText(shown.c_str(), static_cast<int>(rect.x + rect.width) - layout.padding -
        MeasureText(shown.c_str(), layout.fontSize), textY, layout.fontSize, AccentColor);
}

void Client::MenuRenderer::DrawLogo(const Texture2D &logo, int screenWidth,
    int screenHeight, const Layout &layout) const
{
    const float top = static_cast<float>(screenHeight) * 0.03F;
    const float maxHeight = static_cast<float>(layout.top) - top - 8.0F;
    const float maxWidth = static_cast<float>(screenWidth) * 0.8F;
    const float scale = std::min(maxWidth / static_cast<float>(logo.width),
        maxHeight / static_cast<float>(logo.height));
    if (scale <= 0.0F)
        return;
    const float width = static_cast<float>(logo.width) * scale;
    const float height = static_cast<float>(logo.height) * scale;
    const Rectangle destination{(static_cast<float>(screenWidth) - width) * 0.5F, top, width, height};
    DrawTexturePro(logo, Rectangle{0.0F, 0.0F, static_cast<float>(logo.width), static_cast<float>(logo.height)},
        destination, Vector2{0.0F, 0.0F}, 0.0F, WHITE);
}
