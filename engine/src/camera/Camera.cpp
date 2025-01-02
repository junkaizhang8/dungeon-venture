#include "Camera.h"

namespace Engine
{
    Camera::Camera(glm::vec3 position, float width, float height,
                   bool rotationEnabled)
        : position(position),
          width(width),
          height(height),
          rotationEnabled(rotationEnabled)
    {
    }

    void Camera::onEvent(Event& event) { dispatcher.dispatch(event); }
}  // namespace Engine