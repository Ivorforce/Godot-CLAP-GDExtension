#pragma once

#include "godot_cpp/classes/audio_effect.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"

struct clap_plugin_factory;
struct clap_plugin_entry;
using namespace godot;

class ClapAudioEffect : public AudioEffect {
	GDCLASS(ClapAudioEffect, AudioEffect)

protected:
	const clap_plugin_entry * _plugin_entry;
	const clap_plugin_factory * _plugin_factory;

	static void _bind_methods();

public:
	ClapAudioEffect() = default;
	~ClapAudioEffect() override = default;

	Ref<AudioEffectInstance> _instantiate() override;

	void print_type(const Variant &p_variant) const;
};
