#include "Nohmad.hpp"

Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	p->slug = TOSTRING(SLUG);
	p->version = TOSTRING(VERSION);

	p->website = "https://github.com/joelrobichaud/Nohmad";

	p->addModel(modelNoise);
	p->addModel(modelStrangeAttractors);
}
