#include "Nohmad.hpp"

#include <random>

struct Noise : Module {
	enum ParamIds {
		NUM_PARAMS
	};

	enum InputIds {
		NUM_INPUTS
	};

	enum OutputIds {
		WHITE_OUTPUT,
		PINK_OUTPUT,
		RED_OUTPUT,
		GREY_OUTPUT,
		BLUE_OUTPUT,
		PURPLE_OUTPUT,
		QUANTA_OUTPUT,
		NUM_OUTPUTS
	};

	Noise() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS), uniform(-1.0f, 1.0f) {
		rng.seed(std::random_device()());
	}

	void step() override;

private:
	std::mt19937 rng;
	std::uniform_real_distribution<float> uniform;
};

void Noise::step() {
	if (outputs[WHITE_OUTPUT].active) {
		outputs[WHITE_OUTPUT].value = uniform(rng);
	}
}

NoiseWidget::NoiseWidget() {
	Noise *module = new Noise();
	setModule(module);
	box.size = Vec(15*3, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/Noise.svg")));
		addChild(panel);
	}

	addOutput(createOutput<PJ301MPort>(Vec(10.5, 55), module, Noise::WHITE_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(10.5, 101), module, Noise::PINK_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(10.5, 150), module, Noise::RED_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(10.5, 199), module, Noise::GREY_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(10.5, 247), module, Noise::BLUE_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(10.5, 295), module, Noise::PURPLE_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(10.5, 343), module, Noise::QUANTA_OUTPUT));
}
