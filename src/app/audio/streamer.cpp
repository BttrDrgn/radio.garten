/*
	BASS internet radio example
	Copyright (c) 2002-2021 Un4seen Developments Ltd.
*/

#include "logger/logger.hpp"
#include "fs/fs.hpp"
#include "global.hpp"
#include "audio.hpp"
#include "settings/settings.hpp"


// update stream title from metadata
void CALLBACK meta_sync(HSYNC handle, DWORD channel, DWORD data, void* user)
{
	const char *meta = BASS_ChannelGetTags(audio::chan, BASS_TAG_META);
	if (meta)
	{
		const char *p = strstr(meta, "StreamTitle='");
		if (p)
		{
			const char *p2 = strstr(p, "';");
			if (p2)
			{
				char *t = strdup(p + 13);
				t[p2 - (p + 13)] = 0;
				audio::currently_playing.title = std::string(t);
				free(t);
			}
		}
	}
	else
	{
		meta = BASS_ChannelGetTags(audio::chan, BASS_TAG_OGG);

		if (meta)
		{
			const char *artist = NULL, *title = NULL, *p = meta;

			for (; *p; p += strlen(p) + 1)
			{
#ifdef _WIN32
				if (!strnicmp(p, "artist=", 7)) artist = p + 7;
				if (!strnicmp(p, "title=", 6)) title = p + 6;
#else
				if (!strncasecmp(p, "artist=", 7)) artist = p + 7;
				if (!strncasecmp(p, "title=", 6)) title = p + 6;
#endif
			}

			if (title)
			{
				if (artist)
				{
					char text[100];
#ifdef _WIN32
					_snprintf(text, sizeof(text), "%s - %s", artist, title);
#else
					snprintf(text, sizeof(text), "%s - %s", artist, title);
#endif
					audio::currently_playing.title = std::string(text);
				}
				else
				{
					audio::currently_playing.title = std::string(title);
				}
			}
		}
		else
		{
			meta = BASS_ChannelGetTags(audio::chan, BASS_TAG_HLS_EXTINF);

			if (meta)
			{
				const char *p = strchr(meta, ',');
			}
		}
	}
}

void CALLBACK null_callback(HSYNC handle, DWORD channel, DWORD data, void *user)
{
}

void CALLBACK free_sync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	audio::chan = 0;
}

void open_url(const char* url)
{
	logger::log_info(logger::va("Accessing Stream %s", url));

	std::uint32_t c, r;

	r = ++audio::req;
	if (audio::chan) BASS_StreamFree(audio::chan);
	c = BASS_StreamCreateURL(url, 0, BASS_STREAM_BLOCK | BASS_STREAM_STATUS | BASS_STREAM_AUTOFREE | BASS_SAMPLE_FLOAT, 0, (void*)r); 

	if (r != audio::req)
	{
		if (c) BASS_StreamFree(c);
		return;
	}

	audio::chan = c;
	if (!audio::chan)
	{
		global::msg_box("Radio.Garten Streamer", logger::va("Can't play the stream (ErrorCode %i)", BASS_ErrorGetCode()));
	}
	else
	{
		//Meta data updates
		BASS_ChannelSetSync(audio::chan, BASS_SYNC_META, 0, meta_sync, 0); // Shoutcast
		BASS_ChannelSetSync(audio::chan, BASS_SYNC_OGG_CHANGE, 0, meta_sync, 0);
		BASS_ChannelSetSync(audio::chan, BASS_SYNC_HLS_SEGMENT, 0, meta_sync, 0);

		//Free on end
		BASS_ChannelSetSync(audio::chan, BASS_SYNC_FREE, 0, free_sync, 0);

		//Play channel
		BASS_ChannelPlay(audio::chan, FALSE);

		fs::write (
			fs::get_pref_dir().append("last_played.txt"),
			logger::va("%s,%s", &audio::currently_playing.station.title[0], &audio::currently_playing.station.id[0]),
			false
		);
	}
}