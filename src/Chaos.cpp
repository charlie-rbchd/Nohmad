#include "Nohmad.hpp"

struct Chaos : Module {
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

	Chaos() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {

    }

	void step() override;
};

void Chaos::step() {
    if (outputs[LORENZ_X_OUTPUT].active || outputs[LORENZ_Y_OUTPUT].active || outputs[LORENZ_Z_OUTPUT].active) {
        // TODO: Compute Lorenz output
    }

    if (outputs[ROSSLER_X_OUTPUT].active || outputs[ROSSLER_Y_OUTPUT].active || outputs[ROSSLER_Z_OUTPUT].active) {
        // TODO: Compute Rossler output
    }
}

ChaosWidget::ChaosWidget() {
	Chaos *module = new Chaos();
	setModule(module);
	box.size = Vec(15 * 9, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/Chaos.svg")));
		addChild(panel);
	}

    addParam(createParam<Davies1900hBlackKnob>(Vec(8, 45), module, Chaos::LORENZ_SIGMA_PARAM, 0.0, 1.0, 0.0));
    addParam(createParam<Davies1900hBlackKnob>(Vec(50, 45), module, Chaos::LORENZ_BETA_PARAM, 0.0, 1.0, 0.0));
    addParam(createParam<Davies1900hBlackKnob>(Vec(92.5, 45), module, Chaos::LORENZ_RHO_PARAM, 0.0, 1.0, 0.0));

    addParam(createParam<Davies1900hBlackKnob>(Vec(8, 237), module, Chaos::ROSSLER_A_PARAM, 0.0, 1.0, 0.0));
    addParam(createParam<Davies1900hBlackKnob>(Vec(50, 237), module, Chaos::ROSSLER_B_PARAM, 0.0, 1.0, 0.0));
    addParam(createParam<Davies1900hBlackKnob>(Vec(92.5, 237), module, Chaos::ROSSLER_C_PARAM, 0.0, 1.0, 0.0));

	addInput(createInput<PJ301MPort>(Vec(12.5, 110), module, Chaos::LORENZ_SIGMA_INPUT));
	addInput(createInput<PJ301MPort>(Vec(55, 110), module, Chaos::LORENZ_BETA_INPUT));
	addInput(createInput<PJ301MPort>(Vec(97.5, 110), module, Chaos::LORENZ_RHO_INPUT));

	addInput(createInput<PJ301MPort>(Vec(12.5, 300), module, Chaos::ROSSLER_A_INPUT));
	addInput(createInput<PJ301MPort>(Vec(55, 300), module, Chaos::ROSSLER_B_INPUT));
	addInput(createInput<PJ301MPort>(Vec(97.5, 300), module, Chaos::ROSSLER_C_INPUT));

	addOutput(createOutput<PJ301MPort>(Vec(12.5, 154), module, Chaos::LORENZ_X_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(55, 154), module, Chaos::LORENZ_Y_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(97.5, 154), module, Chaos::LORENZ_Z_OUTPUT));

	addOutput(createOutput<PJ301MPort>(Vec(12.5, 345), module, Chaos::ROSSLER_X_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(55, 345), module, Chaos::ROSSLER_Y_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(97.5, 345), module, Chaos::ROSSLER_Z_OUTPUT));
}
