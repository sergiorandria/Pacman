#pragma once

#include "e_AliveEntity.h"

namespace g_PacmanEntityDecl {
namespace g_GhostProperty {
enum class GhostColor { r, o, p, b };
}
class Ghost : public g_PacmanEntityDecl::AliveEntity {
public:
  Ghost();
  Ghost(const Ghost &objCopy);

  void e_MvLogic() noexcept override;
};
} // namespace g_PacmanEntityDecl
