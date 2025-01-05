#include "CustomAttributeLayout.h"

#include "utils/EngineDebug.h"

namespace Engine
{
    int CustomAttributeLayout::addAttribute(AttributeType type,
                                            unsigned int count)
    {
        elements.emplace_back(type, count, GL_FALSE);
        stride += count * CustomAttribute::getSizeOfType(type);

        return Vertex::getAttributeCount() + elements.size() - 1;
    }

    const CustomAttribute* CustomAttributeLayout::getElement(int index) const
    {
        if (index < Vertex::getAttributeCount() ||
            index >= Vertex::getAttributeCount() + elements.size())
            return nullptr;
        return &elements.at(index - Vertex::getAttributeCount());
    }
}  // namespace Engine