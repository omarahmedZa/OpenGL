#pragma once

#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include "Renderer.h"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FregmentSource;
};

class Shader
{
private:
    std::string m_FilePath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
public:
    Shader(const std::string& filePath);
    ~Shader();

    void Bind() const;
    void UnBind() const;

    // Set uniforms
    void SetUniform1f(const std::string& name, float value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
    ShaderProgramSource ParseShader(const string& filePath);
    unsigned int CompileShader(unsigned int type, const string& source);
    unsigned int CreateShader(const string& vertexShader, const string& fragmentShader);
    int GetUniformLocation(const std::string& name);
};
