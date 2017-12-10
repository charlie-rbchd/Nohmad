#pragma once
#include "rack.hpp"

using namespace rack;

extern Plugin *plugin;

////////////////////
// module widgets
////////////////////

struct NoiseWidget : ModuleWidget {
	NoiseWidget();
};

struct ChaosWidget : ModuleWidget {
	ChaosWidget();
};
