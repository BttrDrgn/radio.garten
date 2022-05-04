/*
	BASS internet radio example
	Copyright (c) 2002-2021 Un4seen Developments Ltd.
*/

#include "logger/logger.hpp"
#include "global.hpp"
#include "audio.hpp"


// update stream title from metadata
void CALLBACK meta_sync(HSYNC handle, DWORD channel, DWORD data, void* user)
{
	const char *meta = BASS_ChannelGetTags(audio::chan, BASS_TAG_META);
	LOG_DEBUG("Getting metadata");
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

				LOG_DEBUG("Song: %s | Station: %s | Country: %s | City: %s",
					&audio::currently_playing.title[0], &audio::currently_playing.station.title[0],
					&audio::currently_playing.region.country[0], &audio::currently_playing.region.city[0]);

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
				if (!strnicmp(p, "artist=", 7)) artist = p + 7;
				if (!strnicmp(p, "title=", 6)) title = p + 6;
			}

			if (title)
			{
				if (artist)
				{
					char text[100];
					_snprintf(text, sizeof(text), "%s - %s", artist, title);
					LOG_DEBUG("Text: %s", text);
					audio::currently_playing.title = std::string(text);
				}
				else
				{
					LOG_DEBUG("Title: %s", title);
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
				LOG_DEBUG("Meta: %s", p + 1);
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
	DWORD c, r;

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
		SDL_ShowSimpleMessageBox(0, "Radio.Garten Streamer", "Can't play the stream", global::window);
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
	}
}