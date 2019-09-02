
#ifndef GAME_H_20181201
#define GAME_H_20181201

#include <fase/callable.h>
#include <fase/fase.h>

#include <memory>
#include <thread>

#include "gl_util.h"
#include "window.h"

struct KeyPressStorer {
  std::map<int, std::chrono::system_clock::time_point> pressed_map;
};

template <class... MyParts>
using PartsPointerMap =
    std::map<std::string, std::tuple<std::shared_ptr<MyParts>...>>;

class ShootingGame {
public:
  ShootingGame(GLFWwindow* game_window);
  ~ShootingGame() = default;

  ShootingGame(ShootingGame&);
  ShootingGame(const ShootingGame&);
  ShootingGame(ShootingGame&&);

  ShootingGame& operator=(ShootingGame&);
  ShootingGame& operator=(const ShootingGame&);
  ShootingGame& operator=(ShootingGame&&);

  template <class... MyParts>
  PartsPointerMap<MyParts...> init();

  bool mainLoop();

  void updatePipelines();

private:
  void init();

  GLFWwindow* window;

  // GLuint vertex_array_id;
  std::vector<VBO> polygons;
  GLuint program_id;

  KeyPressStorer key_press_storer;

  std::shared_ptr<fase::Callable> p1_mover;
  std::shared_ptr<fase::Callable> p2_mover;

  using PosUpdateFunc =
      std::function<void(float, float, KeyPressStorer, float*, float*)>;

  PosUpdateFunc p1_update_func;
  PosUpdateFunc p2_update_func;
  int update_func_built_version[2];

  std::array<float, 2> p1_pos = {};
  std::array<float, 2> p2_pos = {-.5f, 0.f};
};

template <class... MyParts>
PartsPointerMap<MyParts...> ShootingGame::init() {
  PartsPointerMap<MyParts...> dst;

  auto fase_instance_maker = [](auto& storer) {
    auto app = std::make_shared<fase::Fase<MyParts..., fase::Callable>>();
    FaseRegisterTestIO((*app), KeyPressStorer,
                       [](const KeyPressStorer&) -> std::string { return {}; },
                       [](const std::string&) -> KeyPressStorer { return {}; },
                       [](const KeyPressStorer&) -> std::string {
                         return "KeyPressStorer{}";
                       });
    storer = app;
    return std::make_tuple(std::shared_ptr<MyParts>(app)...);
  };

  dst["P1 position updator"] = fase_instance_maker(p1_mover);
  dst["P2 position updator"] = fase_instance_maker(p2_mover);

  init();

  return dst;
}

#endif  // GAME_H_20181201
