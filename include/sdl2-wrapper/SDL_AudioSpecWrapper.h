#pragma once
#include <base/Wrapper.h>
#include <ffmpeg-wrapper/info-collection/IAudioFrameInfoCollection.h>
#include <sdl2-wrapper/pch.h>

namespace video
{
	/// @brief 音频设备包装器
	class SDL_AudioSpecWrapper :
		public base::Wrapper<SDL_AudioSpec>,
		public IAudioFrameInfoCollection,
		public base::IJsonSerializable
	{
	private:
		SDL_AudioSpec _spec{};
		SDL_AudioSpec *_wrapped_obj = &_spec;

	public:
		SDL_AudioSpecWrapper() = default;
		SDL_AudioSpecWrapper(IAudioFrameInfoCollection const &infos);
		SDL_AudioSpecWrapper(IAudioStreamInfoCollection const &infos);
		SDL_AudioSpecWrapper(SDL_AudioSpecWrapper const &o);
		SDL_AudioSpecWrapper &operator=(SDL_AudioSpecWrapper const &o);

		SDL_AudioSpec *WrappedObj() const override
		{
			return _wrapped_obj;
		}

		base::Json ToJson() const override;

#pragma region 通过 IAudioFrameInfoCollection 继承
		AVRational TimeBase() const override;
		void SetTimeBase(AVRational value) override;

		/// @brief 获取采样格式
		/// @return 如果 SDL_AudioSpec 的 format 字段内储存的 SDL 采样格式没有对应的
		/// AVSampleFormat 值，则会返回 AVSampleFormat::AV_SAMPLE_FMT_NONE
		AVSampleFormat SampleFormat() const override;

		/// @brief 设置采样格式
		/// @param value 如果设置的 AVSampleFormat 值没有对应的 SDL 值，
		/// 则会将 SDL_AudioSpec 的 format 字段设置为 0.
		void SetSampleFormat(AVSampleFormat value) override;

		int SampleRate() const override;
		void SetSampleRate(int value) override;

		AVChannelLayout ChannelLayout() const override;
		void SetChannelLayout(AVChannelLayout value) override;

		int SampleCount() const override;
		void SetSampleCount(int value) override;
#pragma endregion
	};
} // namespace video
