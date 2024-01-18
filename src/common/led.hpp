#pragma once

#include "time.hpp"
#include <vector>

namespace ws::led {

struct LEDSync {
	ws::TimePoint t0{};
	bool initialized{};
	int ni_channel_index{};

	float pulse_duration_s{ 0.30f };
	float pulse_high_voltage{ 3.0f };

	float sync_interval_s{ 20.0f };
	std::vector<double> sync_time_points;
	ws::TimePoint last_sync_time{};
	bool has_last_sync_time{};
};

void initialize(LEDSync* sync, ws::TimePoint t0, int ni_channel_index);
void trigger(LEDSync* sync);
void update(LEDSync* sync);

}