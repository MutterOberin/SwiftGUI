////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of SwiftGUI.                                             //
//                                                                            //
// Copyright: (c) 2015-2016 Simon Schneegans                                  //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "Helper.hpp"

#include <fstream>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

namespace swift {
namespace detail {

bool WriteTga(const std::string& filename, char* data, unsigned width, unsigned height) {
  std::ofstream tgafile( filename.c_str(), std::ios::binary );
  if (!tgafile) return false;

  char header[18]={0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  header[12] = width         & 0xFF;
  header[13] = ( width >> 8)  & 0xFF;
  header[14] = (height)       & 0xFF;
  header[15] = (height >> 8)  & 0xFF;
  header[16] = 32;
  tgafile.write((const char*)header, 18 );

  for (int y = height -1; y >= 0; y--) {
    tgafile.write(&data[width*4*y], width * 4);
  }

  tgafile.close();
  return true;
}

}
}
