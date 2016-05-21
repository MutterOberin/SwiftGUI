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

#ifndef SWIFT_GUI_HPP
#define SWIFT_GUI_HPP

// includes  -------------------------------------------------------------------
#include <string>

namespace swift {

class Gui {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  static void Init(int argc, char *argv[]);

  static void CleanUp();

  static void Update();
};

}

#endif // SWIFT_GUI_HPP
