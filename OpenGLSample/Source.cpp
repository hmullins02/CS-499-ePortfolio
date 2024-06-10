// Hayley Mullins
// CS499 - Enhancement for Category 2: Algorithms and Data Structures
// 05/26/2024

#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

namespace
{ 
    //window title and size -- constants
    const char* const WINDOW_TITLE = "3D Pyramid and Cube - Time-Based Automatic Rotation & Color Changing Gradients";
    const int WINDOW_WIDTH = 1200;
    const int WINDOW_HEIGHT = 1200;

    // Struct to hold mesh data
    struct GLMesh
    {
        GLuint vao;         // Vertex Array Object
        GLuint vbos[2];     // Vertex Buffer Objects
        GLuint nIndices;    // Number of indices
    };

    // Global variables
    GLFWwindow* gWindow = nullptr;
    GLMesh gMeshPyramid;
    GLMesh gMeshCube;
    GLuint gProgramId;
    float gAngle = 0.0f;
    float gAngleCube = 0.0f;
    const float gAngleX = glm::radians(15.0f);
    const float gAngleX2 = glm::radians(-145.0f);
}

// Function prototypes
bool UInitialize(int, char* [], GLFWwindow**);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void UCreateMesh(GLMesh& mesh, GLfloat* verts, GLushort* indices);
void UDestroyMesh(GLMesh& mesh);
void URender();
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);

// Vertex shader source code
const GLchar* vertexShaderSource = GLSL(440,
    layout(location = 0) in vec3 position;

out vec3 fragPosition;
out vec4 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float uTime;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    fragPosition = position;

    // Continuous gradient based on time
    float red = 0.5 + 0.5 * sin(uTime);
    float green = 0.5 + 0.5 * sin(uTime + 2.0);
    float blue = 0.5 + 0.5 * sin(uTime + 4.0);

    // Limit the color values to avoid black
    red = max(red, 0.5);
    green = max(green, 0.5);
    blue = max(blue, 0.5);

    vertexColor = vec4(red, green, blue, 1.0);
}
);

// Fragment shader source code
const GLchar* fragmentShaderSource = GLSL(440,
    in vec3 fragPosition;
in vec4 vertexColor;

out vec4 fragmentColor;

void main()
{
    vec3 normalizedPos = normalize(fragPosition);
    vec3 gradientColor = mix(vec3(1.0, 1.0, 0.5), vec3(0.0, 1.0, 1.0), normalizedPos.y);

    fragmentColor = vec4(gradientColor * vertexColor.rgb, 1.0);
}
);

int main(int argc, char* argv[])
{
    // Initialize the GLFW window
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    GLfloat vertsPyramid[] = {

        // Vertex data for the pyramid
        // Positions        // Colors (r,g,b,a)
        -0.5f, 0.0f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, // back left base corner
         0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, // back right base corner
         0.5f, 0.0f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, // front right base corner
        -0.5f, 0.0f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, // front left base corner
         0.0f, 1.0f,  0.0f,  0.4f, 1.0f, 0.2f, 1.0f  // top vertex
    };

    // Indices for the pyramid
    GLushort indicesPyramid[] = {
        0, 3, 2,
        0, 1, 4,
        0, 1, 2,
        1, 2, 4,
        2, 3, 4,
        0, 3, 4
    };

    GLfloat vertsCube[] = {
        // Vertex data for the cube
        // Positions        // Colors (r,g,b,a)
        -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 1.0f, // back left bottom corner
         0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 1.0f, // back right bottom corner
         0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 1.0f, // front right bottom corner
        -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 1.0f, // front left bottom corner
        -0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f, // back left top corner
         0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f, // back right top corner
         0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f, // front right top corner
        -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f  // front left top corner
    };

    // Indices for the cube
    GLushort indicesCube[] = {
        0, 3, 2, 2, 1, 0,
        4, 5, 6, 6, 7, 4,
        0, 4, 7, 7, 3, 0,
        1, 5, 6, 6, 2, 1,
        0, 1, 5, 5, 4, 0,
        3, 2, 6, 6, 7, 3
    };

    // Create meshes for the pyramid and the cube
    UCreateMesh(gMeshPyramid, vertsPyramid, indicesPyramid);
    UCreateMesh(gMeshCube, vertsCube, indicesCube);

    // Create the shader program
    if (!UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId))
        return EXIT_FAILURE;

    // Set the background color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Main loop
    while (!glfwWindowShouldClose(gWindow))
    {
        // Process user input
        UProcessInput(gWindow);

        // Update rotation angles
        gAngle += 0.001f;
        gAngleCube -= 0.001f;

        // Render the scene
        URender();

        // Poll for events
        glfwPollEvents();
    }

    // Cleanup
    UDestroyMesh(gMeshPyramid);
    UDestroyMesh(gMeshCube);
    UDestroyShaderProgram(gProgramId);

    // Terminate GLFW
    glfwTerminate();
    return EXIT_SUCCESS;
}

// Initialize GLFW, create a window, and initialize GLEW
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    if (!glfwInit())
    {
        cout << "Error: GLFW initialization failed\n";
        return false;
    }

    // Create a windowed mode window and its OpenGL context
    *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (!(*window))
    {
        cout << "Error: Window creation failed\n";
        glfwTerminate();
        return false;
    }

    // Make the window's context current
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        cout << "Error: GLEW initialization failed\n";
        return false;
    }

    // Print the OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    return true;
}

// Callback function for window resizing
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Process user input
void UProcessInput(GLFWwindow* window)
{
    // Close the window if the ESC key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Create a mesh from vertex and index data
void UCreateMesh(GLMesh& mesh, GLfloat* verts, GLushort* indices)
{
    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerColor = 4;

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);
    glGenBuffers(2, mesh.vbos);

    // Buffer for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (floatsPerVertex + floatsPerColor) * 8, verts, GL_STATIC_DRAW);

    // Buffer for index data
    mesh.nIndices = 36;
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

// Render the scene
void URender()
{
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    float currentTime = glfwGetTime();

    // Transformations for the pyramid
    glm::mat4 modelPyramid = glm::translate(glm::mat4(1.0f), glm::vec3(-0.75f, 0.3f, 0.0f));
    modelPyramid = glm::rotate(modelPyramid, gAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    modelPyramid = glm::rotate(modelPyramid, gAngleX, glm::vec3(1.0f, 0.0f, 0.0f));

    // Transformations for the cube
    glm::mat4 modelCube = glm::translate(glm::mat4(1.0f), glm::vec3(0.75f, -0.3f, 0.0f));
    modelCube = glm::rotate(modelCube, gAngleCube, glm::vec3(0.0f, 1.0f, 0.0f));
    modelCube = glm::rotate(modelCube, gAngleX2, glm::vec3(1.0f, 0.0f, 0.0f));

    // View matrix
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));

    // Projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

    // Use the shader program
    glUseProgram(gProgramId);


     // Set uniform variables for the pyramid
    GLint modelLoc = glGetUniformLocation(gProgramId, "model");
    GLint viewLoc = glGetUniformLocation(gProgramId, "view");
    GLint projLoc = glGetUniformLocation(gProgramId, "projection");
    GLint timeLoc = glGetUniformLocation(gProgramId, "uTime");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPyramid));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform1f(timeLoc, currentTime);

    // Render the pyramid
    glBindVertexArray(gMeshPyramid.vao);
    glDrawElements(GL_TRIANGLES, gMeshPyramid.nIndices, GL_UNSIGNED_SHORT, nullptr);

    // Set uniform variables for the cube
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCube));
    glUniform1f(timeLoc, currentTime);

    // Render the cube
    glBindVertexArray(gMeshCube.vao);
    glDrawElements(GL_TRIANGLES, gMeshCube.nIndices, GL_UNSIGNED_SHORT, nullptr);

    // Swap buffers
    glfwSwapBuffers(gWindow);
}

// Create a shader program from vertex and fragment shader sources
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
    int success;
    char infoLog[512];

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vtxShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    // Check for compile errors
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "Error: Vertex shader compilation failed\n" << infoLog << endl;
        return false;
    }

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    // Check for compile errors
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "Error: Fragment shader compilation failed\n" << infoLog << endl;
        return false;
    }

    // Link shaders into a program
    programId = glCreateProgram();
    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    glLinkProgram(programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &success);

    // Check for linking error
    if (!success)
    {
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        cout << "Error: Shader program linking failed\n" << infoLog << endl;
        return false;
    }

    // Clean up shaders as they're linked into the program now and no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

// Destroy the shader program
void UDestroyShaderProgram(GLuint programId)
{
    glDeleteProgram(programId);
}

// Destry meshes
void UDestroyMesh(GLMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(2, mesh.vbos);
}
