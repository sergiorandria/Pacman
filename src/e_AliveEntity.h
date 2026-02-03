#pragma once

#include "e_IEntity.h"

namespace g_PacmanEntityDecl {
class AliveEntity : public g_PacmanEntityDecl::Entity {
public:
  ~AliveEntity() = default;
  virtual void e_MvLogic() noexcept = 0;

  bool _eIsAlive() const override { return true; }

private:
  bool isCtrl;
};
} // namespace g_PacmanEntityDecl
