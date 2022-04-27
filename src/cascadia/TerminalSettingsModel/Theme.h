/*++
Copyright (c) Microsoft Corporation
Licensed under the MIT license.

Module Name:
- Theme.hpp

Abstract:
- A Theme represents a collection of settings which control the appearance of
  the Terminal window itself. Things like the color of the titlebar, the style
  of the tabs.

Author(s):
- Mike Griese - March 2022

--*/
#pragma once

#include "../../inc/conattrs.hpp"
#include "DefaultSettings.h"
#include "IInheritable.h"
#include "MTSMSettings.h"

#include "ThemeColor.g.h"
#include "WindowTheme.g.h"
#include "TabRowTheme.g.h"
#include "Theme.g.h"

namespace winrt::Microsoft::Terminal::Settings::Model::implementation
{
    struct ThemeColor : ThemeColorT<ThemeColor>
    {
    public:
        ThemeColor() noexcept = default;
        static winrt::Microsoft::Terminal::Settings::Model::ThemeColor FromColor(const winrt::Microsoft::Terminal::Core::Color& coreColor) noexcept;
        static winrt::Microsoft::Terminal::Settings::Model::ThemeColor FromAccent() noexcept;
        static winrt::Microsoft::Terminal::Settings::Model::ThemeColor FromTerminalBackground() noexcept;

        WINRT_PROPERTY(til::color, Color);
        WINRT_PROPERTY(winrt::Microsoft::Terminal::Settings::Model::ThemeColorType, ColorType);
    };

#define THEME_SETTINGS_INITIALIZE(type, name, jsonKey, ...) \
    WINRT_PROPERTY(type, name, ##__VA_ARGS__)

#define THEME_SETTINGS_COPY(type, name, jsonKey, ...) \
    result->_##name = _##name;

#define COPY_THEME_OBJECT(T, macro)           \
    winrt::com_ptr<T> Copy()                  \
    {                                         \
        auto result{ winrt::make_self<T>() }; \
        macro(THEME_SETTINGS_COPY);           \
        return result;                        \
    }

    struct WindowTheme : WindowThemeT<WindowTheme>
    {
        MTSM_THEME_WINDOW_SETTINGS(THEME_SETTINGS_INITIALIZE);

    public:
        COPY_THEME_OBJECT(WindowTheme, MTSM_THEME_WINDOW_SETTINGS);
    };

    struct TabRowTheme : TabRowThemeT<TabRowTheme>
    {
        MTSM_THEME_TABROW_SETTINGS(THEME_SETTINGS_INITIALIZE);

    public:
        COPY_THEME_OBJECT(TabRowTheme, MTSM_THEME_TABROW_SETTINGS);
    };

    struct Theme : ThemeT<Theme>
    {
    public:
        Theme() noexcept;
        Theme(const winrt::Windows::UI::Xaml::ElementTheme& requestedTheme) noexcept;

        com_ptr<Theme> Copy() const;

        hstring ToString()
        {
            return Name();
        }

        static com_ptr<Theme> FromJson(const Json::Value& json);
        void LayerJson(const Json::Value& json);
        Json::Value ToJson() const;

        WINRT_PROPERTY(winrt::hstring, Name);

        MTSM_THEME_SETTINGS(THEME_SETTINGS_INITIALIZE)

    private:
    };

#undef THEME_SETTINGS_INITIALIZE
#undef THEME_SETTINGS_COPY
}

namespace winrt::Microsoft::Terminal::Settings::Model::factory_implementation
{
    BASIC_FACTORY(ThemeColor);
    BASIC_FACTORY(Theme);
}