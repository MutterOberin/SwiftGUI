////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of SwiftGUI.                                             //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <iostream>

#include <SwiftGUI/SwiftGUI.hpp>
#include <SwiftGUI/WebView.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const int WIDTH = 800;
const int HEIGHT = 600;
int frames = 0;
float startTime = 0;

GLuint v_shader;
GLuint f_shader;
GLuint program;
GLuint vao;
GLuint vertex_buffer;
GLuint texture;

GLFWwindow* window;

swift::WebView* web_view;

const std::string V_SOURCE = R"(
  #version 400

  layout(location=0) in vec2 in_Position;

  out vec2 TexCoord;

  uniform bool  fullScreen;
  uniform float time;

  void main() {
    if (fullScreen) {
      gl_Position = vec4(in_Position, 0, 1);
    } else {
      gl_Position = vec4(
        cos(time) * in_Position.x - sin(time) * in_Position.y,
        sin(time) * in_Position.x + cos(time) * in_Position.y,
        0, 1);
    }

    TexCoord.x =  in_Position.x * 0.5 + 0.5;
    TexCoord.y = -in_Position.y * 0.5 + 0.5;
  }
)";

const std::string F_SOURCE = R"(
  #version 400

  in vec2 TexCoord;
  out vec4 out_Color;

  uniform bool  fullScreen;
  uniform sampler2D tex;

  void main() {
    if (fullScreen) {
      out_Color = texture(tex, TexCoord);
    } else {
      out_Color = vec4(TexCoord, 0, 1);
    }
  }
)";

int ToSwiftMods(int glfw_mods) {
  int mods = 0;

  if (glfw_mods & GLFW_MOD_SHIFT)   mods |= (int)swift::Modifier::SHIFT;
  if (glfw_mods & GLFW_MOD_CONTROL) mods |= (int)swift::Modifier::CONTROL;
  if (glfw_mods & GLFW_MOD_ALT)     mods |= (int)swift::Modifier::ALT;
  if (glfw_mods & GLFW_MOD_SUPER)   mods |= (int)swift::Modifier::COMMAND;

  return mods;
}

swift::Button ToSwiftButton(int glfw_button) {
  if (glfw_button == GLFW_MOUSE_BUTTON_LEFT)   return swift::Button::LEFT;
  if (glfw_button == GLFW_MOUSE_BUTTON_RIGHT)  return swift::Button::RIGHT;
  if (glfw_button == GLFW_MOUSE_BUTTON_MIDDLE) return swift::Button::MIDDLE;
  return (swift::Button)glfw_button;
}

void CreateResources() {

  // VBO -----------------------------------------------------------------------
  GLfloat vertices[] = {-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f};

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  // Shader --------------------------------------------------------------------
  v_shader = glCreateShader(GL_VERTEX_SHADER);
  const char *vSource = V_SOURCE.c_str();
  glShaderSource(v_shader, 1, &vSource, NULL);
  glCompileShader(v_shader);

  f_shader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fSource = F_SOURCE.c_str();
  glShaderSource(f_shader, 1, &fSource, NULL);
  glCompileShader(f_shader);

  program = glCreateProgram();
  glAttachShader(program, v_shader);
  glAttachShader(program, f_shader);
  glLinkProgram(program);
  glUseProgram(program);

  // Texture --------------------------------------------------------------------
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void DestroyResources() {

  // VBO -----------------------------------------------------------------------
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &vertex_buffer);

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vao);

  // Shader --------------------------------------------------------------------
  glUseProgram(0);
  glDetachShader(program, v_shader);
  glDetachShader(program, f_shader);
  glDeleteShader(f_shader);
  glDeleteShader(v_shader);
  glDeleteProgram(program);

  // Texture --------------------------------------------------------------------
  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteTextures(1, &texture);
}

int main(int argc, char* argv[]) {
  swift::Gui::Init(argc, argv);

  web_view = new swift::WebView("file://../share/gui.html", WIDTH, HEIGHT);
  // web_view = new swift::WebView("https://www.google.de/webhp?hl=de", WIDTH, HEIGHT);

  web_view->SetDrawCallback([](int x, int y, int width, int height,
                               bool resized, const char* data) {
    // std::cout << "-- " << x << " " << y << " " << width << " " << height << std::endl;
    if (resized) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
    } else {
      glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_BGRA, GL_UNSIGNED_BYTE, data);
    }
  });

  web_view->RegisterCallback("quit", ([]() {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }));

  web_view->RegisterCallback<double>("test", ([](double n) {
    std::cout << "huhuhu " << n << std::endl;
  }));

  web_view->RegisterCallback<double>("stats", [](double number) {
    std::cout << "Currently there are  " << number << " boxes." << std::endl;
  });

  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  window = glfwCreateWindow(WIDTH, HEIGHT, "glfw example", nullptr, nullptr);
  if (!window)
  {
      glfwTerminate();
      exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  // glfwSwapInterval(1);

  glewExperimental = GL_TRUE;
  glewInit();

  glClearColor(0.5, 0.5, 0.5, 0);
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    web_view->Resize(width, height);
  });

  glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
      web_view->InjectKeyDown(key, scancode, ToSwiftMods(mods));

      if (key == '1') {
        web_view->Reload();
      } else if (key == '2') {
        web_view->ShowDevTools();
      }
    } else if (action == GLFW_RELEASE) {
      web_view->InjectKeyUp(key, scancode, ToSwiftMods(mods));
    }
  });

  glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int key){
    web_view->InjectChar(key);
  });

  glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y){
    web_view->InjectMouseMove(x, y);
  });

  glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
      web_view->InjectButtonDown(ToSwiftButton(button));
    } else if (action == GLFW_RELEASE) {
      web_view->InjectButtonUp(ToSwiftButton(button));
    }
  });

  glfwSetScrollCallback(window, [](GLFWwindow* window, double x, double y){
    web_view->InjectMouseWheel(x*10, y*10);
  });

  CreateResources();

  while(!glfwWindowShouldClose(window)) {
    float time = glfwGetTime();
    if (++frames == 100) {
      frames = 0;
      float fps = 100.f / (time - startTime);
      startTime = time;

      web_view->CallJavascript("set_fps", fps);
    }

    swift::Gui::Update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform1i(glGetUniformLocation(program, "fullScreen"), false);
    glUniform1f(glGetUniformLocation(program, "time"), time);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glUniform1i(glGetUniformLocation(program, "fullScreen"), true);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  swift::Gui::CleanUp();
  DestroyResources();

  return 0;
}
