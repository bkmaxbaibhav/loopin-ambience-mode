#ifndef GLAD_H
#define GLAD_H

#include <GL/gl.h>
#include <GL/glext.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* (*GLADloadproc)(const char *name);
int gladLoadGLLoader(GLADloadproc load);

#ifdef __cplusplus
}
#endif

#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER 0x8B31
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER 0x8B30
#endif
#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS 0x8B81
#endif
#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS 0x8B82
#endif
#ifndef GL_STATIC_DRAW
#define GL_STATIC_DRAW 0x88E4
#endif
#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892
#endif

// Forward declarations for functions usually in glad
#ifdef __cplusplus
extern "C" {
#endif
extern void (APIENTRY *glGenVertexArrays)(GLsizei n, GLuint *arrays);
extern void (APIENTRY *glBindVertexArray)(GLuint array);
extern void (APIENTRY *glDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
extern void (APIENTRY *glGenBuffers)(GLsizei n, GLuint *buffers);
extern void (APIENTRY *glBindBuffer)(GLenum target, GLuint buffer);
extern void (APIENTRY *glBufferData)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
extern void (APIENTRY *glDeleteBuffers)(GLsizei n, const GLuint *buffers);
extern void (APIENTRY *glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
extern void (APIENTRY *glEnableVertexAttribArray)(GLuint index);
extern GLuint (APIENTRY *glCreateShader)(GLenum type);
extern void (APIENTRY *glShaderSource)(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
extern void (APIENTRY *glCompileShader)(GLuint shader);
extern void (APIENTRY *glGetShaderiv)(GLuint shader, GLenum pname, GLint *params);
extern void (APIENTRY *glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern void (APIENTRY *glDeleteShader)(GLuint shader);
extern GLuint (APIENTRY *glCreateProgram)(void);
extern void (APIENTRY *glAttachShader)(GLuint program, GLuint shader);
extern void (APIENTRY *glLinkProgram)(GLuint program);
extern void (APIENTRY *glGetProgramiv)(GLuint program, GLenum pname, GLint *params);
extern void (APIENTRY *glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern void (APIENTRY *glUseProgram)(GLuint program);
extern void (APIENTRY *glDeleteProgram)(GLuint program);
extern GLint (APIENTRY *glGetUniformLocation)(GLuint program, const GLchar *name);
extern void (APIENTRY *glUniform1f)(GLint location, GLfloat v0);
extern void (APIENTRY *glUniform1i)(GLint location, GLint v0);
extern void (APIENTRY *glUniform2f)(GLint location, GLfloat v0, GLfloat v1);
extern void (APIENTRY *glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
#ifdef __cplusplus
}
#endif

#endif
