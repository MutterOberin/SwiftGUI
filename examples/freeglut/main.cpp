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
#include <GL/freeglut.h>

const int WIDTH = 800;
const int HEIGHT = 600;
int frames = 0;
int startTime = 0;

GLuint v_shader;
GLuint f_shader;
GLuint program;
GLuint vao;
GLuint vertex_buffer;
GLuint texture;

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

struct GlutKeyEvent: public swift::KeyEvent {
  GlutKeyEvent(unsigned char key, int mods, swift::KeyEvent::Type type) {
    type_         = type;
    character_    = key;
    key_          = (swift::Key)key;

    SetMods(mods);
  }

  GlutKeyEvent(int special_key, int mods, swift::KeyEvent::Type type) {
    type_ = type;

    switch(special_key) {
      case GLUT_KEY_F1:         key_ = swift::Key::F1;        break;
      case GLUT_KEY_F2:         key_ = swift::Key::F2;        break;
      case GLUT_KEY_F3:         key_ = swift::Key::F3;        break;
      case GLUT_KEY_F4:         key_ = swift::Key::F4;        break;
      case GLUT_KEY_F5:         key_ = swift::Key::F5;        break;
      case GLUT_KEY_F6:         key_ = swift::Key::F6;        break;
      case GLUT_KEY_F7:         key_ = swift::Key::F7;        break;
      case GLUT_KEY_F8:         key_ = swift::Key::F8;        break;
      case GLUT_KEY_F9:         key_ = swift::Key::F9;        break;
      case GLUT_KEY_F10:        key_ = swift::Key::F10;       break;
      case GLUT_KEY_F11:        key_ = swift::Key::F11;       break;
      case GLUT_KEY_F12:        key_ = swift::Key::F12;       break;
      case GLUT_KEY_LEFT:       key_ = swift::Key::LEFT;      break;
      case GLUT_KEY_UP:         key_ = swift::Key::UP;        break;
      case GLUT_KEY_RIGHT:      key_ = swift::Key::RIGHT;     break;
      case GLUT_KEY_DOWN:       key_ = swift::Key::DOWN;      break;
      case GLUT_KEY_PAGE_UP:    key_ = swift::Key::PAGE_UP;   break;
      case GLUT_KEY_PAGE_DOWN:  key_ = swift::Key::PAGE_DOWN; break;
      case GLUT_KEY_HOME:       key_ = swift::Key::HOME;      break;
      case GLUT_KEY_END:        key_ = swift::Key::END;       break;
      case GLUT_KEY_INSERT :    key_ = swift::Key::INSERT;    break;
    }

    SetMods(mods);
  }

  void SetMods(int mods) {
    if (mods & GLUT_ACTIVE_SHIFT) modifiers_ |= (int)swift::Modifier::SHIFT;
    if (mods & GLUT_ACTIVE_CTRL)  modifiers_ |= (int)swift::Modifier::CONTROL;
    if (mods & GLUT_ACTIVE_ALT)   modifiers_ |= (int)swift::Modifier::ALT;
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
    glutLeaveMainLoop();
  }));

  web_view->RegisterCallback<double>("test", ([](double n) {
    std::cout << "huhuhu " << n << std::endl;
  }));

  web_view->RegisterCallback<double>("stats", [](double number) {
    std::cout << "Currently there are  " << number << " boxes." << std::endl;
  });

  glutInit(&argc, argv);
  glutInitContextVersion(4, 0);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  glutSetOption(
    GLUT_ACTION_ON_WINDOW_CLOSE,
    GLUT_ACTION_GLUTMAINLOOP_RETURNS
  );

  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

  glutCreateWindow("SwiftGui Example");

  glewExperimental = GL_TRUE;
  glewInit();

  glClearColor(0.5, 0.5, 0.5, 0);
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glutReshapeFunc([](int width, int height){
    glViewport(0, 0, width, height);
    web_view->Resize(width, height);
  });

  glutDisplayFunc([](){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform1i(glGetUniformLocation(program, "fullScreen"), false);
    glUniform1f(glGetUniformLocation(program, "time"), 0.0001f * glutGet(GLUT_ELAPSED_TIME));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glUniform1i(glGetUniformLocation(program, "fullScreen"), true);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glutSwapBuffers();
    glutPostRedisplay();
  });

  glutIdleFunc([](){
    if (++frames == 100) {
      frames = 0;
      int time = glutGet(GLUT_ELAPSED_TIME);
      float fps = 100.f * 1000.f / (time - startTime);
      startTime = time;

      web_view->CallJavascript("set_fps", fps);
    }

    swift::Gui::Update();
    glutPostRedisplay();
  });

  glutKeyboardFunc([](unsigned char key, int x, int y){
    int mods = glutGetModifiers();
    web_view->InjectKeyEvent(GlutKeyEvent(key, mods, swift::KeyEvent::Type::PRESS));
    web_view->InjectKeyEvent(GlutKeyEvent(key, mods, swift::KeyEvent::Type::CHARACTER));
  });

  glutKeyboardUpFunc([](unsigned char key, int x, int y) {
    int mods = glutGetModifiers();
    web_view->InjectKeyEvent(GlutKeyEvent(key, mods, swift::KeyEvent::Type::RELEASE));
  });

  glutSpecialFunc([](int special_key, int x, int y){
    int mods = glutGetModifiers();
    web_view->InjectKeyEvent(GlutKeyEvent(special_key, mods, swift::KeyEvent::Type::PRESS));
  });

  glutSpecialUpFunc([](int special_key, int x, int y){
    int mods = glutGetModifiers();
    web_view->InjectKeyEvent(GlutKeyEvent(special_key, mods, swift::KeyEvent::Type::RELEASE));
  });

  glutMouseFunc([](int button, int state, int x, int y){
    swift::MouseEvent event;
    if (button == 3) {
      event.y_ = 10;
      event.type_ = swift::MouseEvent::Type::SCROLL;
    } else if (button == 4) {
      event.y_ = -10;
      event.type_ = swift::MouseEvent::Type::SCROLL;
    } else {
      event.button_ = (swift::Button)button;
      if (state == GLUT_DOWN) event.type_ = swift::MouseEvent::Type::PRESS;
      else                    event.type_ = swift::MouseEvent::Type::RELEASE;
    }
    web_view->InjectMouseEvent(event);
  });

  glutMotionFunc([](int x, int y){
    swift::MouseEvent event;
    event.type_ = swift::MouseEvent::Type::MOVE;
    event.x_ = x;
    event.y_ = y;
    web_view->InjectMouseEvent(event);
  });

  glutPassiveMotionFunc([](int x, int y){
    swift::MouseEvent event;
    event.type_ = swift::MouseEvent::Type::MOVE;
    event.x_ = x;
    event.y_ = y;
    web_view->InjectMouseEvent(event);
  });

  glutCloseFunc([](){
    DestroyResources();
    swift::Gui::CleanUp();
  });

  CreateResources();

  glutMainLoop();

  return 0;
}

