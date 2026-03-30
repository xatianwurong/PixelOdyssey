#include "Shader.h"
#include <GL/GLEW.h>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& filePath)
  : m_FilePath(filePath)
{
  // 尝试从文件中读取着色器代码
  std::string shaderSource = ParseShader(filePath);

  // 检查文件是否成功读取
  if (shaderSource.empty())
  {
    std::cout << "Warning: Shader file is empty or could not be read, using default shader" << std::endl;
    return;
  }
  else
  {
    // 解析着色器文件
    size_t vertexShaderEnd = shaderSource.find("#shader fragment");
    if (vertexShaderEnd == std::string::npos)
    {
      std::cout << "Error: Invalid shader file format" << std::endl;
      m_RendererID = 0;
      return;
    }

    std::string vertexShader = shaderSource.substr(shaderSource.find("#shader vertex") + 14, vertexShaderEnd - (shaderSource.find("#shader vertex") + 14));
    std::string fragmentShader = shaderSource.substr(vertexShaderEnd + 16);

    m_RendererID = CreateShader(vertexShader, fragmentShader);
  }
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
  : m_FilePath(vertexPath + ";" + fragmentPath)
{
  std::string vertexShader = ParseShader(vertexPath);
  std::string fragmentShader = ParseShader(fragmentPath);
  m_RendererID = CreateShader(vertexShader, fragmentShader);
}

Shader::~Shader()
{
  glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
  glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
  glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
  glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
  glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
  glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
  glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMatrix4fv(const std::string& name, int count, bool transpose, const float* value)
{
  glUniformMatrix4fv(GetUniformLocation(name), count, transpose, value);
}

void Shader::SetUniformMat4f(const std::string& name, const float* value)
{
  SetUniformMatrix4fv(name, 1, false, value);
}

int Shader::GetUniformLocation(const std::string& name)
{
  int location = glGetUniformLocation(m_RendererID, name.c_str());
  if (location == -1)
  {
    std::cout << "Warning: uniform " << name << " not found" << std::endl;
  }
  return location;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
  std::cout << "Creating shader program..." << std::endl;
  unsigned int program = glCreateProgram();
  std::cout << "Shader program created with ID: " << program << std::endl;

  std::cout << "Compiling vertex shader..." << std::endl;
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  std::cout << "Vertex shader compiled with ID: " << vs << std::endl;

  std::cout << "Compiling fragment shader..." << std::endl;
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
  std::cout << "Fragment shader compiled with ID: " << fs << std::endl;

  if (vs == 0 || fs == 0)
  {
    std::cout << "Failed to compile shaders!" << std::endl;
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(program);
    return 0;
  }

  std::cout << "Attaching shaders to program..." << std::endl;
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  std::cout << "Linking shader program..." << std::endl;
  glLinkProgram(program);

  // Check link status
  int result;
  glGetProgramiv(program, GL_LINK_STATUS, &result);
  if (result == GL_FALSE)
  {
    int length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    char* message = new char[length * sizeof(char)];
    glGetProgramInfoLog(program, length, &length, message);
    std::cout << "Failed to link shader program!" << std::endl;
    std::cout << message << std::endl;
    delete[] message;
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(program);
    return 0;
  }
  else
  {
    std::cout << "Shader program linked successfully!" << std::endl;
    // Check if u_Model uniform exists
    int modelLocation = glGetUniformLocation(program, "u_Model");
    std::cout << "u_Model uniform location: " << modelLocation << std::endl;
    // Check if u_Color uniform exists
    int colorLocation = glGetUniformLocation(program, "u_Color");
    std::cout << "u_Color uniform location: " << colorLocation << std::endl;
  }

  std::cout << "Deleting shader objects..." << std::endl;
  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

unsigned int Shader::CreateComputeShader(const std::string& computeShader)
{
  unsigned int program = glCreateProgram();
  unsigned int cs = CompileShader(GL_COMPUTE_SHADER, computeShader);

  if (cs == 0)
  {
    std::cout << "Failed to compile compute shader!" << std::endl;
    glDeleteShader(cs);
    glDeleteProgram(program);
    return 0;
  }

  glAttachShader(program, cs);
  glLinkProgram(program);

  // Check link status
  int result;
  glGetProgramiv(program, GL_LINK_STATUS, &result);
  if (result == GL_FALSE)
  {
    int length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    char* message = new char[length * sizeof(char)];
    glGetProgramInfoLog(program, length, &length, message);
    std::cout << "Failed to link compute shader program!" << std::endl;
    std::cout << message << std::endl;
    delete[] message;
    glDeleteShader(cs);
    glDeleteProgram(program);
    return 0;
  }

  glDeleteShader(cs);

  return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  // Check compile status
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = new char[length * sizeof(char)];
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : type == GL_FRAGMENT_SHADER ? "fragment" : "compute") << " shader!" << std::endl;
    std::cout << message << std::endl;
    delete[] message;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

std::string Shader::ParseShader(const std::string& filePath)
{
  std::cout << "Parsing shader file: " << filePath << std::endl;

  // 检查文件是否存在
  std::ifstream stream(filePath);
  if (!stream.is_open())
  {
    std::cout << "Error: Could not open shader file: " << filePath << std::endl;
    return "";
  }
  else
  {
    std::cout << "Successfully opened shader file!" << std::endl;
  }

  // 读取文件内容
  std::stringstream buffer;
  buffer << stream.rdbuf();
  std::string shaderSource = buffer.str();

  // 移除UTF-8 BOM标记
  if (shaderSource.size() >= 3)
  {
    unsigned char bom1 = static_cast<unsigned char>(shaderSource[0]);
    unsigned char bom2 = static_cast<unsigned char>(shaderSource[1]);
    unsigned char bom3 = static_cast<unsigned char>(shaderSource[2]);
    std::cout << "BOM bytes: " << static_cast<int>(bom1) << ", " << static_cast<int>(bom2) << ", " << static_cast<int>(bom3) << std::endl;
    if (bom1 == 0xEF && bom2 == 0xBB && bom3 == 0xBF)
    {
      std::cout << "Removed UTF-8 BOM from shader file" << std::endl;
      shaderSource = shaderSource.substr(3);
    }
  }

  // 打印前几个字符，确保BOM已移除
  std::cout << "First 10 characters of shader source: ";
  for (int i = 0; i < 10 && i < shaderSource.size(); i++)
  {
    std::cout << static_cast<unsigned char>(shaderSource[i]) << " (" << static_cast<int>(static_cast<unsigned char>(shaderSource[i])) << "), ";
  }
  std::cout << std::endl;

  if (shaderSource.empty())
  {
    std::cout << "Warning: Shader file is empty: " << filePath << std::endl;
  }
  else
  {
    std::cout << "Shader file loaded successfully. Length: " << shaderSource.length() << " characters" << std::endl;
  }

  return shaderSource;
}
