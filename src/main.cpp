#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstddef>
#include <csignal>
#include <cstdlib>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"



struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FregmentSource;
};

static ShaderProgramSource ParseShader(const string& filepath)
{
    ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;
    string line;
    stringstream ss[2];
    while (getline(stream, line))
    {
        if (line.find("#shader") != string::npos)
        {
            if (line.find("vertex") != string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[(int)type] << line << '\n';
        }

    }

    return { ss[0].str(), ss[1].str()};
}

static int CompileShader(unsigned int type, const string& source){
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << endl;
        cout << message << endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const string& vertexShader, const string& fragmentShader){
    unsigned int program = glCreateProgram();
    unsigned int vertexS = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fragmentS = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    if (vertexS == 0 || fragmentS == 0) {
        cerr << "Shader compilation failed\n";
        return 0;
    }

    GLCall(glAttachShader(program, vertexS));
    GLCall(glAttachShader(program, fragmentS));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vertexS));
    GLCall(glDeleteShader(fragmentS));

    return program;
}

int main() {
    GLFWwindow* window;

    if (!glfwInit()) {
        cerr << "GLFW init failed\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "test", NULL, NULL);
    if (!window) {
        glfwTerminate();
        cerr << "Window creation failed\n";
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        cerr << "GLEW init failed\n";
        return -1;
    }

    cout << glGetString(GL_VERSION) << endl;
    {
        float positions[] = {
            -0.5f, -0.5f, //0
            0.5f, -0.5f, //1
            0.5f,  0.5f, //2
            -0.5f,  0.5f  //3
        };

        unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
        };

        unsigned int vao; //virtix array object
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        ShaderProgramSource source = ParseShader("../res/shaders/Basic.shader");

        //cout << source.VertexSource << endl << source.FregmentSource << endl;

        unsigned int shader = CreateShader(source.VertexSource, source.FregmentSource);
        if (shader == 0) {
            cerr << "Shader creation failed\n";
            return -1;
        }
        GLCall(glUseProgram(shader));

        int location = glGetUniformLocation(shader, "u_Color");
        GLCall(glUniform4f(location, 1.0f, 0.2f, 0.7f, 1.0f));

        GLCall(glBindVertexArray(0));
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        float r = 0.0f;
        float increment = 0.05f;

        while (!glfwWindowShouldClose(window)) {
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            //glDrawArrays(GL_TRIANGLES, 0, 6);
            GLCall(glUseProgram(shader));
            GLCall(glUniform4f(location, r, 0.2f, 0.7f, 1.0f));

            va.Bind();
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            GLCall(glfwSwapBuffers(window));

            GLCall(glfwPollEvents());
        }

        GLCall(glDeleteProgram(shader));
    }
    GLCall(glfwTerminate());

    return 0;
}
