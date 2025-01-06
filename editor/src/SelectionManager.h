#pragma once

#include <vector>

#include "Selectable.h"

/**
 * @brief A class that manages the selection of objects.
 *
 * This class provides a way to manage the selection of objects in the editor.
 */
class SelectionManager
{
public:
    /**
     * @brief Selects the specified object.
     *
     * This method selects the specified object and calls the onSelect callback.
     *
     * @param object The object to select.
     */
    void select(Selectable* object);

    /**
     * @brief Deselects the specified object.
     *
     * This method deselects the specified object and calls the onDeselect
     * callback.
     *
     * @param object The object to deselect.
     */
    void deselect(Selectable* object);

    /**
     * @brief Deselects all selected objects.
     *
     * This method deselects all selected objects and calls the onDeselect
     * callback for each object.
     */
    void deselectAll();

    /**
     * @brief Deletes all selected objects.
     *
     * This method deletes all selected objects and calls the onDelete callback
     * for each object.
     */
    void deleteSelected();

private:
    // A vector of selected objects
    std::vector<Selectable*> selectedObjects;
};