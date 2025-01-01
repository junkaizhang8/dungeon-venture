#pragma once

#include <vector>

#include "events/Event.h"

namespace Engine
{
    /**
     * @brief A class that represents a layer.
     *
     * This class represents a layer. Layers are objects that can be pushed onto
     * the layer stack. Each layer is updated during each update loop and events
     * are dispatched to them in the order they are pushed.
     */
    class Layer
    {
    public:
        /**
         * @brief Destroys the Layer object.
         *
         * This destructor destroys the Layer object and frees any resources
         * associated with it.
         */
        virtual ~Layer() = default;

        /**
         * @brief Called when the layer is attached to the layer stack.
         *
         * This method is called when the layer is attached to the layer stack.
         */
        virtual void onAttach() = 0;

        /**
         * @brief Called when the layer is detached from the layer stack.
         *
         * This method is called when the layer is detached from the layer
         * stack.
         */
        virtual void onDetach() = 0;

        /**
         * @brief Updates the layer.
         *
         * This method updates the layer.
         *
         * @param deltaTime The time since the last update.
         */
        virtual void onUpdate(float deltaTime) = 0;

        /**
         * @brief Called when an event is received.
         *
         * This method is called when an event is received.
         *
         * @param event The event received.
         */
        virtual void onEvent(Event& event);

    protected:
        // The event dispatcher for the layer
        EventDispatcher dispatcher;
    };

    /**
     * @brief A class that represents a stack of layers.
     *
     * This class represents a stack of layers. Layers are objects that can be
     * pushed onto the layer stack.
     */
    class LayerStack
    {
    public:
        /**
         * @brief Destroys the LayerStack object.
         *
         * This destructor destroys the LayerStack object and frees any
         * resources associated with it.
         */
        ~LayerStack();

        /**
         * @brief Pushes a layer onto the layer stack.
         *
         * @param layer The layer to push.
         */
        void pushLayer(Layer* layer);

        /**
         * @brief Pops a layer from the layer stack.
         *
         * @param layer The layer to pop.
         */
        void popLayer(Layer* layer);

        /**
         * @brief Gets an iterator to the top of the layer stack.
         *
         * @return An iterator to the top of the layer stack.
         */
        std::vector<Layer*>::iterator begin() { return layers.begin(); }

        /**
         * @brief Gets an iterator to the bottom of the layer stack.
         *
         * @return An iterator to the bottom of the layer stack.
         */
        std::vector<Layer*>::iterator end() { return layers.end(); }

    private:
        // The layers in the stack
        std::vector<Layer*> layers;
    };
}  // namespace Engine