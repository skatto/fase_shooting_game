
#ifndef GAME_H_20181201
#define GAME_H_20181201

#include <fase/callable.h>

#include "gl_util.h"
#include "window.h"

class ShootingGame {
public:
  void init(const std::vector<fase::Callable*>& pipes);
  bool mainLoop();

private:
  std::vector<VBO> polygons;
  GLuint program_id;
};

#endif  // GAME_H_20181201
