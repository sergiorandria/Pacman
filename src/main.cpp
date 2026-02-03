#include "Game.h"

#include "e_Pacman.h"

#include <iostream>

using namespace g_GameEngineInternal;
using namespace g_PacmanEntityDecl;

int main() {
  Game engine;

  engine.loop();
  return 0;
}
