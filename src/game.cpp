
#include "game.h"

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

void ShootingGame::init(const std::vector<fase::Callable*>& pipes) {
  // Fase Setups
  pipes[0]->fixInput<float>({"power"});
  pipes[0]->fixOutput<float>({"damage"});

  // OpenGL Setups
  std::vector<GLfloat> g_vertex_buffer_data = {
      -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
  };

  program_id = LoadShaders(vertex_shader_code, fragment_shader_code);

  polygons.emplace_back();
  polygons.back().init(GL_TRIANGLES, GL_STATIC_DRAW, g_vertex_buffer_data);
}

bool ShootingGame::mainLoop() {
  // Drawing window
  glUseProgram(program_id);

  for (auto& pol : polygons) {
    pol.draw();
  }

  return true;
}
