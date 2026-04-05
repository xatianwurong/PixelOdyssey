#pragma once

#include <afxwin.h>
#include <map>
#include <string>
#include "UILayout.h"

// UI 公共工具集 - 字体/通用绘制辅助
namespace UICommon {

inline CFont* GetFont(int size, int weight = UILayout::UIFonts::BODY_WEIGHT, const TCHAR* face = UILayout::UIFonts::FONT_FAMILY)
{
    // key: size(16bit) | weight(8bit) | face-hash(remaining)
    using tstring = std::basic_string<TCHAR>;
    std::hash<tstring> hasher;
    tstring faceStr(face);
    unsigned long long faceHash = hasher(faceStr);
    unsigned long long key = ((unsigned long long)size << 48) ^ ((unsigned long long)weight << 40) ^ (faceHash & 0xFFFFFFFFFFULL);

    static std::map<unsigned long long, CFont*> s_fonts;
    auto it = s_fonts.find(key);
    if (it != s_fonts.end())
        return it->second;

    LOGFONT lf = {};
    lf.lfHeight = -size;
    lf.lfWeight = weight;
    lf.lfQuality = CLEARTYPE_QUALITY;
    _tcscpy_s(lf.lfFaceName, face);

    CFont* pFont = new CFont();
    pFont->CreateFontIndirect(&lf);
    s_fonts.insert(std::make_pair(key, pFont));
    return pFont;
}

inline CFont* GetBodyFont()
{
    return GetFont(UILayout::UIFonts::BODY_SIZE, UILayout::UIFonts::BODY_WEIGHT, UILayout::UIFonts::FONT_FAMILY);
}

inline CFont* GetTitleFont()
{
    return GetFont(UILayout::UIFonts::TITLE_SIZE, UILayout::UIFonts::TITLE_WEIGHT, UILayout::UIFonts::FONT_FAMILY);
}

inline CFont* GetMonospaceFont(int size = UILayout::UIFonts::MONOSPACE_SIZE)
{
    return GetFont(size, UILayout::UIFonts::BODY_WEIGHT, UILayout::UIFonts::MONOSPACE_FONT);
}

inline CFont* GetSymbolFont(int size = UILayout::UIFonts::TOOL_ICON_SIZE)
{
    // 优先使用 Segoe MDL2（Windows 11 内置），回退到 Wingdings
    return GetFont(size, UILayout::UIFonts::BODY_WEIGHT, UILayout::UIFonts::SYMBOL_FONT);
}

inline CFont* GetChineeseFont(int size = UILayout::UIFonts::BODY_SIZE)
{
    // 用于中文文本的后备字体
    return GetFont(size, UILayout::UIFonts::BODY_WEIGHT, UILayout::UIFonts::FALLBACK_FONT);
}

} // namespace UICommon
