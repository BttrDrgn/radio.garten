/*
	BASS internet radio example
	Copyright (c) 2002-2021 Un4seen Developments Ltd.
*/

// HLS definitions (copied from BASSHLS.H)
#define BASS_SYNC_HLS_SEGMENT	0x10300
#define BASS_TAG_HLS_EXTINF		0x14000

HWND win;
DWORD req;	// request number/counter
HSTREAM chan;	// stream handle

// display error messages
void Error(const char *es)
{
	char mes[200];
	sprintf(mes, "%s\n(error code: %d)", es, BASS_ErrorGetCode());
	MessageBoxA(win, mes, 0, 0);
}

#define MESS(id,m,w,l) SendDlgItemMessage(win,id,m,(WPARAM)(w),(LPARAM)(l))

// update stream title from metadata
void DoMeta()
{
	const char *meta = BASS_ChannelGetTags(chan, BASS_TAG_META);
	if (meta) { // got Shoutcast metadata
		const char *p = strstr(meta, "StreamTitle='"); // locate the title
		if (p) {
			const char *p2 = strstr(p, "';"); // locate the end of it
			if (p2) {
				char *t = strdup(p + 13);
				t[p2 - (p + 13)] = 0;
				MESS(30, WM_SETTEXT, 0, t);
				free(t);
			}
		}
	} else {
		meta = BASS_ChannelGetTags(chan, BASS_TAG_OGG);
		if (meta) { // got Icecast/OGG tags
			const char *artist = NULL, *title = NULL, *p = meta;
			for (; *p; p += strlen(p) + 1) {
				if (!strnicmp(p, "artist=", 7)) // found the artist
					artist = p + 7;
				if (!strnicmp(p, "title=", 6)) // found the title
					title = p + 6;
			}
			if (title) {
				if (artist) {
					char text[100];
					_snprintf(text, sizeof(text), "%s - %s", artist, title);
					MESS(30, WM_SETTEXT, 0, text);
				} else
					MESS(30, WM_SETTEXT, 0, title);
			}
		} else {
			meta = BASS_ChannelGetTags(chan, BASS_TAG_HLS_EXTINF);
			if (meta) { // got HLS segment info
				const char *p = strchr(meta, ',');
				if (p) MESS(30, WM_SETTEXT, 0, p + 1);
			}
		}
	}
}

void CALLBACK MetaSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	DoMeta();
}

void CALLBACK StallSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	if (!data) // stalled
		SetTimer(win, 0, 50, 0); // start buffer monitoring
}

void CALLBACK FreeSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	chan = 0;
	MESS(31, WM_SETTEXT, 0, "not playing");
	MESS(30, WM_SETTEXT, 0, "");
	MESS(32, WM_SETTEXT, 0, "");
}

void CALLBACK StatusProc(const void *buffer, DWORD length, void *user)
{
	if (buffer && !length && (DWORD)user == req) // got HTTP/ICY tags, and this is still the current request
		MESS(32, WM_SETTEXT, 0, buffer); // display status
}

DWORD WINAPI OpenURL(const char* url)
{
	DWORD c, r;
	r = ++req; // increment the request counter for this request
	if (chan) BASS_StreamFree(chan); // close old stream
	MESS(31, WM_SETTEXT, 0, "connecting...");
	MESS(30, WM_SETTEXT, 0, "");
	MESS(32, WM_SETTEXT, 0, "");
	c = BASS_StreamCreateURL(url, 0, BASS_STREAM_BLOCK | BASS_STREAM_STATUS | BASS_STREAM_AUTOFREE | BASS_SAMPLE_FLOAT, StatusProc, (void*)r); // open URL
	if (r != req) { // there is a newer request, discard this stream
		if (c) BASS_StreamFree(c);
		return 0;
	}
	chan = c; // this is now the current stream
	if (!chan) { // failed to open
		MESS(31, WM_SETTEXT, 0, "not playing");
		Error("Can't play the stream");
	} else {
		// set syncs for stream title updates
		BASS_ChannelSetSync(chan, BASS_SYNC_META, 0, MetaSync, 0); // Shoutcast
		BASS_ChannelSetSync(chan, BASS_SYNC_OGG_CHANGE, 0, MetaSync, 0); // Icecast/OGG
		BASS_ChannelSetSync(chan, BASS_SYNC_HLS_SEGMENT, 0, MetaSync, 0); // HLS
		// set sync for stalling/buffering
		BASS_ChannelSetSync(chan, BASS_SYNC_STALL, 0, StallSync, 0);
		// set sync for end of stream (when freed due to AUTOFREE)
		BASS_ChannelSetSync(chan, BASS_SYNC_FREE, 0, FreeSync, 0);
		// play it!
		BASS_ChannelPlay(chan, FALSE);
		// start buffer monitoring (and display stream info when done)
		SetTimer(win, 0, 50, 0);
	}
	return 0;
}