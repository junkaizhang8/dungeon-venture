#include "Layer.h"

#include "utils/EngineDebug.h"

namespace Engine
{
    LayerStack::~LayerStack()
    {
        for (auto& layer : layers) delete layer;
    }

    void LayerStack::pushLayer(Layer* layer)
    {
        // Check if the layer is already in the stack
        const auto& it = std::find(layers.begin(), layers.end(), layer);
        if (it != layers.end()) return;

        // Push the layer onto the stack
        layers.emplace(layers.begin(), layer);
        layer->onAttach();
    }

    void LayerStack::popLayer(Layer* layer)
    {
        // Find the layer in the stack
        const auto& it = std::find(layers.begin(), layers.end(), layer);
        if (it != layers.end())
        {
            // Pop the layer from the stack
            layers.erase(it);
            layer->onDetach();
        }
    }

    void Layer::onEvent(Event& event) { dispatcher.dispatch(event); }
}  // namespace Engine