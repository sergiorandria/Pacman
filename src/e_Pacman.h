#pragma once

#include "e_AliveEntity.h"

#include <mutex>

namespace g_PacmanEntityDecl {
class Pacman : public g_PacmanEntityDecl::AliveEntity {
private:
  static Pacman *instance;
  static std::mutex p_MutexLock;

  Pacman() {}

public:
  Pacman(const Pacman &cpObject) = delete;
  static Pacman *Instance() noexcept;

  void e_MvLogic() noexcept override;
};
} // namespace g_PacmanEntityDecl
