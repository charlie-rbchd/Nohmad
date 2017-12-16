#include "Nohmad.hpp"

struct LorenzAttractor {
    float sigma, beta, rho, pitch; // In
    float x, y, z; // Out

    static constexpr float DEFAULT_SIGNMA_VALUE = 10.0;
    static constexpr float DEFAULT_BETA_VALUE = 8.0 / 3.0;
    static constexpr float DEFAULT_RHO_VALUE = 28.0;
    static constexpr float DEFAULT_PITCH_VALUE = 0.5;

    LorenzAttractor() :
        sigma(DEFAULT_SIGNMA_VALUE), beta(DEFAULT_BETA_VALUE), rho(DEFAULT_RHO_VALUE), pitch(DEFAULT_PITCH_VALUE),
        x(1.0), y(1.0), z(1.0) {}

    void process(float dt) {
        float dx = sigma * (y - x);
        float dy = x * (rho - z) - y;
        float dz = (x * y) - (beta * z);

        x += dx * dt * pitch * 375.0;
        y += dy * dt * pitch * 375.0;
        z += dz * dt * pitch * 375.0;
    }
};

struct RosslerAttractor {
    float a, b, c, pitch; // In
    float x, y, z; // Out

    static constexpr float DEFAULT_A_VALUE = 0.2;
    static constexpr float DEFAULT_B_VALUE = 0.2;
    static constexpr float DEFAULT_C_VALUE = 5.7;
    static constexpr float DEFAULT_PITCH_VALUE = 0.5;

    RosslerAttractor() :
        a(DEFAULT_A_VALUE), b(DEFAULT_B_VALUE), c(DEFAULT_C_VALUE), pitch(DEFAULT_PITCH_VALUE),
        x(1.0), y(1.0), z(1.0) {}

    void process(float dt) {
        float dx = -y - z;
        float dy = x + (a * y);
        float dz = b + z * (x - c);

        x += dx * dt * pitch * 2910.0;
        y += dy * dt * pitch * 2910.0;
        z += dz * dt * pitch * 2910.0;
    }
};

struct StrangeAttractors : Module {
	enum ParamIds {
        LORENZ_SIGMA_PARAM,
        LORENZ_BETA_PARAM,
        LORENZ_RHO_PARAM,
        LORENZ_PITCH_PARAM,
        ROSSLER_A_PARAM,
        ROSSLER_B_PARAM,
        ROSSLER_C_PARAM,
        ROSSLER_PITCH_PARAM,
		NUM_PARAMS
	};

	enum InputIds {
        LORENZ_SIGMA_INPUT,
        LORENZ_BETA_INPUT,
        LORENZ_RHO_INPUT,
        LORENZ_PITCH_INPUT,
        ROSSLER_A_INPUT,
        ROSSLER_B_INPUT,
        ROSSLER_C_INPUT,
        ROSSLER_PITCH_INPUT,
		NUM_INPUTS
	};

	enum OutputIds {
        LORENZ_X_OUTPUT,
        LORENZ_Y_OUTPUT,
        ROSSLER_X_OUTPUT,
        ROSSLER_Y_OUTPUT,
		NUM_OUTPUTS
	};

    LorenzAttractor lorenz;
    RosslerAttractor rossler;

	StrangeAttractors() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {

    }

	void step() override;
};

void StrangeAttractors::step() {
    if (outputs[LORENZ_X_OUTPUT].active || outputs[LORENZ_Y_OUTPUT].active) {
        lorenz.sigma = params[LORENZ_SIGMA_PARAM].value;
        lorenz.beta = params[LORENZ_BETA_PARAM].value;
        lorenz.rho = params[LORENZ_RHO_PARAM].value;
        lorenz.pitch = params[LORENZ_PITCH_PARAM].value;

        lorenz.process(1.0 / engineGetSampleRate());
        outputs[LORENZ_X_OUTPUT].value = 5.0 * 0.044 * lorenz.x;
        outputs[LORENZ_Y_OUTPUT].value = 5.0 * 0.0328 * lorenz.y;
    }

    if (outputs[ROSSLER_X_OUTPUT].active || outputs[ROSSLER_Y_OUTPUT].active) {
        rossler.a = params[ROSSLER_A_PARAM].value;
        rossler.b = params[ROSSLER_B_PARAM].value;
        rossler.c = params[ROSSLER_C_PARAM].value;
        rossler.pitch = params[ROSSLER_PITCH_PARAM].value;

        rossler.process(1.0 / engineGetSampleRate());
        outputs[ROSSLER_X_OUTPUT].value = 5.0 * 0.054 * rossler.x;
        outputs[ROSSLER_Y_OUTPUT].value = 5.0 * 0.0569 * rossler.y;
    }
}

StrangeAttractorsWidget::StrangeAttractorsWidget() {
	StrangeAttractors *module = new StrangeAttractors();
	setModule(module);
	box.size = Vec(15 * 12, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/StrangeAttractors.svg")));
		addChild(panel);
	}

    addParam(createParam<Davies1900hBlackKnob>(Vec(8, 45), module, StrangeAttractors::LORENZ_SIGMA_PARAM, 3.0, 30.0, LorenzAttractor::DEFAULT_SIGNMA_VALUE));
    addParam(createParam<Davies1900hBlackKnob>(Vec(50, 45), module, StrangeAttractors::LORENZ_BETA_PARAM, 0.5, 3.0, LorenzAttractor::DEFAULT_BETA_VALUE));
    addParam(createParam<Davies1900hBlackKnob>(Vec(92.5, 45), module, StrangeAttractors::LORENZ_RHO_PARAM, 13.0, 80.0, LorenzAttractor::DEFAULT_RHO_VALUE));
	addParam(createParam<Davies1900hBlackKnob>(Vec(135, 45), module, StrangeAttractors::LORENZ_PITCH_PARAM, 0.001, 1.0, LorenzAttractor::DEFAULT_PITCH_VALUE));
    addParam(createParam<Davies1900hBlackKnob>(Vec(8, 237), module, StrangeAttractors::ROSSLER_A_PARAM, 0.0, 0.2, RosslerAttractor::DEFAULT_A_VALUE));
    addParam(createParam<Davies1900hBlackKnob>(Vec(50, 237), module, StrangeAttractors::ROSSLER_B_PARAM, 0.1, 1.0, RosslerAttractor::DEFAULT_B_VALUE));
    addParam(createParam<Davies1900hBlackKnob>(Vec(92.5, 237), module, StrangeAttractors::ROSSLER_C_PARAM, 3.0, 12.0, RosslerAttractor::DEFAULT_C_VALUE));
	addParam(createParam<Davies1900hBlackKnob>(Vec(135, 237), module, StrangeAttractors::ROSSLER_PITCH_PARAM, 0.001, 1.0, RosslerAttractor::DEFAULT_PITCH_VALUE));

	addInput(createInput<PJ301MPort>(Vec(12.5, 110), module, StrangeAttractors::LORENZ_SIGMA_INPUT));
	addInput(createInput<PJ301MPort>(Vec(55, 110), module, StrangeAttractors::LORENZ_BETA_INPUT));
	addInput(createInput<PJ301MPort>(Vec(97.5, 110), module, StrangeAttractors::LORENZ_RHO_INPUT));
	addInput(createInput<PJ301MPort>(Vec(140, 110), module, StrangeAttractors::LORENZ_PITCH_INPUT));
	addInput(createInput<PJ301MPort>(Vec(12.5, 300), module, StrangeAttractors::ROSSLER_A_INPUT));
	addInput(createInput<PJ301MPort>(Vec(55, 300), module, StrangeAttractors::ROSSLER_B_INPUT));
	addInput(createInput<PJ301MPort>(Vec(97.5, 300), module, StrangeAttractors::ROSSLER_C_INPUT));
	addInput(createInput<PJ301MPort>(Vec(140, 300), module, StrangeAttractors::ROSSLER_PITCH_INPUT));

	addOutput(createOutput<PJ301MPort>(Vec(12.5, 154), module, StrangeAttractors::LORENZ_X_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(140, 154), module, StrangeAttractors::LORENZ_Y_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(12.5, 345), module, StrangeAttractors::ROSSLER_X_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(140, 345), module, StrangeAttractors::ROSSLER_Y_OUTPUT));
}
