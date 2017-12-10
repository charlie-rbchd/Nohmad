#include "Nohmad.hpp"

struct LorenzAttractor {
    float x, y, z;
    float sigma, beta, rho;

    void process() {
        x = sigma * (y - x);
        y = x * (rho - z) - y;
        z = (x * y) - (beta * z);
    }
};

struct RosslerAttractor {
    float x, y, z;
    float a, b, c;

    void process() {
        x = -y - z;
        y = z + (a * y);
        z = b + z * (x - c);
    }
};

struct StrangeAttractors : Module {
	enum ParamIds {
        LORENZ_SIGMA_PARAM,
        LORENZ_BETA_PARAM,
        LORENZ_RHO_PARAM,
        ROSSLER_A_PARAM,
        ROSSLER_B_PARAM,
        ROSSLER_C_PARAM,
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

        lorenz.process();
        outputs[LORENZ_X_OUTPUT].value = lorenz.x;
        outputs[LORENZ_Y_OUTPUT].value = lorenz.y;
        outputs[LORENZ_Z_OUTPUT].value = lorenz.z;
    }

    if (outputs[ROSSLER_X_OUTPUT].active || outputs[ROSSLER_Y_OUTPUT].active || outputs[ROSSLER_Z_OUTPUT].active) {
        rossler.a = params[ROSSLER_A_PARAM].value;
        rossler.b = params[ROSSLER_B_PARAM].value;
        rossler.c = params[ROSSLER_C_PARAM].value;

        rossler.process();
        outputs[ROSSLER_X_OUTPUT].value = rossler.x;
        outputs[ROSSLER_Y_OUTPUT].value = rossler.y;
        outputs[ROSSLER_Z_OUTPUT].value = rossler.z;
    }
}

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

    addParam(createParam<Davies1900hBlackKnob>(Vec(8, 45), module, StrangeAttractors::LORENZ_SIGMA_PARAM, 0.0, 1.0, 0.0));
    addParam(createParam<Davies1900hBlackKnob>(Vec(50, 45), module, StrangeAttractors::LORENZ_BETA_PARAM, 0.0, 1.0, 0.0));
    addParam(createParam<Davies1900hBlackKnob>(Vec(92.5, 45), module, StrangeAttractors::LORENZ_RHO_PARAM, 0.0, 1.0, 0.0));

    addParam(createParam<Davies1900hBlackKnob>(Vec(8, 237), module, StrangeAttractors::ROSSLER_A_PARAM, 0.0, 1.0, 0.0));
    addParam(createParam<Davies1900hBlackKnob>(Vec(50, 237), module, StrangeAttractors::ROSSLER_B_PARAM, 0.0, 1.0, 0.0));
    addParam(createParam<Davies1900hBlackKnob>(Vec(92.5, 237), module, StrangeAttractors::ROSSLER_C_PARAM, 0.0, 1.0, 0.0));

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
}
