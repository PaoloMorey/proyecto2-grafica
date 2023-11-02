//
// Created by hgallegos on 5/10/2022.
//

//
// Created by hgallegos on 5/10/2022.
//

#ifndef LEARNOPENGL_OBJETO_H
#define LEARNOPENGL_OBJETO_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "shader_m.h"

using namespace std;
using namespace glm;

class BoundingSphere {
public:
    GLfloat radio;
    vec3 centro;
    vec3 traslacion = vec3(0);
    float escala = 1;

    BoundingSphere() {}

    vec3 getc() {
        return centro + traslacion;
    }

    double distancia_centros(BoundingSphere otra) {
        vec3 oc = otra.getc();
        vec3 c = this->getc();
        double D = sqrt(
                pow(oc.x - c.x, 2) + pow(oc.y - c.y, 2) + pow(oc.z - c.z, 2));
        return D;
    }

    bool colision(BoundingSphere otra) {
        return distancia_centros(otra) <= otra.radio * otra.escala + radio * escala;
    }
};

class Objeto {
public:
    vector<vec3> positions;
    vector<vec3> normals;
    vector<vec2> textureCoords;
    vector<GLuint> indices;
    GLuint indices_size;
    GLuint vao, vbo_position;
    mat4 model;
    vec3 centro;
    vec3 traslacion;
    float escala;
    vec3 rotacion;
    BoundingSphere bs;

    bool visible = true;
    bool mueve = true;

    GLint POSITION_ATTRIBUTE = 0, NORMAL_ATTRIBUTE = 1, TEXCOORD0_ATTRIBUTE = 8;

    Objeto() {
        traslacion = vec3(0);
        escala = 1;
    }

    vec3 cal_normal(vec3 v1, vec3 v2, vec3 v3) {
        return glm::cross(v2 - v1, v3 - v2);
    }

    void set_traslacion(vec3 tras) {
        this->traslacion = tras;
        this->bs.traslacion = tras;
    }

    void change_traslacion(vec3 change_vector) {
        this->traslacion += change_vector;
        this->bs.traslacion += change_vector;
    }

    void set_escala(float escala) {
        this->escala = escala;
        this->bs.escala = escala;
        this->traslacion/=escala;
        this->bs.traslacion/=escala;
    }

    virtual GLuint setup() = 0;

    virtual void display(Shader &sh) = 0;
};

class Esfera : public Objeto {
public:
    float radius;
    int slices, stacks;

    Esfera() {
        escala = 1;
        centro = vec3(0.0);
    }

    Esfera(vec3 _centro) {
        escala = 1;
        centro = _centro;
    }

    Esfera(vec3 _centro, float _radius, int _slices, int _stacks) {
        escala = 1;
        centro = _centro;
        radius = _radius;
        slices = _slices;
        stacks = _stacks;
    }

    void move_around_point(vec3 point, float orbital_angle) {
        float radius_ = sqrt(
                pow(point.x - this->bs.getc().x, 2) + pow(point.y - this->bs.getc().y, 2) + pow(point.z - this->bs.getc().z, 2));
        auto new_x = point.x + radius_ * cos(orbital_angle);
        auto new_z = point.z + radius_ * sin(orbital_angle);
        this->traslacion = vec3(new_x - this->centro.x, 0, new_z - this->centro.z);
        this->bs.traslacion = this->traslacion;
    }

    GLuint setup();

    void display(Shader &sh);
};

class Plano : public Objeto {
public:
    vec3 normal;
    float d;

    Plano(vec3 _normal, float _d) {
        normal = _normal;
        d = _d;
    }

    void display(Shader &sh) {}
};

class Piramide : public Objeto {
public:
    float rotacion;

    Piramide() {
        escala = 1;
        rotacion = 0;
    }

    GLuint setup();

    void display(Shader &sh);
};

class Cubo : public Objeto {
public:
    Cubo() { escala = 1; }

    GLuint setup() override;

    void display(Shader &sh) override;
};

#endif //LEARNOPENGL_OBJETO_H
