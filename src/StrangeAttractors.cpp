#include "Nohmad.hpp"

struct LorenzAttractor {
    float x, y, z;
    float sigma, beta, rho;
    float pitchX, pitchY, pitchZ;

    LorenzAttractor() :
        x(1.0), y(1.0), z(1.0),
        sigma(10.0), beta(8.0 / 3.0), rho(28.0),
        pitchX(0.5), pitchY(0.5), pitchZ(0.5) {}

    void process(float dt) {
        float dx = sigma * (y - x);
        float dy = x * (rho - z) - y;
        float dz = (x * y) - (beta * z);

        x += dx * dt * pitchX * 750.0 * 0.5;
        y += dy * dt * pitchY * 750.0 * 0.5;
        z += dz * dt * pitchZ * 750.0 * 0.5;
    }
};

struct RosslerAttractor {
    float x, y, z;
    float a, b, c;
    float pitchX, pitchY, pitchZ;

    RosslerAttractor() :
        x(1.0), y(1.0), z(1.0),
        a(0.1), b(0.1), c(14.0),
        pitchX(0.5), pitchY(0.5), pitchZ(0.5) {}

    void process(float dt) {
        float dx = -y - z;
        float dy = x + (a * y);
        float dz = b + z * (x - c);

        x += dx * dt * pitchX * 2.91 * 1000.0;
        y += dy * dt * pitchY * 2.91 * 1000.0;
        z += dz * dt * pitchZ * 2.91 * 1000.0;
    }
};

struct StrangeAttractors : Module {
	enum ParamIds {
        LORENZ_SIGMA_PARAM,
        LORENZ_BETA_PARAM,
        LORENZ_RHO_PARAM,
        LORENZ_X_PITCH_PARAM,
        LORENZ_Y_PITCH_PARAM,
        LORENZ_Z_PITCH_PARAM,
        ROSSLER_A_PARAM,
        ROSSLER_B_PARAM,
        ROSSLER_C_PARAM,
        ROSSLER_X_PITCH_PARAM,
        ROSSLER_Y_PITCH_PARAM,
        ROSSLER_Z_PITCH_PARAM,
		NUM_PARAMS
	};

	enum InputIds {
        LORENZ_SIGMA_INPUT,
        LORENZ_BETA_INPUT,
        LORENZ_RHO_INPUT,
        ROSSLER_A_INPUT,
        ROSSLER_B_INPUT,
        ROSSLER_C_INPUT,
		NUM_INPUTS
	};

	enum OutputIds {
        LORENZ_X_OUTPUT,
        LORENZ_Y_OUTPUT,
        LORENZ_Z_OUTPUT,
        ROSSLER_X_OUTPUT,
        ROSSLER_Y_OUTPUT,
        ROSSLER_Z_OUTPUT,
		NUM_OUTPUTS
	};

    LorenzAttractor lorenz;
    RosslerAttractor rossler;

	StrangeAttractors() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {

    }

	void step() override;
};

void StrangeAttractors::step() {
    if (outputs[LORENZ_X_OUTPUT].active || outputs[LORENZ_Y_OUTPUT].active || outputs[LORENZ_Z_OUTPUT].active) {
        lorenz.sigma = params[LORENZ_SIGMA_PARAM].value;
        lorenz.beta = params[LORENZ_BETA_PARAM].value;
        lorenz.rho = params[LORENZ_RHO_PARAM].value;
        lorenz.pitchX = params[LORENZ_X_PITCH_PARAM].value;
        lorenz.pitchY = params[LORENZ_Y_PITCH_PARAM].value;
        lorenz.pitchZ = params[LORENZ_Z_PITCH_PARAM].value;

        lorenz.process(1.0 / engineGetSampleRate());
        outputs[LORENZ_X_OUTPUT].value = 5.0 * 0.044 * lorenz.x;
        outputs[LORENZ_Y_OUTPUT].value = 5.0 * 0.0328 * lorenz.y;
        // outputs[LORENZ_Z_OUTPUT].value = lorenz.z;
    }

    if (outputs[ROSSLER_X_OUTPUT].active || outputs[ROSSLER_Y_OUTPUT].active || outputs[ROSSLER_Z_OUTPUT].active) {
        rossler.a = params[ROSSLER_A_PARAM].value;
        rossler.b = params[ROSSLER_B_PARAM].value;
        rossler.c = params[ROSSLER_C_PARAM].value;
        rossler.pitchX = params[ROSSLER_X_PITCH_PARAM].value;
        rossler.pitchY = params[ROSSLER_Y_PITCH_PARAM].value;
        rossler.pitchZ = params[ROSSLER_Z_PITCH_PARAM].value;

        rossler.process(1.0 / engineGetSampleRate());
        outputs[ROSSLER_X_OUTPUT].value = 5.0 * 0.054 * rossler.x;
        outputs[ROSSLER_Y_OUTPUT].value = 5.0 * 0.0569 * rossler.y;
        // outputs[ROSSLER_Z_OUTPUT].value = rossler.z;
    }
}

struct MiniTrimpot : Trimpot  {
	MiniTrimpot() {
		box.size = Vec(12, 12);
	}
};

StrangeAttractorsWidget::StrangeAttractorsWidget() {
	StrangeAttractors *module = new StrangeAttractors();
	setModule(module);
	box.size = Vec(15 * 9, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/StrangeAttractors.svg")));
		addChild(panel);
	}

    addParam(createParam<Davies1900hBlackKnob>(Vec(8, 45), module, StrangeAttractors::LORENZ_SIGMA_PARAM, 3.0, 30.0, 10.0));
    addParam(createParam<Davies1900hBlackKnob>(Vec(50, 45), module, StrangeAttractors::LORENZ_BETA_PARAM, 0.5, 3.0, 8.0 / 3.0));
    addParam(createParam<Davies1900hBlackKnob>(Vec(92.5, 45), module, StrangeAttractors::LORENZ_RHO_PARAM, 10.0, 80.0, 28.0));
    addParam(createParam<Davies1900hBlackKnob>(Vec(8, 237), module, StrangeAttractors::ROSSLER_A_PARAM, 0.0, 0.3, 0.2));
    addParam(createParam<Davies1900hBlackKnob>(Vec(50, 237), module, StrangeAttractors::ROSSLER_B_PARAM, 0.1, 1.0, 0.2));
    addParam(createParam<Davies1900hBlackKnob>(Vec(92.5, 237), module, StrangeAttractors::ROSSLER_C_PARAM, 3.0, 10.0, 5.7));

	addInput(createInput<PJ301MPort>(Vec(12.5, 110), module, StrangeAttractors::LORENZ_SIGMA_INPUT));
	addInput(createInput<PJ301MPort>(Vec(55, 110), module, StrangeAttractors::LORENZ_BETA_INPUT));
	addInput(createInput<PJ301MPort>(Vec(97.5, 110), module, StrangeAttractors::LORENZ_RHO_INPUT));
	addInput(createInput<PJ301MPort>(Vec(12.5, 300), module, StrangeAttractors::ROSSLER_A_INPUT));
	addInput(createInput<PJ301MPort>(Vec(55, 300), module, StrangeAttractors::ROSSLER_B_INPUT));
	addInput(createInput<PJ301MPort>(Vec(97.5, 300), module, StrangeAttractors::ROSSLER_C_INPUT));

	addOutput(createOutput<PJ301MPort>(Vec(12.5, 154), module, StrangeAttractors::LORENZ_X_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(55, 154), module, StrangeAttractors::LORENZ_Y_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(97.5, 154), module, StrangeAttractors::LORENZ_Z_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(12.5, 345), module, StrangeAttractors::ROSSLER_X_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(55, 345), module, StrangeAttractors::ROSSLER_Y_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(97.5, 345), module, StrangeAttractors::ROSSLER_Z_OUTPUT));

	addParam(createParam<MiniTrimpot>(Vec(34, 175), module, StrangeAttractors::LORENZ_X_PITCH_PARAM, 0.1, 1.0, 0.5));
	addParam(createParam<MiniTrimpot>(Vec(76.5, 175), module, StrangeAttractors::LORENZ_Y_PITCH_PARAM, 0.1, 1.0, 0.5));
	addParam(createParam<MiniTrimpot>(Vec(119, 175), module, StrangeAttractors::LORENZ_Z_PITCH_PARAM, 0.1, 1.0, 0.5));
	addParam(createParam<MiniTrimpot>(Vec(34, 366), module, StrangeAttractors::ROSSLER_X_PITCH_PARAM, 0.1, 1.0, 0.5));
	addParam(createParam<MiniTrimpot>(Vec(76.5, 366), module, StrangeAttractors::ROSSLER_Y_PITCH_PARAM, 0.1, 1.0, 0.5));
	addParam(createParam<MiniTrimpot>(Vec(119, 366), module, StrangeAttractors::ROSSLER_Z_PITCH_PARAM, 0.1, 1.0, 0.5));
}
