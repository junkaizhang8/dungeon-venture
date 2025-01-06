#include <Engine.h>

#include <memory>

#include "EditorApp.h"
#include "EditorLayer.h"

int main()
{
    std::unique_ptr<Editor> app = std::make_unique<Editor>();
    app->run();
    return 0;
}