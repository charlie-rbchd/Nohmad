#include "Nohmad.hpp"

#include "dsp/filter.hpp"

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

	Noise() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS), uniform(-1.0, 1.0) {
		rng.seed(std::random_device()());
		lpf.setCutoff(441.0 / engineGetSampleRate());
		hpf.setCutoff(44100.0 / engineGetSampleRate());
	}

	void step() override;

private:
	std::mt19937 rng;
	std::uniform_real_distribution<float> uniform;
	RCFilter lpf;
	RCFilter hpf;
};

void Noise::step() {
	float randValue = uniform(rng);

	lpf.process(randValue);
	hpf.process(randValue);

	outputs[WHITE_OUTPUT].value = 5.0 * randValue;
	outputs[RED_OUTPUT].value = 5.0 * clampf(10.0 * lpf.lowpass(), -1.0, 1.0);
	outputs[PURPLE_OUTPUT].value = 5.0 * clampf(0.9 * hpf.highpass(), -1.0, 1.0);
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
