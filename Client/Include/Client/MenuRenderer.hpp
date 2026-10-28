/*
** EPITECH PROJECT, 2026
** RType
** File description:
** MenuRenderer.hpp
*/

#ifndef CLIENT_MENURENDERER_HPP
    #define CLIENT_MENURENDERER_HPP
    #include <raylib.h>
    #include <string>

namespace Client {

    class MenuRenderer {
    public:
        struct Layout {
            int fontSize;
            int rowHeight;
            int rowWidth;
            int left;
            int top;
            int padding;
        };
        static constexpr Color TextColor{245, 245, 245, 255};
        static constexpr Color DimColor{165, 165, 180, 255};
        static constexpr Color AccentColor{86, 180, 233, 255};
        Layout MakeLayout(int screenWidth, int screenHeight, float scale) const;
        int RowUnderMouse(const Layout &layout, int count) const;
        static bool KeyOrRepeat(int key);
        void DrawCentered(const std::string &text, int screenWidth, int y,
            int size, Color color) const;
        void DrawRow(const Layout &layout, int index,
            const std::string &label, bool selected,
            const std::string &value = "") const;
        void DrawLogo(const Texture2D &logo, int screenWidth,
            int screenHeight, const Layout &layout) const;

    private:
        static constexpr Color HighlightColor{30, 45, 80, 255};
        static Rectangle RowRect(const Layout &layout, int index);
    };

}

#endif
