#include "glad.h"
#include <GLFW/glfw3.h>

void (APIENTRY *glGenVertexArrays)(GLsizei n, GLuint *arrays);
void (APIENTRY *glBindVertexArray)(GLuint array);
void (APIENTRY *glDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
void (APIENTRY *glGenBuffers)(GLsizei n, GLuint *buffers);
void (APIENTRY *glBindBuffer)(GLenum target, GLuint buffer);
void (APIENTRY *glBufferData)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
void (APIENTRY *glDeleteBuffers)(GLsizei n, const GLuint *buffers);
void (APIENTRY *glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
void (APIENTRY *glEnableVertexAttribArray)(GLuint index);
GLuint (APIENTRY *glCreateShader)(GLenum type);
void (APIENTRY *glShaderSource)(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
void (APIENTRY *glCompileShader)(GLuint shader);
void (APIENTRY *glGetShaderiv)(GLuint shader, GLenum pname, GLint *params);
void (APIENTRY *glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void (APIENTRY *glDeleteShader)(GLuint shader);
GLuint (APIENTRY *glCreateProgram)(void);
void (APIENTRY *glAttachShader)(GLuint program, GLuint shader);
void (APIENTRY *glLinkProgram)(GLuint program);
void (APIENTRY *glGetProgramiv)(GLuint program, GLenum pname, GLint *params);
void (APIENTRY *glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void (APIENTRY *glUseProgram)(GLuint program);
void (APIENTRY *glDeleteProgram)(GLuint program);
GLint (APIENTRY *glGetUniformLocation)(GLuint program, const GLchar *name);
void (APIENTRY *glUniform1f)(GLint location, GLfloat v0);
void (APIENTRY *glUniform1i)(GLint location, GLint v0);
void (APIENTRY *glUniform2f)(GLint location, GLfloat v0, GLfloat v1);
void (APIENTRY *glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);

int gladLoadGLLoader(GLADloadproc load) {
    glGenVertexArrays = (void(*)(GLsizei, GLuint*)) load("glGenVertexArrays");
    glBindVertexArray = (void(*)(GLuint)) load("glBindVertexArray");
    glDeleteVertexArrays = (void(*)(GLsizei, const GLuint*)) load("glDeleteVertexArrays");
    glGenBuffers = (void(*)(GLsizei, GLuint*)) load("glGenBuffers");
    glBindBuffer = (void(*)(GLenum, GLuint)) load("glBindBuffer");
    glBufferData = (void(*)(GLenum, GLsizeiptr, const void*, GLenum)) load("glBufferData");
    glDeleteBuffers = (void(*)(GLsizei, const GLuint*)) load("glDeleteBuffers");
    glVertexAttribPointer = (void(*)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*)) load("glVertexAttribPointer");
    glEnableVertexAttribArray = (void(*)(GLuint)) load("glEnableVertexAttribArray");
    glCreateShader = (GLuint(*)(GLenum)) load("glCreateShader");
    glShaderSource = (void(*)(GLuint, GLsizei, const GLchar *const*, const GLint *)) load("glShaderSource");
    glCompileShader = (void(*)(GLuint)) load("glCompileShader");
    glGetShaderiv = (void(*)(GLuint, GLenum, GLint *)) load("glGetShaderiv");
    glGetShaderInfoLog = (void(*)(GLuint, GLsizei, GLsizei *, GLchar *)) load("glGetShaderInfoLog");
    glDeleteShader = (void(*)(GLuint)) load("glDeleteShader");
    glCreateProgram = (GLuint(*)(void)) load("glCreateProgram");
    glAttachShader = (void(*)(GLuint, GLuint)) load("glAttachShader");
    glLinkProgram = (void(*)(GLuint)) load("glLinkProgram");
    glGetProgramiv = (void(*)(GLuint, GLenum, GLint *)) load("glGetProgramiv");
    glGetProgramInfoLog = (void(*)(GLuint, GLsizei, GLsizei *, GLchar *)) load("glGetProgramInfoLog");
    glUseProgram = (void(*)(GLuint)) load("glUseProgram");
    glDeleteProgram = (void(*)(GLuint)) load("glDeleteProgram");
    glGetUniformLocation = (GLint(*)(GLuint, const GLchar *)) load("glGetUniformLocation");
    glUniform1f = (void(*)(GLint, GLfloat)) load("glUniform1f");
    glUniform1i = (void(*)(GLint, GLint)) load("glUniform1i");
    glUniform2f = (void(*)(GLint, GLfloat, GLfloat)) load("glUniform2f");
    glUniform3f = (void(*)(GLint, GLfloat, GLfloat, GLfloat)) load("glUniform3f");
    return 1;
}
