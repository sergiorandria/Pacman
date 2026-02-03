#include "e_Ghost.h"

g_PacmanEntityDecl::Ghost::Ghost() 
  : ghostColor_(g_GhostProperty::GhostColor::r), isScared_(false) {
  loadTexture();
}

g_PacmanEntityDecl::Ghost::Ghost(g_GhostProperty::GhostColor color) 
  : ghostColor_(color), isScared_(false) {
  loadTexture();
}

g_PacmanEntityDecl::Ghost::Ghost(const Ghost &objCopy) { 
  *this = objCopy; 
}

void g_PacmanEntityDecl::Ghost::e_MvLogic() noexcept {
  // Ghost AI movement logic will be implemented here
  // For now, this is a placeholder
}
