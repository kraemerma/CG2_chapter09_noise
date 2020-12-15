#ifndef SCENENOISE_H
#define SCENENOISE_H

#include "scene.h"
#include "glslprogram.h"

#include "cookbookogl.h"

#include <glm/glm.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

struct uiParams
{
    float UiBaseFreq = 4.0f;
    float UiPersist = 0.5;
    int UiWidth = 128;
    int UiHeight = 128;
};

class SceneNoise : public Scene
{
private:
    uiParams uiParams;

    GLSLProgram prog;

    GLuint quad;

    glm::vec3 lightPos;
    float angle;

    void setMatrices();
    void compileAndLinkShader();
    //
    void renderGUI();
    void drawScene();

public:
    SceneNoise();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENENOISE_H
