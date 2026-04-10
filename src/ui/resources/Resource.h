#pragma once

// 对话框资源 ID
#define IDD_MAINDIALOG                    101

// 控件资源 ID (已弃用，保留向后兼容)
#define IDC_GROUP_RENDER                  1001
#define IDC_BUTTON_START                  1002
#define IDC_BUTTON_STOP                   1003
#define IDC_BUTTON_RESET                  1004
#define IDC_CHECK_WIREFRAME               1005
#define IDC_CHECK_SMOOTH                  1006
#define IDC_CHECK_DEPTH                   1007
#define IDC_GROUP_CAMERA                  1008
#define IDC_EDIT_CAMERA_X                 1009
#define IDC_EDIT_CAMERA_Y                 1010
#define IDC_EDIT_CAMERA_Z                 1011
#define IDC_BUTTON_APPLY                  1012
#define IDC_STATIC_OPENGL                 1013
#define IDC_GROUP_INFO                    1014
#define IDC_EDIT_STATUS                   1015
#define IDC_STATIC_TITLE                  1016
#define IDC_STATIC_VERSION                1017

// MDI 多文档界面资源 ID
#define IDR_MAINFRAME                     200    // 主框架窗口资源 (菜单 + 工具栏)
#define IDR_PIXELTYPE                     201    // 文档类型资源 (菜单 + 工具栏)

// 文件/编辑等标准命令 ID 使用 MFC 在 afxres.h 中提供的定义
// 自定义资源头里不要重复定义，否则 rc.exe 会报 redefinition 警告。

// 应用退出命令复用 MFC 标准退出 ID，便于直接映射到 CWinAppEx::OnAppExit
#define ID_FILE_EXIT                      ID_APP_EXIT

// 菜单命令 ID - 视图操作
#define ID_VIEW_STATUSBAR                 32821
#define ID_VIEW_WINDOWS                   32822

// 菜单命令 ID - 帮助
#define ID_HELP_ABOUT                     32840

