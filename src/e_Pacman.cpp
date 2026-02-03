#include "e_Pacman.h"

using g_PacmanEntityDecl::Pacman;

g_PacmanEntityDecl::Pacman *g_PacmanEntityDecl::Pacman::instance = nullptr;
std::mutex g_PacmanEntityDecl::Pacman::p_MutexLock;

g_PacmanEntityDecl::Pacman *Pacman::Instance() noexcept {
  if ((Pacman *)instance == nullptr) {
    std::lock_guard<std::mutex> lock_(p_MutexLock);
    if ((Pacman *)instance == nullptr) {
      instance = new Pacman();
    }
  }

  return instance;
}

void g_PacmanEntityDecl::Pacman::e_MvLogic() noexcept {
  // Movement logic will be implemented here
  // For now, this handles the basic entity behavior
}
