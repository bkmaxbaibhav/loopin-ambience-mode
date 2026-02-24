#include "Shader.h"

Shader::Shader()  {}
Shader::~Shader() {}

bool Shader::load(const std::string& vertPath,
                  const std::string& fragPath) {
    // TODO Session 4: read files, compile shaders, link program
    return false;
}

void Shader::use() {
    // TODO Session 4: glUseProgram(programID_)
}

void Shader::setFloat(const std::string& name, float value) {
    // TODO Session 4: glUniform1f
}

void Shader::setInt(const std::string& name, int value) {
    // TODO Session 4: glUniform1i
}

void Shader::setVec3(const std::string& name,
                     float x, float y, float z) {
    // TODO Session 4: glUniform3f
}
