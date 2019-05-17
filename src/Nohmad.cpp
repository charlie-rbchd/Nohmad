#include "Nohmad.hpp"

Plugin *pluginInstance;

void init(rack::Plugin *p) {
	pluginInstance = p;

	p->addModel(modelNoise);
	p->addModel(modelStrangeAttractors);
}
