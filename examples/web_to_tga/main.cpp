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
#include <fstream>

#include <SwiftGUI/SwiftGUI.hpp>
#include <SwiftGUI/WebView.hpp>

// -----------------------------------------------------------------------------
bool WriteTga(const std::string& filename, const char* data, unsigned width, unsigned height) {
  std::ofstream tgafile( filename.c_str(), std::ios::binary );
  if (!tgafile) return false;

  char header[18]={0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  header[12] = width         & 0xFF;
  header[13] = ( width >> 8) & 0xFF;
  header[14] = (height)      & 0xFF;
  header[15] = (height >> 8) & 0xFF;
  header[16] = 32;
  tgafile.write((const char*)header, 18 );

  for (int y = height -1; y >= 0; y--) {
    tgafile.write(&data[width*4*y], width * 4);
  }

  tgafile.close();
  return true;
}

// -----------------------------------------------------------------------------
int main(int argc, char *argv[]) {

  swift::Gui::Init(argc, argv);

  swift::WebView webView("https://www.youtube.com/watch?v=ghV21DlDOug", 960, 640);

  webView.SetDrawCallback([](int x, int y, int width, int height,
                             bool resized, const char* data){
    WriteTga("test.tga", data, width, height);
  });

  while (true) {
    swift::Gui::Update();
  }

  swift::Gui::CleanUp();

  return 0;
}
