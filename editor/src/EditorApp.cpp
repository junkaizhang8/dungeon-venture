#include "EditorApp.h"

#include "EditorLayer.h"

using namespace Engine;

Editor::Editor() : Application("Editor")
{
    pushLayer(new EditorLayer());

    dispatcher.addHandler<KeyPressedEvent>(
        [this](KeyPressedEvent& event)
        {
            if (event.getKeyCode() == GLFW_KEY_ESCAPE)
            {
                WindowCloseEvent closeEvent;
                onWindowClose(closeEvent);
            }
        });
}
