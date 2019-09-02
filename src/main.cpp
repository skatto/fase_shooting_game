#include <iostream>

#define FASE_USE_ADD_FUNCTION_BUILDER_MACRO
#include <fase/editor.h>
#include <fase/fase.h>

#include "game.h"
#include "window.h"

int main() {
  auto window = InitOpenGL("editor");
  InitImGui(window, "##editor1");

  ShootingGame game(window);

  using fase::GUIEditor;

  PartsPointerMap<GUIEditor> apps = game.init<GUIEditor>();

  std::map<std::string, std::shared_ptr<GUIEditor>> editors;

  for (auto& [key, app] : apps) {
    editors[key] = std::get<0>(app);
    editors[key]->addOptinalButton("update", [&] { game.updatePipelines(); },
                                   "update pipelines of game.");
  }

  RunRenderingLoop(window, editors, [&]() { return game.mainLoop(); });

  return EXIT_SUCCESS;
}
