
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
  //
  // GLuint VertexArrayID;
  // glGenVertexArrays(1, &VertexArrayID);
  // glBindVertexArray(VertexArrayID);
  //
  // glGenBuffers(1, &vertexbuffer);
  // glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
  //              g_vertex_buffer_data, GL_STATIC_DRAW);
}

bool ShootingGame::mainLoop() {
  // Drawing window
  glUseProgram(program_id);

  for (auto& pol : polygons) {
    pol.draw();
  }
  //
  // glEnableVertexAttribArray(0);
  // glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  //
  // glDrawArrays(GL_TRIANGLES, 0, 3);
  //
  // glDisableVertexAttribArray(0);

  return true;
}
