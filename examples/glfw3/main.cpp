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

struct GlfwKeyEvent: public swift::KeyEvent {
  GlfwKeyEvent(int key, int scancode, int action, int mods) {

    if (action == GLFW_RELEASE) type_ = swift::KeyEvent::Type::RELEASE;
    else                        type_ = swift::KeyEvent::Type::PRESS;

    switch(key) {
      case GLFW_KEY_SPACE:          key_ = swift::Key::SPACE;         break;
      case GLFW_KEY_A:              key_ = swift::Key::KEY_A;         break;
      case GLFW_KEY_B:              key_ = swift::Key::KEY_B;         break;
      case GLFW_KEY_C:              key_ = swift::Key::KEY_C;         break;
      case GLFW_KEY_D:              key_ = swift::Key::KEY_D;         break;
      case GLFW_KEY_E:              key_ = swift::Key::KEY_E;         break;
      case GLFW_KEY_F:              key_ = swift::Key::KEY_F;         break;
      case GLFW_KEY_G:              key_ = swift::Key::KEY_G;         break;
      case GLFW_KEY_H:              key_ = swift::Key::KEY_H;         break;
      case GLFW_KEY_I:              key_ = swift::Key::KEY_I;         break;
      case GLFW_KEY_J:              key_ = swift::Key::KEY_J;         break;
      case GLFW_KEY_K:              key_ = swift::Key::KEY_K;         break;
      case GLFW_KEY_L:              key_ = swift::Key::KEY_L;         break;
      case GLFW_KEY_M:              key_ = swift::Key::KEY_M;         break;
      case GLFW_KEY_N:              key_ = swift::Key::KEY_N;         break;
      case GLFW_KEY_O:              key_ = swift::Key::KEY_O;         break;
      case GLFW_KEY_P:              key_ = swift::Key::KEY_P;         break;
      case GLFW_KEY_Q:              key_ = swift::Key::KEY_Q;         break;
      case GLFW_KEY_R:              key_ = swift::Key::KEY_R;         break;
      case GLFW_KEY_S:              key_ = swift::Key::KEY_S;         break;
      case GLFW_KEY_T:              key_ = swift::Key::KEY_T;         break;
      case GLFW_KEY_U:              key_ = swift::Key::KEY_U;         break;
      case GLFW_KEY_V:              key_ = swift::Key::KEY_V;         break;
      case GLFW_KEY_W:              key_ = swift::Key::KEY_W;         break;
      case GLFW_KEY_X:              key_ = swift::Key::KEY_X;         break;
      case GLFW_KEY_Y:              key_ = swift::Key::KEY_Y;         break;
      case GLFW_KEY_Z:              key_ = swift::Key::KEY_Z;         break;
      case GLFW_KEY_ESCAPE:         key_ = swift::Key::ESCAPE;        break;
      case GLFW_KEY_ENTER:          key_ = swift::Key::RETURN;        break;
      case GLFW_KEY_TAB:            key_ = swift::Key::TAB;           break;
      case GLFW_KEY_BACKSPACE:      key_ = swift::Key::BACKSPACE;     break;
      case GLFW_KEY_INSERT:         key_ = swift::Key::INSERT;        break;
      case GLFW_KEY_DELETE:         key_ = swift::Key::DELETE;        break;
      case GLFW_KEY_RIGHT:          key_ = swift::Key::RIGHT;         break;
      case GLFW_KEY_LEFT:           key_ = swift::Key::LEFT;          break;
      case GLFW_KEY_DOWN:           key_ = swift::Key::DOWN;          break;
      case GLFW_KEY_UP:             key_ = swift::Key::UP;            break;
      case GLFW_KEY_PAGE_UP:        key_ = swift::Key::PAGE_UP;       break;
      case GLFW_KEY_PAGE_DOWN:      key_ = swift::Key::PAGE_DOWN;     break;
      case GLFW_KEY_HOME:           key_ = swift::Key::HOME;          break;
      case GLFW_KEY_END:            key_ = swift::Key::END;           break;
      case GLFW_KEY_CAPS_LOCK:      key_ = swift::Key::CAPS_LOCK;     break;
      case GLFW_KEY_LEFT_SHIFT:     key_ = swift::Key::LEFT_SHIFT;    break;
      case GLFW_KEY_LEFT_CONTROL:   key_ = swift::Key::LEFT_CONTROL;  break;
      case GLFW_KEY_LEFT_ALT:       key_ = swift::Key::LEFT_ALT;      break;
      case GLFW_KEY_LEFT_SUPER:     key_ = swift::Key::LEFT_SUPER;    break;
      case GLFW_KEY_RIGHT_SHIFT:    key_ = swift::Key::RIGHT_SHIFT;   break;
      case GLFW_KEY_RIGHT_CONTROL:  key_ = swift::Key::RIGHT_CONTROL; break;
      case GLFW_KEY_RIGHT_SUPER:    key_ = swift::Key::RIGHT_SUPER;   break;
      default:                      key_ = swift::Key::UNKNOWN;       break;
    }

    SetMods(mods);
  }

  GlfwKeyEvent(unsigned int key, int mods) {
    type_ = swift::KeyEvent::Type::CHARACTER;
    character_ = key;
    SetMods(mods);
  }

  void SetMods(int mods) {
    if (mods & GLFW_MOD_SHIFT)   modifiers_ |= (int)swift::Modifier::SHIFT;
    if (mods & GLFW_MOD_CONTROL) modifiers_ |= (int)swift::Modifier::CONTROL;
    if (mods & GLFW_MOD_ALT)     modifiers_ |= (int)swift::Modifier::ALT;
    if (mods & GLFW_MOD_SUPER)   modifiers_ |= (int)swift::Modifier::COMMAND;
  }
};

struct GlfwMouseEvent: public swift::MouseEvent {
  GlfwMouseEvent(double x, double y) {
    type_ = swift::MouseEvent::Type::MOVE;
    x_ = x;
    y_ = y;
  }

  GlfwMouseEvent(double scroll_amount) {
    type_ = swift::MouseEvent::Type::SCROLL;
    y_ = scroll_amount;
  }

  GlfwMouseEvent(int button, int action) {
    if      (button == GLFW_MOUSE_BUTTON_LEFT)   button_ = swift::Button::LEFT;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)  button_ = swift::Button::RIGHT;
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE) button_ = swift::Button::MIDDLE;
    else                                         button_ = (swift::Button)button;

    if      (action == GLFW_PRESS)   type_ = swift::MouseEvent::Type::PRESS;
    else if (action == GLFW_RELEASE) type_ = swift::MouseEvent::Type::RELEASE;
  }
};

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

  swift::DrawSettings settings;
  settings.full_redraws_ = true;
  settings.flip_y_ = false;
  settings.convert_to_rgba_ = true;
  web_view->SetDrawCallback(settings, [](swift::DrawEvent const& event) {
    // std::cout << "-- " << x << " " << y << " " << width << " " << height << std::endl;
    if (event.resized_) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, event.width_, event.height_, 0, GL_BGRA, GL_UNSIGNED_BYTE, event.data_);
    } else {
      glTexSubImage2D(GL_TEXTURE_2D, 0, event.x_, event.y_, event.width_, event.height_, GL_BGRA, GL_UNSIGNED_BYTE, event.data_);
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
    web_view->InjectKeyEvent(GlfwKeyEvent(key, scancode, action, mods));
  });

  glfwSetCharModsCallback(window, [](GLFWwindow* window, unsigned int key, int mods){
    web_view->InjectKeyEvent(GlfwKeyEvent(key, mods));
  });

  glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y){
    web_view->InjectMouseEvent(GlfwMouseEvent(x, y));
  });

  glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
    web_view->InjectMouseEvent(GlfwMouseEvent(button, action));
  });

  glfwSetScrollCallback(window, [](GLFWwindow* window, double x, double y){
    web_view->InjectMouseEvent(GlfwMouseEvent(y));
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

