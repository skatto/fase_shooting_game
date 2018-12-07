
#include "game.h"

#include <chrono>
#include <cmath>
#include <thread>

namespace {

// clang-format off

constexpr static char vertex_shader_code[] = R"(
#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;

void main(){
    gl_Position.xyz = vertexPosition_modelspace;
    gl_Position.w = 1.0;
}
 )";

constexpr static char fragment_shader_code[] = R"(
#version 330 core

out vec3 color;

void main(){
    color = vec3(1,0,0);
}
)";

// clang-format on

}  // namespace

class ShootingGame::Impl {
public:
  Impl(GLFWwindow *window_) : window(window_) {}
  void init(const std::vector<fase::Callable *> &pipes);
  bool mainLoop();

private:
  GLFWwindow *window;

  std::vector<VBO> polygons;
  GLuint program_id;
  std::vector<fase::Callable *> pipes;

  float p1_pos[3] = {};
  float p2_pos[3] = {};
};

void ShootingGame::Impl::init(const std::vector<fase::Callable *> &pipes_) {
  pipes = pipes_;
  // Fase Setups
  pipes[0]->fixInput<float, float, float>({"x", "y", "z"});
  pipes[0]->fixOutput<float, float, float>({"dst_x", "dst_y", "dst_z"});

  // OpenGL Setups
  program_id = LoadShaders(vertex_shader_code, fragment_shader_code);

  polygons.emplace_back();
  polygons.back().init(GL_TRIANGLES, GL_DYNAMIC_DRAW, {});
}

bool ShootingGame::Impl::mainLoop() {
  // Drawing window
  glUseProgram(program_id);

  try {
    try {
      (*pipes[0])(p1_pos[0], p1_pos[1], p1_pos[2])
          .get(p1_pos, p1_pos + 1, p1_pos + 2);
    } catch (fase::ErrorThrownByNode &e) {
      std::cerr << e.what() << std::endl;
      e.rethrow_nested();
    }
  } catch (std::exception &e) {
    std::cerr << "    " << e.what() << std::endl;
  }

  std::vector<GLfloat> g_vertex_buffer_data = {
      -.1f, -.1f, .0f, .1f, -.1f, .0f, .0f, .1f, .0f,
  };

  for (size_t i = 0; i < 3; i++) {
    g_vertex_buffer_data[i * 3 + 0] += p1_pos[0];
    g_vertex_buffer_data[i * 3 + 1] += p1_pos[1];
    g_vertex_buffer_data[i * 3 + 2] += p1_pos[2];
  }

  polygons.back().setData(g_vertex_buffer_data);

  for (auto &pol : polygons) {
    pol.draw();
  }

  return true;
}

// ==========================PImpl Pattern======================================
ShootingGame::ShootingGame(GLFWwindow *window)
    : impl(std::make_unique<Impl>(window)) {}
ShootingGame::~ShootingGame() {}

void ShootingGame::init(const std::vector<fase::Callable *> &pipes) {
  impl->init(pipes);
}
bool ShootingGame::mainLoop() { return impl->mainLoop(); }
