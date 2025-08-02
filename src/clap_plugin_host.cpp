#include "clap_plugin_host.h"

#include "godot_cpp/core/print_string.hpp"
#include <dlfcn.h>

#include <clap/helpers/host.hxx>
#include <clap/helpers/plugin-proxy.hxx>
#include <clap/helpers/reducing-param-queue.hxx>

// Instantiate
template class clap::helpers::Host<PluginHost_MH, PluginHost_CL>;

template class clap::helpers::PluginProxy<PluginHost_MH, PluginHost_CL>;

void ClapPluginHost::requestRestart() noexcept {
	godot::print_line("Requesting restart");
}
void ClapPluginHost::requestProcess() noexcept {
	godot::print_line("Requesting process");
}
void ClapPluginHost::requestCallback() noexcept {
	godot::print_line("Requesting callback");
}
bool ClapPluginHost::implementsGui() const noexcept { return false; }
void ClapPluginHost::guiResizeHintsChanged() noexcept {
	godot::print_line("Requesting resize hints");
}
bool ClapPluginHost::guiRequestResize(uint32_t width, uint32_t height) noexcept {
	godot::print_line("Requesting resize");
	return false;
}
bool ClapPluginHost::guiRequestShow() noexcept {
	godot::print_line("Requesting show");
	return false;
}
bool ClapPluginHost::guiRequestHide() noexcept {
	godot::print_line("Requesting hide");
	return false;
}
void ClapPluginHost::guiClosed(bool wasDestroyed) noexcept {
	godot::print_line("Requesting closed");
}
void ClapPluginHost::logLog(clap_log_severity severity, const char *message) const noexcept {
	godot::print_line("Requesting log");
}
void ClapPluginHost::paramsRescan(clap_param_rescan_flags flags) noexcept {
	godot::print_line("Requesting rescan");
}
void ClapPluginHost::paramsClear(clap_id paramId, clap_param_clear_flags flags) noexcept {
	godot::print_line("Requesting clear");
}
void ClapPluginHost::paramsRequestFlush() noexcept {
	godot::print_line("Requesting flush");
}
bool ClapPluginHost::posixFdSupportRegisterFd(int fd, clap_posix_fd_flags_t flags) noexcept {
	godot::print_line("Requesting register fd");
	return false;
}
bool ClapPluginHost::posixFdSupportModifyFd(int fd, clap_posix_fd_flags_t flags) noexcept {
	godot::print_line("Requesting modify fd");
	return false;
}
bool ClapPluginHost::posixFdSupportUnregisterFd(int fd) noexcept {
	godot::print_line("Requesting unregister fd");
	return false;
}
bool ClapPluginHost::implementsRemoteControls() const noexcept { return false; }
void ClapPluginHost::remoteControlsChanged() noexcept {
	godot::print_line("Requesting remote controls");
}
void ClapPluginHost::remoteControlsSuggestPage(clap_id pageId) noexcept {
	godot::print_line("Requesting suggest page");
}
bool ClapPluginHost::implementsState() const noexcept { return false; }
void ClapPluginHost::stateMarkDirty() noexcept {
	godot::print_line("state marke dirty");
}
bool ClapPluginHost::implementsTimerSupport() const noexcept { return false; }
bool ClapPluginHost::timerSupportRegisterTimer(uint32_t periodMs, clap_id *timerId) noexcept {
	godot::print_line("Requesting timer register");
	return false;
}
bool ClapPluginHost::timerSupportUnregisterTimer(clap_id timerId) noexcept {
	return false;
}
bool ClapPluginHost::threadCheckIsMainThread() const noexcept {
	godot::print_line("Requesting thread check");
	return false;
}
bool ClapPluginHost::threadCheckIsAudioThread() const noexcept {
	godot::print_line("Requesting thread check");
	return false;
}
bool ClapPluginHost::implementsThreadPool() const noexcept { return false; }
bool ClapPluginHost::threadPoolRequestExec(uint32_t numTasks) noexcept {
	godot::print_line("Requesting thread pool");
	return false;
}

ClapPluginHost::ClapPluginHost() :
		BaseHost("Godot Clap Host", // name
				"ivorius", // vendor
				"0.1.0", // version
				"https://github.com/ivorforce" // url
		) {
}

bool ClapPluginHost::load(const char *path, int plugin_index) {
	void *handle = dlopen(path, RTLD_NOW);

	// Clear any existing errors
	dlerror();

	// Get the function from the library
	_plugin_entry = reinterpret_cast<const struct clap_plugin_entry *>(dlsym(handle, "clap_entry"));
	const char *dlsym_error = dlerror();
	if (dlsym_error || !_plugin_entry) {
		fprintf(stderr, "Error finding symbol 'clap_entry': %s\n", dlsym_error);
		dlclose(handle);
		return false;
	}

	_plugin_entry->init(path);

	_plugin_factory = static_cast<const clap_plugin_factory *>(_plugin_entry->get_factory(CLAP_PLUGIN_FACTORY_ID));
	auto count = _plugin_factory->get_plugin_count(_plugin_factory);
	if (plugin_index >= count) {
		godot::print_line("Not found");
		return false;
	}

	auto desc = _plugin_factory->get_plugin_descriptor(_plugin_factory, plugin_index);
	if (!desc) {
		godot::print_line("No plugin descriptor");
		return false;
	}

	if (!clap_version_is_compatible(desc->clap_version)) {
		// qWarning() << "Incompatible clap version: Plugin is: " << desc->clap_version.major << "."
		// 		   << desc->clap_version.minor << "." << desc->clap_version.revision << " Host is "
		// 		   << CLAP_VERSION.major << "." << CLAP_VERSION.minor << "." << CLAP_VERSION.revision;
		godot::print_line("Incompatible clap version");
		return false;
	}

	godot::print_line("Loaded plugin with id: %s", godot::String::utf8(desc->id));

	const auto plugin = _plugin_factory->create_plugin(_plugin_factory, clapHost(), desc->id);
	if (!plugin) {
		godot::print_line("could not create the plugin");
		return false;
	}

	_plugin = std::make_unique<PluginProxy>(*plugin, *this);

	if (!_plugin->init()) {
		godot::print_line("could not init the plugin");
		return false;
	}

	// TODO
	// scanParams();
	// scanQuickControls();
	//
	// pluginLoadedChanged(true);

	return true;
}
