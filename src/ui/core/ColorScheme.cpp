#include "ColorScheme.h"

void ColorScheme::ApplyTheme(Theme theme) {
  m_currentTheme = theme;
  
  switch (theme) {
    case Theme::Dark:
      SetupDarkTheme();
      break;
    case Theme::Light:
      SetupLightTheme();
      break;
    case Theme::HighContrast:
      SetupHighContrastTheme();
      break;
    case Theme::Custom:
      // 自定义主题，不设置默认值
      break;
  }
}

void ColorScheme::SetupDarkTheme() {
  // 深色主题 - 类似 Visual Studio / AutoCAD 深色模式
  m_colors[ColorRole::Background] = RGB(30, 30, 30);       // #1E1E1E
  m_colors[ColorRole::Surface] = RGB(37, 37, 37);          // #252525
  m_colors[ColorRole::SurfaceHover] = RGB(45, 45, 48);     // #2D2D30
  m_colors[ColorRole::Primary] = RGB(0, 120, 212);         // #0078D4
  m_colors[ColorRole::PrimaryHover] = RGB(30, 144, 255);   // #1E90FF
  m_colors[ColorRole::Secondary] = RGB(0, 200, 83);        // #00C853
  m_colors[ColorRole::TextPrimary] = RGB(240, 240, 240);   // #F0F0F0
  m_colors[ColorRole::TextSecondary] = RGB(160, 160, 160); // #A0A0A0
  m_colors[ColorRole::TextDisabled] = RGB(100, 100, 100);  // #646464
  m_colors[ColorRole::Border] = RGB(62, 62, 66);           // #3E3E42
  m_colors[ColorRole::BorderFocus] = RGB(0, 120, 212);     // #0078D4
  m_colors[ColorRole::Success] = RGB(0, 200, 83);          // #00C853
  m_colors[ColorRole::Warning] = RGB(255, 179, 0);         // #FFB300
  m_colors[ColorRole::Error] = RGB(244, 67, 54);           // #F44336
  m_colors[ColorRole::Selection] = RGB(0, 120, 212);   // 半透明蓝色
  m_colors[ColorRole::Highlight] = RGB(255, 165, 0);       // #FFA500
}

void ColorScheme::SetupLightTheme() {
  // 浅色主题 - 类似 Visual Studio / AutoCAD 浅色模式
  m_colors[ColorRole::Background] = RGB(245, 245, 245);    // #F5F5F5
  m_colors[ColorRole::Surface] = RGB(255, 255, 255);       // #FFFFFF
  m_colors[ColorRole::SurfaceHover] = RGB(235, 235, 235);  // #EBEBEB
  m_colors[ColorRole::Primary] = RGB(0, 120, 212);         // #0078D4
  m_colors[ColorRole::PrimaryHover] = RGB(0, 90, 160);     // #005AA0
  m_colors[ColorRole::Secondary] = RGB(0, 150, 60);        // #00963C
  m_colors[ColorRole::TextPrimary] = RGB(30, 30, 30);      // #1E1E1E
  m_colors[ColorRole::TextSecondary] = RGB(90, 90, 90);    // #5A5A5A
  m_colors[ColorRole::TextDisabled] = RGB(150, 150, 150);  // #969696
  m_colors[ColorRole::Border] = RGB(200, 200, 200);        // #C8C8C8
  m_colors[ColorRole::BorderFocus] = RGB(0, 120, 212);     // #0078D4
  m_colors[ColorRole::Success] = RGB(0, 150, 60);          // #00963C
  m_colors[ColorRole::Warning] = RGB(200, 140, 0);         // #C88C00
  m_colors[ColorRole::Error] = RGB(200, 50, 50);           // #C83232
  m_colors[ColorRole::Selection] = RGB(0, 120, 212);   // 半透明蓝色
  m_colors[ColorRole::Highlight] = RGB(255, 140, 0);       // #FF8C00
}

void ColorScheme::SetupHighContrastTheme() {
  // 高对比度主题 - 提高可访问性
  m_colors[ColorRole::Background] = RGB(0, 0, 0);          // #000000
  m_colors[ColorRole::Surface] = RGB(20, 20, 20);          // #141414
  m_colors[ColorRole::SurfaceHover] = RGB(40, 40, 40);     // #282828
  m_colors[ColorRole::Primary] = RGB(0, 255, 255);         // #00FFFF (青色)
  m_colors[ColorRole::PrimaryHover] = RGB(100, 255, 255);  // #64FFFF
  m_colors[ColorRole::Secondary] = RGB(0, 255, 0);         // #00FF00 (绿色)
  m_colors[ColorRole::TextPrimary] = RGB(255, 255, 255);   // #FFFFFF
  m_colors[ColorRole::TextSecondary] = RGB(200, 200, 200); // #C8C8C8
  m_colors[ColorRole::TextDisabled] = RGB(120, 120, 120);  // #787878
  m_colors[ColorRole::Border] = RGB(255, 255, 255);        // #FFFFFF
  m_colors[ColorRole::BorderFocus] = RGB(0, 255, 255);     // #00FFFF
  m_colors[ColorRole::Success] = RGB(0, 255, 0);           // #00FF00
  m_colors[ColorRole::Warning] = RGB(255, 255, 0);         // #FFFF00
  m_colors[ColorRole::Error] = RGB(255, 0, 0);             // #FF0000
  m_colors[ColorRole::Selection] = RGB(0, 255, 255);   // 半透明青色
  m_colors[ColorRole::Highlight] = RGB(255, 0, 255);       // #FF00FF
}
