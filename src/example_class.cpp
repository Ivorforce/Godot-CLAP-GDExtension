#include "example_class.h"

#include "clap/clap.h"

#include <clap_effect_instance.h>

#include <godot_cpp/classes/os.hpp>
#include <dlfcn.h>

void ClapAudioEffect::_bind_methods() {
	godot::ClassDB::bind_method(D_METHOD("print_type", "variant"), &ClapAudioEffect::print_type);
}

Ref<AudioEffectInstance> ClapAudioEffect::_instantiate() {
	static constexpr char path[] = "/Users/lukas/Library/Audio/Plug-Ins/CLAP/Apricot.clap/Contents/MacOS/Apricot";

	void *handle = dlopen(path, RTLD_NOW);

	// Clear any existing errors
	dlerror();

	// Get the function from the library
	_plugin_entry = reinterpret_cast<const struct clap_plugin_entry *>(dlsym(handle, "clap_entry"));
	const char *dlsym_error = dlerror();
	if (dlsym_error || !_plugin_entry) {
		fprintf(stderr, "Error finding symbol 'clap_entry': %s\n", dlsym_error);
		dlclose(handle);
		return {};
	}

	_plugin_entry->init(path);

	int32_t plugin_index = 0;

	_plugin_factory = static_cast<const clap_plugin_factory *>(_plugin_entry->get_factory(CLAP_PLUGIN_FACTORY_ID));
	auto count = _plugin_factory->get_plugin_count(_plugin_factory);
	if (plugin_index >= count) {
		print_line("Not found");
		return {};
	}

	auto desc = _plugin_factory->get_plugin_descriptor(_plugin_factory, plugin_index);
	if (!desc) {
		print_line("No plugin descriptor");
		return {};
	}

	if (!clap_version_is_compatible(desc->clap_version)) {
		// qWarning() << "Incompatible clap version: Plugin is: " << desc->clap_version.major << "."
		// 		   << desc->clap_version.minor << "." << desc->clap_version.revision << " Host is "
		// 		   << CLAP_VERSION.major << "." << CLAP_VERSION.minor << "." << CLAP_VERSION.revision;
		print_line("Incompatible clap version");
		return {};
	}

	print_line("Loaded plugin with id: %s", String::utf8(desc->id));

	// const auto plugin = _plugin_factory->create_plugin(_plugin_factory, clapHost(), desc->id);
	// if (!plugin) {
	// 	qWarning() << "could not create the plugin with id: " << desc->id;
	// 	return false;
	// }
	//
	// _plugin = std::make_unique<PluginProxy>(*plugin, *this);
	//
	// if (!_plugin->init()) {
	// 	qWarning() << "could not init the plugin with id: " << desc->id;
	// 	return false;
	// }

	return { memnew(ClapAudioEffectInstance) };
}

void ClapAudioEffect::print_type(const Variant &p_variant) const {
	clap_plugin_entry entry;
	entry.clap_version = CLAP_VERSION;
	print_line(vformat("CLAP version: %d.%d.%d", entry.clap_version.major, entry.clap_version.minor, entry.clap_version.revision));
}
