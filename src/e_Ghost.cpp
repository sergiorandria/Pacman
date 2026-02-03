#include "e_Ghost.h"

g_PacmanEntityDecl::Ghost::Ghost() {}

g_PacmanEntityDecl::Ghost::Ghost(const Ghost &objCopy) { *this = objCopy; }

void g_PacmanEntityDecl::Ghost::e_MvLogic() noexcept {}
