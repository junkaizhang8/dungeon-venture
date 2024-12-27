#pragma once

// Main header file for the engine. This file includes all the necessary headers

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/IndexBuffer.h"
#include "core/Renderer.h"
#include "core/Shader.h"
#include "core/Texture.h"
#include "core/VertexArray.h"
#include "core/VertexBuffer.h"