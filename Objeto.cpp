//
// Created by hgallegos on 5/10/2022.
//

#include "Objeto.h"

GLuint Piramide::setup() {
    vec3 v0 = vec3(0, 0, 0);
    vec3 v1 = vec3(1, 0, 0);
    vec3 v2 = vec3(0.5, 1, 0.5);
    vec3 v3 = vec3(0, 0, 1);
    positions = {v0, v2, v1, v0, v1, v3, v0, v3, v2, v1, v2, v3};
    vec3 n1 = cal_normal(v0, v2, v1);
    vec3 n2 = cal_normal(v0, v1, v3);
    vec3 n3 = cal_normal(v0, v3, v2);
    vec3 n4 = cal_normal(v1, v2, v3);
    normals = {n1, n1, n1, n2, n2, n2, n3, n3, n3, n4, n4, n4};

    //GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbos[2];
    glGenBuffers(2, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec3), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    glEnableVertexAttribArray(POSITION_ATTRIBUTE);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_TRUE, 0, (void *) 0);
    glEnableVertexAttribArray(NORMAL_ATTRIBUTE);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    indices_size = indices.size();
    return vao;
}

void Piramide::display(Shader &sh) {
    model = mat4(1.0);
    model = scale(model, vec3(escala));
    model = glm::rotate(model, glm::radians(rotacion), vec3(0, 1, 1));
    //cout << endl << to_string(centro);
    //model = translate(model, centro);
    sh.setMat4("model", model);
    if (visible) {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 12);
        glBindVertexArray(0);
    }
}

GLuint Cubo::setup() {
    float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };
    int cant = sizeof(vertices) / sizeof(float);
    positions.reserve(cant / 5);
    textureCoords.reserve(cant / 5);
    for (int i = 0; i < cant; i += 5) {
        vec3 v(vertices[i], vertices[i + 1], vertices[i + 2]);
        vec2 c(vertices[i + 3], vertices[i + 4]);
        positions.push_back(v);
        textureCoords.push_back(c);
    }
    //unsigned int VBO, VAO;
    glGenVertexArrays(1, &vao);
    GLuint vbos[2];
    glGenBuffers(2, vbos);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec3), positions.data(), GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    glEnableVertexAttribArray(POSITION_ATTRIBUTE);
    // texture coord attribute
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(vec2), textureCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(TEXCOORD0_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    glEnableVertexAttribArray(TEXCOORD0_ATTRIBUTE);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    vbo_position = vbos[0];
    return vao;
}

void Cubo::display(Shader &sh) {
    model = mat4(1.0);
    model = scale(model, vec3(escala));
    //cout << endl << to_string(centro);
    //model = translate(model, centro);
    sh.setMat4("model", model);
    if (visible) {
        glBindVertexArray(vao);
        //glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }
}


GLuint Esfera::setup() {
    const float pi = 3.1415926535897932384626433832795f;
    const float _2pi = 2.0f * pi;
    vector<vec3> positions;
    vector<vec3> normals;
    vector<vec2> textureCoords;
    for (int i = 0; i <= stacks; ++i) {
        // V texture coordinate.
        float V = i / (float) stacks;
        float phi = V * pi;
        for (int j = 0; j <= slices; ++j) {
            // U texture coordinate.
            float U = j / (float) slices;
            float theta = U * _2pi;
            float X = cos(theta) * sin(phi);
            float Y = cos(phi);
            float Z = sin(theta) * sin(phi);
            positions.push_back(vec3(X, Y, Z) * radius);
            normals.push_back(vec3(X, Y, Z));
            textureCoords.push_back(vec2(U, V));
        }
    }
    //inicializar bounding sphere
    this->bs.radio = radius;
    this->bs.centro = centro;
    this->bs.traslacion = traslacion;
    this->bs.escala = escala;

    // Now generate the index buffer
    //vector<GLuint> indicies;
    for (int i = 0; i < slices * stacks + slices; ++i) {
        indices.push_back(i);
        indices.push_back(i + slices + 1);
        indices.push_back(i + slices);
        indices.push_back(i + slices + 1);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    //GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbos[4];
    glGenBuffers(4, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec3), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    glEnableVertexAttribArray(POSITION_ATTRIBUTE);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_TRUE, 0, (void *) 0);
    glEnableVertexAttribArray(NORMAL_ATTRIBUTE);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(vec2), textureCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(TEXCOORD0_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    glEnableVertexAttribArray(TEXCOORD0_ATTRIBUTE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    indices_size = indices.size();
    return vao;
}

void Esfera::display(Shader &sh) {
    model = mat4(1.0);
    model = scale(model, vec3(escala));
    //cout << endl << to_string(centro);
    model = translate(model, centro + traslacion);
    sh.setMat4("model", model);
    if (visible) {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

}
