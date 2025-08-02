#include "clap_effect_instance.h"

#include "clap/clap.h"

void ClapAudioEffectInstance::_bind_methods() {
}

void ClapAudioEffectInstance::_process(const void *p_src_buffer, AudioFrame *p_dst_buffer, int32_t p_frame_count) {

}

bool ClapAudioEffectInstance::_process_silence() const {
	return true;
}
