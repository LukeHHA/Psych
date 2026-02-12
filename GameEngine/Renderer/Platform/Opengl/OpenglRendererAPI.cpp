#include "OpenglRendererAPI.h"
#include "GLFW/glfw3.h"

namespace ge
{
void OpenglRendererAPI::SetClearColour(const glm::vec3& colour)
{
  glClearColor(colour.x, colour.y, colour.z, 0.01f);
}

void OpenglRendererAPI::Clear()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
} // namespace ge
