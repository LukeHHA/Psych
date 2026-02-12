#include "OpenglRendererAPI.h"
#include "GLFW/glfw3.h"

namespace ge
{
void OpenglRendererAPI::SetClearColour(glm::vec3& colour)
{
  glClearColor(colour.x, colour.y, colour.z, 0.01f);
  glClear(GL_COLOR_BUFFER_BIT);
}
} // namespace ge
