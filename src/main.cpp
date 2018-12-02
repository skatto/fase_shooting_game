#include <iostream>

#define FASE_USE_ADD_FUNCTION_BUILDER_MACRO
#include <fase/callable.h>
#include <fase/editor.h>
#include <fase/fase.h>

#include "game.h"
#include "window.h"

int main() {
  fase::Fase<fase::Callable, fase::GUIEditor> app;

  auto window = InitOpenGL("editor");
  InitImGui(window, "##editor1");

  ShootingGame game;

  game.init({&app});

  RunRenderingLoop(window, app, [&]() { return game.mainLoop(); });

  return EXIT_SUCCESS;
}
