#include "Nohmad.hpp"

Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	p->slug = "Nohmad";
#ifdef VERSION
	p->version = TOSTRING(VERSION);
#endif
	p->website = "https://github.com/joelrobichaud/nohmad";

	p->addModel(createModel<NoiseWidget>("Nohmad", "Noise", "Noise", OSCILLATOR_TAG));
}
