
#ifndef GL_UTIL_H_20181202
#define GL_UTIL_H_20181202

#include <GL/gl3w.h>

#include <iostream>
#include <vector>

static GLuint LoadShaders(const std::string& vertex_shader_code,
                          const std::string& fragment_shader_code) {
  GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

  GLint result = GL_FALSE;
  int log_length;
  auto check_shader = [&](GLuint id) {
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
    std::vector<char> message(log_length);
    glGetShaderInfoLog(id, log_length, nullptr, &message[0]);
    if (!message.empty()) {
      std::clog << &message[0] << std::endl;
    }
    if (result == GL_FALSE) {
      std::terminate();
    }
  };

  // Compile vertex shader.
  char const* src_ptr = vertex_shader_code.c_str();
  glShaderSource(vertex_shader_id, 1, &src_ptr, nullptr);
  glCompileShader(vertex_shader_id);

  check_shader(vertex_shader_id);

  // Compile fragment shader.
  src_ptr = fragment_shader_code.c_str();
  glShaderSource(fragment_shader_id, 1, &src_ptr, nullptr);
  glCompileShader(fragment_shader_id);

  check_shader(fragment_shader_id);

  // link rendering pipeline.
  GLuint program_id = glCreateProgram();

  glAttachShader(program_id, vertex_shader_id);
  glAttachShader(program_id, fragment_shader_id);
  glLinkProgram(program_id);

  // check linking.
  glGetProgramiv(program_id, GL_LINK_STATUS, &result);
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
  std::vector<char> err_m(std::max(log_length, int(1)));
  glGetProgramInfoLog(program_id, log_length, nullptr, &err_m[0]);

  if (err_m.size() != 1) {
    std::clog << &err_m[0] << std::endl;
  }
  if (result == GL_FALSE) {
    std::terminate();
  }

  // delete shader programs.
  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);

  return program_id;
}

class VBO {
public:
  VBO() = default;
  VBO(VBO&) = delete;
  VBO(const VBO&) = delete;
  VBO(VBO&& another)
      : vertex_array_id(another.vertex_array_id),
        vertex_buffer_id(another.vertex_buffer_id) {
    another.vertex_array_id = GLuint(-1);
    another.vertex_buffer_id = GLuint(-1);
  }
  VBO& operator=(VBO&) = delete;
  VBO& operator=(const VBO&) = delete;
  VBO& operator=(VBO&& another) {
    reset();

    std::swap(vertex_array_id, another.vertex_array_id);
    std::swap(vertex_buffer_id, another.vertex_buffer_id);
    return *this;
  }

  void init(const GLenum& primitive_type_, const GLenum& buffer_data_usage_,
            const std::vector<GLfloat>& data) {
    primitive_type = primitive_type_;
    buffer_data_usage = buffer_data_usage_;
    size = int(data.size());

    glGenVertexArrays(1, &vertex_array_id);
    glBindVertexArray(vertex_array_id);

    glGenBuffers(1, &vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, (data.size() * sizeof(GLfloat)), &data[0],
                 buffer_data_usage);
  }

  void setData(const std::vector<GLfloat>& data) {
    size = int(data.size());

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, (data.size() * sizeof(GLfloat)), &data[0],
                 buffer_data_usage);
  }

  void draw() {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glDrawArrays(primitive_type, 0, size);

    glDisableVertexAttribArray(0);
  }

  void reset() {
    if (vertex_array_id != GLuint(-1)) {
      glDeleteVertexArrays(1, &vertex_array_id);
      vertex_array_id = GLuint(-1);
    }
    if (vertex_buffer_id != GLuint(-1)) {
      glDeleteBuffers(1, &vertex_buffer_id);
      vertex_buffer_id = GLuint(-1);
    }
  }

  ~VBO() { reset(); }

private:
  GLuint vertex_array_id = GLuint(-1);
  GLuint vertex_buffer_id = GLuint(-1);

  GLenum primitive_type;
  GLenum buffer_data_usage;
  int size;
};

#endif  // GL_UTIL_H_20181202
