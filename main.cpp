#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"
#include "Objeto.h"
#include "glut_ply.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void processInput(GLFWwindow *window);

void check_colision(vector<Objeto *> &objetos);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(-95.0f, 0.0f, 20.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float traslacion_objeto_X, traslacion_objeto_Z;
// lighting
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
std::vector<Objeto *> objs;

Model_PLY *modelo = new Model_PLY();

int main() {
    char *archivo = "../models/big_porsche.ply";
    modelo->Load(archivo);
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
    // build and compile our shader zprogram
    Shader lightingShader("../2.2.basic_lighting.vs", "../2.2.basic_lighting.fs");
    Shader lightCubeShader("../2.2.light_cube.vs", "../2.2.light_cube.fs");


    //PROBANDO MODELO esfera

    auto *mercurio = new Esfera(vec3(-5.8, 0, 0), 0.25, 20, 20);
    mercurio->setup();

    auto *venus = new Esfera(vec3(10.8, 0, 0), 0.6, 20, 20);
    venus->setup();

    auto *tierra = new Esfera(vec3(-15,0,0), 0.63, 20, 20);
    tierra->setup();

    auto *marte = new Esfera(vec3(22.8,0,0), 0.3389, 20, 20);
    marte->setup();

    auto *jupiter = new Esfera(vec3(30,0,0), 2.9, 20, 20);
    jupiter->setup();

    auto *saturno = new Esfera(vec3(50,0,0), 2.4, 20, 20);
    saturno->setup();

    auto *urano = new Esfera(vec3(70,0,0), 1.5, 20, 20);
    urano->setup();

    auto *neptuno = new Esfera(vec3(80, 0, 0), 1.4, 20, 20);
    neptuno->setup();

    Esfera sol(lightPos, 0.696340, 20, 20);
    sol.setup();

    modelo->setup();
    modelo->set_traslacion(vec3(-90, 0, 0));
    float orbital_angle = 0;

    objs.push_back(mercurio);
    objs.push_back(venus);
    objs.push_back(tierra);
    objs.push_back(marte);
    objs.push_back(jupiter);
    objs.push_back(saturno);
    objs.push_back(urano);
    objs.push_back(neptuno);
    objs.push_back(modelo);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
                                                100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);
        mercurio->display(lightingShader);
        mercurio->move_around_point(vec3(0, 0, 0), orbital_angle*0D);
        lightingShader.setVec3("objectColor", 0.431f, 0.333f, 0.059f);
        venus->display(lightingShader);
        venus->move_around_point(vec3(0, 0, 0), orbital_angle*0);
        lightingShader.setVec3("objectColor", 0.059f, 0.388f, 0.431f);
        tierra->display(lightingShader);
        tierra->move_around_point(vec3(0, 0, 0), orbital_angle*0);
        lightingShader.setVec3("objectColor", 0.729f, 0.22f, 0.078f);
        marte->display(lightingShader);
        marte->move_around_point(vec3(0, 0, 0), orbital_angle*0);
        lightingShader.setVec3("objectColor", 0.569f, 0.525f, 0.365f);
        jupiter->display(lightingShader);
        jupiter->move_around_point(vec3(0, 0, 0), orbital_angle*0);
        lightingShader.setVec3("objectColor", 0.388f, 0.349f, 0.196f);
        saturno->display(lightingShader);
        saturno->move_around_point(vec3(0, 0, 0), orbital_angle*0);
        lightingShader.setVec3("objectColor", 0.086f, 0.475f, 0.659f);
        urano->display(lightingShader);
        urano->move_around_point(vec3(0, 0, 0), orbital_angle*0);
        lightingShader.setVec3("objectColor", 0.027f, 0.349f, 0.502f);
        neptuno->display(lightingShader);
        neptuno->move_around_point(vec3(0, 0, 0), orbital_angle*0);

        modelo->change_traslacion(vec3(traslacion_objeto_X / 10000, 0, traslacion_objeto_Z / 10000));
        traslacion_objeto_X = traslacion_objeto_Z = 0;

        lightingShader.setVec3("objectColor", modelo->color);
        modelo->display(lightingShader);

        orbital_angle += 0.01 * 1/60;

        lightCubeShader.use();
        lightCubeShader.setVec3("objectColor", 1.0f, 1.0f, 0.0f);
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.1f));
        lightCubeShader.setMat4("model", model);

        sol.display(lightCubeShader);
        check_colision(objs);

        if(modelo->bs.colision(sol.bs)){
            modelo->color = vec3(1.0f, 0.0f, 0.0f);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime*10);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime*10);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime*10);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime*10);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        traslacion_objeto_X = 20;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        traslacion_objeto_X = -20;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        traslacion_objeto_Z = -20;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        traslacion_objeto_Z = 20;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void check_colision(vector<Objeto *> &objetos) {
    if (objetos.empty()) return;
    for (int i = 0; i < objetos.size() - 1; ++i) {
        for (int j = i + 1; j < objetos.size(); ++j) {
            if (objetos[i]->bs.colision(objetos[j]->bs)) {
                objetos[j]->set_traslacion(vec3(-90,0,0));
                return;
            }
        }
    }
}