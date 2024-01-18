#pragma once

#include "serial_lever.hpp"
#include "lever_system.hpp"
#include <array>

namespace ws {

struct App {
  virtual ~App() = default;
  virtual void gui_update() {}
  virtual void task_update() {}
  virtual void always_update() {}
  virtual void setup() {}
  virtual void shutdown() {}
  int run();

  std::vector<ws::PortDescriptor> ports;
  std::array<ws::lever::SerialLeverHandle, 2> levers{};
  bool start_render{};
};

}