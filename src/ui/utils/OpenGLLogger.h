#pragma once
#include <afxwin.h>
#include <time.h>
#include <fstream>

/**
 * @class COpenGLLogger
 * @brief OpenGL日志类
 * @details 负责记录应用程序的日志信息到文件
 * @功能描述 提供日志记录功能，支持不同参数类型的日志输出
 * @设计目的 为应用程序提供统一的日志记录机制
 * @使用场景 用于记录应用程序运行状态、错误信息和调试信息
 * @关键实现 使用单例模式，支持不同参数类型的重载Log方法
 */
class COpenGLLogger
{
public:
  static COpenGLLogger& Instance()
  {
    static COpenGLLogger instance;
    return instance;
  }

  void Log(const CString& strMessage)
  {
    CString strLogPath;
    strLogPath = _T("opengl_log.txt");

    CTime tm = CTime::GetCurrentTime();
    CString strTime = tm.Format(_T("%Y-%m-%d %H:%M:%S"));

    CString strFullMessage;
    strFullMessage.Format(_T("[%s] %s\n"), strTime, strMessage);

    std::ofstream file;
    file.open(CStringA(strLogPath), std::ios::app);
    if (file.is_open())
    {
      file << CStringA(strFullMessage);
      file.close();
    }
  }

  void Log(const CString& strFormat, int nParam1)
  {
    CString strMessage;
    strMessage.Format(strFormat, nParam1);
    Log(strMessage);
  }

  void Log(const CString& strFormat, int nParam1, int nParam2)
  {
    CString strMessage;
    strMessage.Format(strFormat, nParam1, nParam2);
    Log(strMessage);
  }

  void Log(const CString& strFormat, float fParam1, float fParam2, float fParam3)
  {
    CString strMessage;
    strMessage.Format(strFormat, fParam1, fParam2, fParam3);
    Log(strMessage);
  }

private:
  COpenGLLogger() {}
  ~COpenGLLogger() {}
  COpenGLLogger(const COpenGLLogger&) = delete;
  COpenGLLogger& operator=(const COpenGLLogger&) = delete;
};

#define LOG(msg) COpenGLLogger::Instance().Log(msg)
