#pragma once

#include <functional>

/**
 * @brief An interface for selectable objects.
 *
 * This interface defines the methods that selectable objects must implement.
 */
struct Selectable
{
    // Flag indicating whether the object is selected
    bool selected = false;
    // Callback for when the object is selected
    std::function<void(Selectable*)> onSelect;
    // Callback for when the object is deselected
    std::function<void(Selectable*)> onDeselect;
    // Callback for when the object is deleted
    std::function<void(Selectable*)> onDelete;

    /**
     * @brief Destroys the Selectable object.
     *
     * This destructor destroys the Selectable object.
     */
    virtual ~Selectable() = default;

    /**
     * @brief Checks if the object is selected.
     *
     * This method checks if the object is selected.
     *
     * @return True if the object is selected, false otherwise.
     */
    inline bool isSelected() const { return selected; }
};