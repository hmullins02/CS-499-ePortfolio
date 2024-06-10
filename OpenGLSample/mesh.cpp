#include "mesh.h"

void UCreateMesh(GLMesh& mesh, GLfloat* verts, GLushort* indices, size_t vertexCount, size_t indexCount)
{
    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerColor = 4;

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);
    glGenBuffers(2, mesh.vbos);

    // Buffer for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (floatsPerVertex + floatsPerColor) * vertexCount, verts, GL_STATIC_DRAW);

    // Buffer for index data
    mesh.nIndices = indexCount;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * mesh.nIndices, indices, GL_STATIC_DRAW);

    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor);

    // Vertex attributes
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    // Color attributes
    glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);
}

void UDestroyMesh(GLMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(2, mesh.vbos);
}
