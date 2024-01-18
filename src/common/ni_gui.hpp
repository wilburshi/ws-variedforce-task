#pragma once

#include "sample_queue.hpp"
#include "ni.hpp"

namespace ws::led {
struct LEDSync;
}

namespace ws::gui {

struct NIGUIData {
  SampleQueue<double> sample_history;
};

void render_ni_gui(NIGUIData* gui, const ni::SampleBuffer* buffs, int num_sample_buffs, ws::led::LEDSync* sync);

}