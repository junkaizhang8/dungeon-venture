#include <Engine.h>

#include <iostream>

int main()
{
    GLFWwindow* window;
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Error initializing GLEW" << std::endl;
        return -1;
    }

    // float positions[] = {-0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f,
    //                      0.0f,  1.0f,  0.0f, 0.5f, 0.5f, 0.0f, 0.0f,
    //                      1.0f,  -0.5f, 0.5f, 1.0f, 1.0f, 0.0f};
    float positions[] = {100.0f, 100.0f, 0.0f,   0.0f,   200.0f, 100.0f,
                         1.0f,   0.0f,   200.0f, 200.0f, 1.0f,   1.0f,
                         100.0f, 200.0f, 0.0f,   1.0f};

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VertexArray va;
    VertexBuffer vb(positions, sizeof(positions));

    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(2);
    va.addBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

    Shader shader;
    shader.addShader(Shader::Type::VERTEX, "res/shaders/default.vert");
    shader.addShader(Shader::Type::FRAGMENT, "res/shaders/default.frag");
    shader.compileShader();
    shader.bind();

    Texture texture("res/textures/bird.png");
    texture.bind();
    shader.setUniform1i("u_Texture", 0);

    va.unbind();
    vb.unbind();
    ib.unbind();
    shader.unbind();

    Renderer renderer;

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    glm::vec3 translation(200, 200, 0);

    while (!glfwWindowShouldClose(window))
    {
        renderer.clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        shader.bind();
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 mvp = proj * view * model;
        shader.setUniformMat4f("u_MVP", mvp);

        renderer.draw(va, ib, shader);

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");  // Create a window called "Hello,
                                            // world!" and append into it.

            ImGui::Text(
                "This is some useful text.");  // Display some text (you can use
                                               // a format strings too)
            ImGui::Checkbox("Demo Window",
                            &show_demo_window);  // Edit bools storing our
                                                 // window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);
            ImGui::ColorEdit3(
                "clear color",
                (float*)&clear_color);  // Edit 3 floats representing a color

            if (ImGui::Button(
                    "Button"))  // Buttons return true when clicked (most
                                // widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}