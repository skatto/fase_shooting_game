
#ifndef GAME_H_20181201
#define GAME_H_20181201

#include <fase/callable.h>

#include "gl_util.h"
#include "window.h"

class ShootingGame {
public:
  ShootingGame(GLFWwindow* game_window);
  ~ShootingGame();

  ShootingGame(ShootingGame&);
  ShootingGame(const ShootingGame&);
  ShootingGame(ShootingGame&&);

  ShootingGame& operator=(ShootingGame&);
  ShootingGame& operator=(const ShootingGame&);
  ShootingGame& operator=(ShootingGame&&);

  void init(const std::vector<fase::Callable*>& pipes);
  bool mainLoop();

private:
  class Impl;

  std::unique_ptr<Impl> impl;
};

#endif  // GAME_H_20181201
