#pragma once

#include <WebServer.h>

class WebController {
 private:
  bool Validate();
  void ChangeToAPMode();
  void ChangeToSTAMode();
  void SetRate();

 public:
  WebServer server_;

  void Setup();
};

extern WebController Web;