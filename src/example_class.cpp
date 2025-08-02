#include "example_class.h"

#include "clap/clap.h"

#include <clap_effect_instance.h>

void ClapAudioEffect::_bind_methods() {
	godot::ClassDB::bind_method(D_METHOD("print_type", "variant"), &ClapAudioEffect::print_type);
}

Ref<AudioEffectInstance> ClapAudioEffect::_instantiate() {
	return { memnew(ClapAudioEffectInstance) };
}

void ClapAudioEffect::print_type(const Variant &p_variant) const {
	clap_plugin_entry entry;
	entry.clap_version = CLAP_VERSION;
	print_line(vformat("CLAP version: %d.%d.%d", entry.clap_version.major, entry.clap_version.minor, entry.clap_version.revision));
}
