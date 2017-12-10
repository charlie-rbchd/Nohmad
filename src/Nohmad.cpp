#include "Nohmad.hpp"

Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	p->slug = "Nohmad";
#ifdef VERSION
	p->version = TOSTRING(VERSION);
#endif
	p->website = "https://github.com/joelrobichaud/Nohmad";

	p->addModel(createModel<NoiseWidget>("Nohmad", "Noise", "Noise", OSCILLATOR_TAG));
	p->addModel(createModel<ChaosWidget>("Nohmad", "Chaos", "Chaos", OSCILLATOR_TAG));
}
