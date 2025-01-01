#include "Event.h"

namespace Engine
{
    void EventDispatcher::dispatch(Event& event) const
    {
        auto it = handlers.find(typeid(event));
        if (it != handlers.end()) it->second(event);
    }
}  // namespace Engine