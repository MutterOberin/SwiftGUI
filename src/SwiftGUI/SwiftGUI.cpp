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
#include "detail/Helper.hpp"

#include "SwiftGUI.hpp"

namespace swift {

void WriteTga(const std::string& filename, char* data, unsigned width, unsigned height) {
  detail::WriteTga(filename, data, width, height);
}

}
