#include "Camera.h"

namespace Engine
{
    Camera::Camera(glm::vec3 position, float aspectRatio, bool rotationEnabled)
        : position(position),
          aspectRatio(aspectRatio),
          rotationEnabled(rotationEnabled),
          movementSpeed(1.0f)
    {
    }

    void Camera::onEvent(Event& event) { dispatcher.dispatch(event); }
}  // namespace Engine