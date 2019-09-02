
#include "game.h"

#include <chrono>
#include <cmath>
#include <thread>

namespace {

// clang-format off

constexpr static char vertex_shader_code[] = R"(
#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;

void main() {
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

void drawPlayer(const float &x, const float &y, VBO &vbo) {
  std::vector<GLfloat> g_vertex_buffer_data = {
      -.1f, -.1f, .0f, .1f, -.1f, .0f, .0f, .1f, .0f,
  };

  for (size_t i = 0; i < 3; i++) {
    g_vertex_buffer_data[i * 3 + 0] += x;
    g_vertex_buffer_data[i * 3 + 1] += y;
  }

  vbo.setData(g_vertex_buffer_data);
}

void updateKeyInputData(GLFWwindow *window, KeyPressStorer *data) {
  for (int key = 32; key < 348; key++) {
    int action = glfwGetKey(window, key);
    if (action == GLFW_PRESS) {
      if (!data->pressed_map.count(key)) {
        data->pressed_map[key] = std::chrono::system_clock::now();
      }
    } else {
      if (data->pressed_map.count(key)) {
        data->pressed_map.erase(key);
      }
    }
  }
}

}  // namespace

//============================ ShootingGame ====================================

ShootingGame::ShootingGame(GLFWwindow *game_window) : window(game_window) {}

void ShootingGame::init() {
  // Fase Setups
  p1_mover->fixInput<float, float, KeyPressStorer>({"x", "y", "key_data"});
  p1_mover->fixOutput<float, float>({"dst_x", "dst_y"});

  p2_mover->fixInput<float, float, KeyPressStorer>({"x", "y", "key_data"});
  p2_mover->fixOutput<float, float>({"dst_x", "dst_y"});

  // OpenGL Setups
  program_id = LoadShaders(vertex_shader_code, fragment_shader_code);

  // std::clog << "p1" << std::endl;
  polygons.emplace_back();
  polygons.back().init(GL_TRIANGLES, GL_DYNAMIC_DRAW, {});
  // std::clog << "p2" << std::endl;
  // polygons.emplace_back();
  // polygons.back().init(GL_TRIANGLES, GL_DYNAMIC_DRAW, {});

  updatePipelines();
}

void ShootingGame::updatePipelines() {
  p1_update_func = p1_mover->exportPipeline<float, float, KeyPressStorer>(false)
                       .getp<float, float>();
  p2_update_func = p2_mover->exportPipeline<float, float, KeyPressStorer>(false)
                       .getp<float, float>();
}

bool ShootingGame::mainLoop() {
  updateKeyInputData(window, &key_press_storer);

  // Drawing window
  glUseProgram(program_id);

#if 0
    p1_update_func =
        p1_mover->exportPipeline<float, float, KeyPressStorer>(false)
            .getp<float, float>();
    p2_update_func =
        p2_mover->exportPipeline<float, float, KeyPressStorer>(false)
            .getp<float, float>()();
    {
      p1_update_func(p1_pos[0], p1_pos[1], key_press_storer, &p1_pos[0],
                     &p1_pos[1]);
      p2_update_func(p2_pos[0], p2_pos[1], key_press_storer, &p2_pos[0],
                     &p2_pos[1]);
    }
#endif

  try {
    try {
      p1_update_func(p1_pos[0], p1_pos[1], key_press_storer, &p1_pos[0],
                     &p1_pos[1]);
      // (*p1_mover)(p1_pos[0], p1_pos[1], key_press_storer)
      //     .get(&p1_pos[0], &p1_pos[1]);
      // (*p2_mover)(p2_pos[0], p2_pos[1], key_press_storer)
      //     .get(&p2_pos[0], &p2_pos[1]);
    } catch (fase::ErrorThrownByNode &e) {
      std::cerr << e.what() << std::endl;
      e.rethrow_nested();
    }
  } catch (std::exception &e) {
    std::cerr << "    " << e.what() << std::endl;
  }

  // std::clog << "p1" << std::endl;
  drawPlayer(p1_pos[0], p1_pos[1], polygons[0]);
  // std::clog << "p2" << std::endl;
  // drawPlayer(p2_pos[0] + 0.5, p2_pos[1], polygons[1]);

  polygons[0].draw();
  // polygons[1].draw();
  // for (auto &pol : polygons) {
  //   pol.draw();
  // }

  return true;
}
