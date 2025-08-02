#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/classes/audio_effect_instance.hpp"

using namespace godot;

class ClapAudioEffectInstance : public AudioEffectInstance {
	GDCLASS(ClapAudioEffectInstance, AudioEffectInstance)

protected:
	static void _bind_methods();

public:
	ClapAudioEffectInstance() = default;
	~ClapAudioEffectInstance() override = default;

	void _process(const void *p_src_buffer, AudioFrame *p_dst_buffer, int32_t p_frame_count) override;
	bool _process_silence() const override;
};
