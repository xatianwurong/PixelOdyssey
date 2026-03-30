#pragma once
#include <afxwin.h>
#include <time.h>
#include <fstream>

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
    strLogPath = _T("d:\\TestOPenGL\\opengl_log.txt");

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
