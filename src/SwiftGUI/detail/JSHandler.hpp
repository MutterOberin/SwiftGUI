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

#ifndef SWIFT_DETAIL_JS_HANDLER_HPP
#define SWIFT_DETAIL_JS_HANDLER_HPP

// includes  -------------------------------------------------------------------
#include <include/cef_v8.h>

namespace swift {
namespace detail {

class JSHandler : public CefV8Handler {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  JSHandler(CefRefPtr<CefBrowser> browser)
  : browser_(browser) {}

  virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& exception) override ;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  IMPLEMENT_REFCOUNTING(JSHandler);

  void SendError(std::string const& message) const;

  CefRefPtr<CefBrowser> browser_;
};

}
}

#endif // SWIFT_DETAIL_JS_HANDLER_HPP
