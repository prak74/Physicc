#include "sphere.h"

Sphere::Sphere(unsigned int ID)
{
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);

    GLuint vColor = glGetAttribLocation(ID, "vColor");
    GLuint vPosition = glGetAttribLocation(ID, "vPosition");
    GLuint vNormal = glGetAttribLocation(ID, "vNormal");

    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);

    // Call the sphere function
    Lat = tesselation;
    Long = tesselation;
    sphereGen(Radius, Lat, Long);

    glBufferData(GL_ARRAY_BUFFER, sizeof(v_positions) + sizeof(v_normals), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v_positions), v_positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(v_positions), sizeof(v_normals), v_normals);

    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(v_positions)));
    glBindVertexArray(0);
}

// Create Sphere data
void Sphere::sphereGen(double radius, int Lats, int Longs)
{
    float lats, longs;
    double PI = 3.1415;

    // float slices=float(Lats)/100;
    // float sectors=float(Longs)/100;

    float slices = (180 / (float(Lats) * 10)) / 2;
    float sectors = (180 / (float(Longs) * 10)) / 2;

    float l;

    for (lats = 0.0; lats <= PI; lats += sectors)
    {
        for (longs = 0.0; longs <= 2.0 * PI; longs += slices)
        {
            float x = radius * sin(lats) * cos(longs);
            float y = radius * sin(lats) * sin(longs);
            float z = radius * cos(lats);
            glm::vec4 pt(x, y, z, 1.0);

            v_positions[tri_idx] = pt;
            v_normals[tri_idx] = pt; tri_idx++;

            if (lats + sectors > PI)
                l = PI;
            else
                l = lats + sectors;
            x = radius * sin(l) * cos(longs);
            y = radius * sin(l) * sin(longs);
            z = radius * cos(l);
            pt = glm::vec4(x, y, z, 1.0);

            v_positions[tri_idx] = pt;
            v_normals[tri_idx] = pt; tri_idx++;
        }
    }
}