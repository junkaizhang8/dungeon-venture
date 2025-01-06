#include "SelectionManager.h"

void SelectionManager::select(Selectable* object)
{
    if (object)
    {
        object->onSelect(object);
        selectedObjects.push_back(object);
    }
}

void SelectionManager::deselect(Selectable* object)
{
    if (object)
    {
        object->onDeselect(object);
        selectedObjects.erase(
            std::remove(selectedObjects.begin(), selectedObjects.end(), object),
            selectedObjects.end());
    }
}

void SelectionManager::deselectAll()
{
    for (auto* object : selectedObjects) object->onDeselect(object);
    selectedObjects.clear();
}

void SelectionManager::deleteSelected()
{
    for (auto* object : selectedObjects)
    {
        if (object->onDelete) object->onDelete(object);
    }
    selectedObjects.clear();
}