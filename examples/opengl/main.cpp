////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift3D.                                              //
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

int CurrentWidth = 800;
int CurrentHeight = 600;
int WindowHandle = 0;

GLuint VertexShaderId;
GLuint FragmentShaderId;
GLuint ProgramId;
GLuint VaoId;
GLuint VboId;
GLuint ColorBufferId;
GLuint TextureId;

const std::string VertexShader = R"(
  #version 400

  layout(location=0) in vec4 in_Position;
  layout(location=1) in vec2 in_TexCoord;

  out vec2 TexCoord;

  void main() {
    gl_Position = in_Position;
    TexCoord.x = in_TexCoord.x;
    TexCoord.y = 1.0 - in_TexCoord.y;
  }
)";

const std::string FragmentShader = R"(
  #version 400

  in vec2 TexCoord;
  out vec4 out_Color;

  uniform sampler2D ourTexture;

  void main() {
    out_Color = texture(ourTexture, TexCoord);
  }
)";

void CheckError(std::string const& msg) {
  GLint ErrorCheckValue = glGetError();
  if (ErrorCheckValue != GL_NO_ERROR) {
    std::cerr << msg << ": " << gluErrorString(ErrorCheckValue) << std::endl;
  }
}


void CreateVBO() {
  GLfloat Vertices[] = {
    -0.8f, -0.9f, 0.0f, 1.0f,
     0.8f, -0.8f, 0.0f, 1.0f,
     -0.8f,  0.8f, 0.0f, 1.0f,
     0.8f,  0.9f, 0.0f, 1.0f
  };

  GLfloat Colors[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
  };

  glGenVertexArrays(1, &VaoId);
  glBindVertexArray(VaoId);

  glGenBuffers(1, &VboId);
  glBindBuffer(GL_ARRAY_BUFFER, VboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &ColorBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);

  CheckError("Could not create VBO");
}

void DestroyVBO() {
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &ColorBufferId);
  glDeleteBuffers(1, &VboId);

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &VaoId);
}

void CreateShaders() {
  VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  const char *vSource = VertexShader.c_str();
  glShaderSource(VertexShaderId, 1, &vSource, NULL);
  glCompileShader(VertexShaderId);

  FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fSource = FragmentShader.c_str();
  glShaderSource(FragmentShaderId, 1, &fSource, NULL);
  glCompileShader(FragmentShaderId);

  ProgramId = glCreateProgram();
  glAttachShader(ProgramId, VertexShaderId);
  glAttachShader(ProgramId, FragmentShaderId);
  glLinkProgram(ProgramId);
  glUseProgram(ProgramId);

  CheckError("Could not create Shader");
}

void CreateTexture() {
  glGenTextures(1, &TextureId);
  glBindTexture(GL_TEXTURE_2D, TextureId);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  CheckError("Could not create Texture");
}

void DestroyTexture() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteTextures(1, &TextureId);
}

void DestroyShaders() {
  glUseProgram(0);
  glDetachShader(ProgramId, VertexShaderId);
  glDetachShader(ProgramId, FragmentShaderId);
  glDeleteShader(FragmentShaderId);
  glDeleteShader(VertexShaderId);
  glDeleteProgram(ProgramId);
}

int main(int argc, char* argv[]) {
  swift::Gui::Init(argc, argv);

  glutInit(&argc, argv);
  glutInitContextVersion(4, 0);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  glutSetOption(
    GLUT_ACTION_ON_WINDOW_CLOSE,
    GLUT_ACTION_GLUTMAINLOOP_RETURNS
  );

  glutInitWindowSize(CurrentWidth, CurrentHeight);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

  glutCreateWindow("SwiftGui Example");

  glewExperimental = GL_TRUE;
  glewInit();
  CheckError("Glew init issue");

  glutReshapeFunc([](int Width, int Height){
    CurrentWidth = Width;
    CurrentHeight = Height;
    glViewport(0, 0, CurrentWidth, CurrentHeight);
  });

  glutDisplayFunc([](){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glutSwapBuffers();
    glutPostRedisplay();
  });

  glutIdleFunc([](){
    swift::Gui::Update();
    glutPostRedisplay();
  });

  glutCloseFunc([](){
    DestroyShaders();
    DestroyVBO();
    DestroyTexture();
    swift::Gui::CleanUp();
  });

  CreateTexture();
  CreateShaders();
  CreateVBO();

  swift::WebView webView("https://www.youtube.com/watch?v=ghV21DlDOug");

  webView.SetDrawCallback([](const std::vector<swift::Rect>& dirtyRects, const char* data){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 960, 640, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
    CheckError("Texture update issue");
  });

  glutMainLoop();

  return 0;
}

