#pragma once
#include <string>

// Loads, compiles and manages a GLSL shader program
class Shader {
public:
    Shader();
    ~Shader();

    bool load(const std::string& vertPath,
              const std::string& fragPath);
    void use();
    void setFloat(const std::string& name, float value);
    void setInt  (const std::string& name, int value);
    void setVec2 (const std::string& name, float x, float y);
    void setVec3 (const std::string& name,
                  float x, float y, float z);

private:
    unsigned int programID_ = 0;
};
