#include <iostream>

#define FASE_USE_ADD_FUNCTION_BUILDER_MACRO
#include <fase/callable.h>
#include <fase/editor.h>
#include <fase/fase.h>

#include <functional>
#include <thread>

#include "game.h"
#include "window.h"

class KeyPressStorer {
public:
  void start(GLFWwindow *window);
  float getPressTime(int key);

  ~KeyPressStorer() {
    end_frag = true;
    if (key_check_th.joinable()) {
      key_check_th.join();
    }
  }

private:
  std::thread key_check_th;
  std::atomic_bool end_frag;

  std::mutex pressed_map_mutex;
  std::map<int, std::chrono::system_clock::time_point> pressed_map;
};

void KeyPressStorer::start(GLFWwindow *window) {
  key_check_th = std::thread([window, this] {
    while (!end_frag) {
      std::this_thread::sleep_for(std::chrono::milliseconds(17));

      std::lock_guard<std::mutex> lg(pressed_map_mutex);
      for (int key = 32; key < 348; key++) {
        int action = glfwGetKey(window, key);
        if (action == GLFW_PRESS) {
          if (!pressed_map.count(key)) {
            pressed_map[key] = std::chrono::system_clock::now();
          }
        } else {
          if (pressed_map.count(key)) {
            pressed_map.erase(key);
          }
        }
      }
    }
  });
}

float KeyPressStorer::getPressTime(int key) {
  std::lock_guard<std::mutex> lg(pressed_map_mutex);
  if (pressed_map.count(key)) {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now() - pressed_map[key])
               .count() /
           1000.f;
  }
}

int main() {
  fase::Fase<fase::Callable, fase::GUIEditor> app;

  auto window = InitOpenGL("editor");
  InitImGui(window, "##editor1");

  KeyPressStorer key_controller;
  key_controller.start(window);

  app.addFunctionBuilder<void, int, float &>(
      "KeyTime", std::function<void(int, float &)>([&](int key, float &time) {
        time = key_controller.getPressTime(key);
      }),
      {"", ""}, {"key", "dst_time"});

  ShootingGame game(window);

  game.init({&app});

  bool apply_f = false;
  app.addOptinalButton("Toggle Applying", [&]() { apply_f = !apply_f; }, "");

  RunRenderingLoop(window, app, [&]() {
    if (apply_f)
      return game.mainLoop();
    else {
      return true;
    }
  });

  return EXIT_SUCCESS;
}
