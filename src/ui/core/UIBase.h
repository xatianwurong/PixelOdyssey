#include <afxwin.h> // Include necessary header for CWnd

class UIBase : public CWnd {
public:
    void SetVisible(bool visible);
    void SetEnabled(bool enabled);
    void SetHoverState(bool hover);
    void SetFocusState(bool focus);
    void CreateFont(const CString& fontName, int fontSize, int fontWeight);
    void Draw();
    static ThemeManager& GetThemeManager(); // Singleton
};
