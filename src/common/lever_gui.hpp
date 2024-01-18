#pragma once

#include <optional>

namespace ws {
struct PortDescriptor;
}

namespace ws::lever {
struct LeverSystem;
struct SerialLeverHandle;
}

namespace ws::gui {

struct LeverGUIParams {
  lever::LeverSystem* lever_system;
  const lever::SerialLeverHandle* levers;
  int num_levers;
  const PortDescriptor* serial_ports;
  int num_serial_ports;
  int force_limit0;
  int force_limit1;
};

struct LeverGUIResult {
  std::optional<int> force_limit0;
  std::optional<int> force_limit1;
};

LeverGUIResult render_lever_gui(const LeverGUIParams& params);

}