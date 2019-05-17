#include "Nohmad.hpp"

#include <random>
#include <cmath>

struct NoiseGenerator {
	std::mt19937 rng;
	std::uniform_real_distribution<float> uniform;

	NoiseGenerator() : uniform(-1.0f, 1.0f) {
		rng.seed(std::random_device()());
	}

	float white() {
		return uniform(rng);
	}
};

struct PinkFilter {
	float b0, b1, b2, b3, b4, b5, b6; // Coefficients
	float y; // Out

	void process(float x) {
		b0 = 0.99886f * b0 + x * 0.0555179f;
		b1 = 0.99332f * b1 + x * 0.0750759f;
		b2 = 0.96900f * b2 + x * 0.1538520f;
		b3 = 0.86650f * b3 + x * 0.3104856f;
		b4 = 0.55000f * b4 + x * 0.5329522f;
		b5 = -0.7616f * b5 - x * 0.0168980f;
		y = b0 + b1 + b2 + b3 + b4 + b5 + b6 + x * 0.5362f;
		b6 = x * 0.115926f;
	}

	float pink() {
		return y;
	}
};

struct NotchFilter {
	float freq, bandwidth; // Params
	float a0, a1, a2, b1, b2; // Coefficients
	float x1, x2; // In
	float y1, y2; // out

	void setFreq(float value) {
		freq = value;
		computeCoefficients();
	}

	void setBandwidth(float value) {
		bandwidth = value;
		computeCoefficients();
	}

	void process(float x) {
		float y = a0 * x + a1 * x1 + a2 * x2 + b1 * y1 + b2 * y2;

		x2 = x1;
		x1 = x;
		y2 = y1;
		y1 = y;
	}

	float notch() {
		return y1;
	}

	void computeCoefficients() {
		float c2pf = cos(2.0f * M_PI * freq);
		float r = 1.0f - 3.0f * bandwidth;
		float r2 = r * r;
		float k = (1.0f - (2.0f * r * c2pf) + r2) / (2.0f - 2.0f * c2pf);

		a0 = k;
		a1 = -2.0f * k * c2pf;
		a2 = k;
		b1 = 2.0f * r * c2pf;
		b2 = -r2;
	}
};

struct Noise : Module {
	enum ParamIds {
		QUANTA_PARAM,
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

	NoiseGenerator noise;

	PinkFilter pinkFilter;
	dsp::RCFilter redFilter;
	NotchFilter greyFilter;
	dsp::RCFilter blueFilter;
	dsp::RCFilter purpleFilter;

	Noise() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS);
    configParam(QUANTA_PARAM, 0.0f, 1.0f, 0.066f);
    onSampleRateChange();
	}

	void onSampleRateChange() override {
		redFilter.setCutoff(441.0f / APP->engine->getSampleRate());
		purpleFilter.setCutoff(44100.0f / APP->engine->getSampleRate());
		blueFilter.setCutoff(44100.0f / APP->engine->getSampleRate());
		greyFilter.setFreq(1000.0f / APP->engine->getSampleRate());
		greyFilter.setBandwidth(0.3f);
	}

	void process(const ProcessArgs &args) override;
};

void Noise::process(const ProcessArgs &args) {
	float white = noise.white();
	if (outputs[PINK_OUTPUT].isConnected() || outputs[BLUE_OUTPUT].isConnected() || outputs[GREY_OUTPUT].isConnected()) {
		pinkFilter.process(white);
	}

	if (outputs[WHITE_OUTPUT].isConnected()) {
		outputs[WHITE_OUTPUT].setVoltage(5.0f * white);
	}

	if (outputs[RED_OUTPUT].isConnected()) {
		redFilter.process(white);
		outputs[RED_OUTPUT].setVoltage(5.0f * clamp(7.8f * redFilter.lowpass(), -1.0f, 1.0f));
	}

	if (outputs[PINK_OUTPUT].isConnected()) {
		outputs[PINK_OUTPUT].setVoltage(5.0f * clamp(0.18f * pinkFilter.pink(), -1.0f, 1.0f));
	}

	if (outputs[GREY_OUTPUT].isConnected()) {
		greyFilter.process(pinkFilter.pink() * 0.034);
		outputs[GREY_OUTPUT].setVoltage(5.0f * clamp(0.23f * (pinkFilter.pink() * 0.5f + greyFilter.notch() * 0.5f), -1.0f, 1.0f));
	}

	if (outputs[BLUE_OUTPUT].isConnected()) {
		blueFilter.process(pinkFilter.pink());
		outputs[BLUE_OUTPUT].setVoltage(5.0f * clamp(0.64f * blueFilter.highpass(), -1.0f, 1.0f));
	}

	if (outputs[PURPLE_OUTPUT].isConnected()) {
		purpleFilter.process(white);
		outputs[PURPLE_OUTPUT].setVoltage(5.0f * clamp(0.82f * purpleFilter.highpass(), -1.0f, 1.0f));
	}

	if (outputs[QUANTA_OUTPUT].isConnected()) {
		outputs[QUANTA_OUTPUT].setVoltage(abs(white) <= params[QUANTA_PARAM].getValue() ? 5.0f * sgn(white) : 0.0f);
	}
}

struct MiniTrimpot : Trimpot  {
	MiniTrimpot() {
		box.size = Vec(12, 12);
	}
};


struct NoiseWidget : ModuleWidget {
	NoiseWidget(Noise *module);
};

NoiseWidget::NoiseWidget(Noise *module) {
		setModule(module);
	box.size = Vec(15 * 3, 380);

	setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Noise.svg")));

	addOutput(createOutput<PJ301MPort>(Vec(10.5, 55), module, Noise::WHITE_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(10.5, 101), module, Noise::PINK_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(10.5, 150), module, Noise::RED_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(10.5, 199), module, Noise::GREY_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(10.5, 247), module, Noise::BLUE_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(10.5, 295), module, Noise::PURPLE_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(10.5, 343), module, Noise::QUANTA_OUTPUT));

	addParam(createParam<MiniTrimpot>(Vec(30, 365), module, Noise::QUANTA_PARAM));
}

Model *modelNoise = createModel<Noise, NoiseWidget>("Noise");
