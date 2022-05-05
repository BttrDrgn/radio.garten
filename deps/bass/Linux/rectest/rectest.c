/*
	BASS recording example
	Copyright (c) 2002-2021 Un4seen Developments Ltd.
*/

#include <gtk/gtk.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <regex.h>
#include "bass.h"

#define UIFILE "rectest.ui"
GtkBuilder *builder;

GtkWidget *win;
GtkWidget *filesel; // file selector

#define BUFSTEP 1000000	// memory allocation unit

int input;				// current input source
int freq, chans;		// sample format
BYTE *recbuf;			// recording buffer
DWORD reclen;			// recording length
HRECORD rchan;			// recording channel
HSTREAM pchan;			// playback channel

// display error messages
void Error(const char *es)
{
	GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(win), GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s\n(error code: %d)", es, BASS_ErrorGetCode());
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

#define GetWidget(id) GTK_WIDGET(gtk_builder_get_object(builder,id))

void WindowDestroy(GtkObject *obj, gpointer data)
{
	gtk_main_quit();
}

// buffer the recorded data
BOOL CALLBACK RecordingCallback(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	// increase buffer size if needed
	if ((reclen % BUFSTEP) + length >= BUFSTEP) {
		void *newbuf = realloc(recbuf, ((reclen + length) / BUFSTEP + 1) * BUFSTEP);
		if (!newbuf) {
			rchan = 0;
			free(recbuf);
			recbuf = NULL;
			Error("Out of memory!");
			gtk_button_set_label(GTK_BUTTON(GetWidget("record")), "Record");
			gtk_widget_set_sensitive(GetWidget("format"), TRUE);
			return FALSE; // stop recording
		}
		recbuf = (BYTE*)newbuf;
	}
	// buffer the data
	memcpy(recbuf + reclen, buffer, length);
	reclen += length;
	return TRUE; // continue recording
}

void StartRecording()
{
	WAVEFORMATEX *wf;
	if (recbuf) { // free old recording
		BASS_StreamFree(pchan);
		pchan = 0;
		free(recbuf);
		recbuf = NULL;
		gtk_widget_set_sensitive(GetWidget("play"), FALSE);
		gtk_widget_set_sensitive(GetWidget("save"), FALSE);
	}
	{ // get selected sample format
		int format = gtk_combo_box_get_active(GTK_COMBO_BOX(GetWidget("format")));
		freq = format > 3 ? 22050 : format > 1 ? 44100 : 48000;
		chans = 1 + (format & 1);
	}
	// allocate initial buffer and make space for WAVE header
	recbuf = (BYTE*)malloc(BUFSTEP);
	reclen = 44;
	// fill the WAVE header
	memcpy(recbuf, "RIFF\0\0\0\0WAVEfmt \20\0\0\0", 20);
	memcpy(recbuf + 36, "data\0\0\0\0", 8);
	wf = (WAVEFORMATEX*)(recbuf + 20);
	wf->wFormatTag = 1;
	wf->nChannels = chans;
	wf->wBitsPerSample = 16;
	wf->nSamplesPerSec = freq;
	wf->nBlockAlign = wf->nChannels * wf->wBitsPerSample / 8;
	wf->nAvgBytesPerSec = wf->nSamplesPerSec * wf->nBlockAlign;
	// start recording
	rchan = BASS_RecordStart(freq, chans, 0, RecordingCallback, NULL);
	if (!rchan) {
		Error("Can't start recording");
		free(recbuf);
		recbuf = 0;
		return;
	}
	gtk_button_set_label(GTK_BUTTON(GetWidget("record")), "Stop");
	gtk_widget_set_sensitive(GetWidget("format"), FALSE);
}

void StopRecording()
{
	BASS_ChannelStop(rchan);
	rchan = 0;
	// complete the WAVE header
	*(DWORD*)(recbuf + 4) = reclen - 8;
	*(DWORD*)(recbuf + 40) = reclen - 44;
	// enable "save" button
	gtk_widget_set_sensitive(GetWidget("save"), TRUE);
	// create a stream from the recording
	if (pchan = BASS_StreamCreateFile(TRUE, recbuf, 0, reclen, 0))
		gtk_widget_set_sensitive(GetWidget("play"), TRUE); // enable "play" button
	gtk_button_set_label(GTK_BUTTON(GetWidget("record")), "Record");
	gtk_widget_set_sensitive(GetWidget("format"), TRUE);
}

gboolean FileExtensionFilter(const GtkFileFilterInfo *info, gpointer data)
{
	return !regexec((regex_t*)data, info->filename, 0, NULL, 0);
}

void WriteToDisk()
{
	int resp = gtk_dialog_run(GTK_DIALOG(filesel));
	gtk_widget_hide(filesel);
	if (resp == GTK_RESPONSE_ACCEPT) {
		char *file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filesel));
		FILE *fp = fopen(file, "wb");
		if (fp) {
			fwrite(recbuf, reclen, 1, fp);
			fclose(fp);
		} else
			Error("Can't create the file");
		g_free(file);
	}
}

void UpdateInputInfo()
{
	float level;
	int r = BASS_RecordGetInput(input, &level); // get info on the input
	if (r == -1 || level < 0) { // failed
		r = BASS_RecordGetInput(-1, &level); // try master input instead
		if (r == -1 || level < 0) level = 1; // that failed too, just display 100%
	}
	gtk_range_set_value(GTK_RANGE(GetWidget("volume")), level * 100); // set the level slider
}

void RecordClicked(GtkButton *obj, gpointer data)
{
	if (!rchan)
		StartRecording();
	else
		StopRecording();
}

void PlayClicked(GtkButton *obj, gpointer data)
{
	BASS_ChannelPlay(pchan, TRUE); // play the recorded data
}

void SaveClicked(GtkButton *obj, gpointer data)
{
	WriteToDisk();
}

void InputChanged(GtkComboBox *obj, gpointer data)
{
	int i;
	input = gtk_combo_box_get_active(obj); // get the selection
	// enable the selected input
	for (i = 0; BASS_RecordSetInput(i, BASS_INPUT_OFF, -1); i++); // 1st disable all inputs, then...
	BASS_RecordSetInput(input, BASS_INPUT_ON, -1); // enable the selected
	UpdateInputInfo(); // update info
}

void LevelChanged(GtkRange *range, gpointer data)
{
	double level = gtk_range_get_value(range) / 100;
	if (!BASS_RecordSetInput(input, 0, level)) // failed to set input level
		BASS_RecordSetInput(-1, 0, level); // try master level instead
}

gboolean TimerProc(gpointer data)
{
	float level = 0;
	char text[30] = "";
	// update the level display
	if (rchan || pchan) {
		BASS_ChannelGetLevelEx(rchan ? rchan : pchan, &level, 0.1, BASS_LEVEL_MONO); // get current level
		if (level > 0) {
			level = 1 + log10(level) * 20 / 30; // convert to dB (30dB range)
			if (level < 0) level = 0;
		}
	}
	gtk_progress_set_percentage(GTK_PROGRESS(GetWidget("level")), level);
	// update the recording/playback counter
	if (rchan) { // recording
		if (!BASS_ChannelIsActive(rchan)) { // the recording has stopped, eg. unplugged device
			StopRecording();
			Error("The recording stopped");
			return TRUE;
		}
		sprintf(text, "%d", reclen - 44);
	} else if (pchan) {
		if (BASS_ChannelIsActive(pchan)) // playing
			sprintf(text, "%lld / %lld", BASS_ChannelGetPosition(pchan, BASS_POS_BYTE), BASS_ChannelGetLength(pchan, BASS_POS_BYTE));
		else
			sprintf(text, "%lld", BASS_ChannelGetLength(pchan, BASS_POS_BYTE));
	}
	gtk_label_set(GTK_LABEL(GetWidget("status")), text);
	return TRUE;
}

int main(int argc, char* argv[])
{
	regex_t fregex;

	gtk_init(&argc, &argv);

	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
		Error("An incorrect version of BASS was loaded");
		return 0;
	}

	// initalize default recording device
	if (!BASS_RecordInit(-1)) {
		Error("Can't initialize recording device");
		return 0;
	}
	// initialize default output device
	if (!BASS_Init(-1, 48000, 0, NULL, NULL))
		Error("Can't initialize output device");

	// initialize GUI
	builder = gtk_builder_new();
	if (!gtk_builder_add_from_file(builder, UIFILE, NULL)) {
		char path[PATH_MAX];
		readlink("/proc/self/exe", path, sizeof(path));
		strcpy(strrchr(path, '/') + 1, UIFILE);
		if (!gtk_builder_add_from_file(builder, path, NULL)) {
			Error("Can't load UI");
			return 0;
		}
	}
	win = GetWidget("window1");
	gtk_builder_connect_signals(builder, NULL);

	{ // get list of inputs
		int c;
		const char *name;
		GtkComboBox *list = GTK_COMBO_BOX(GetWidget("input"));
		for (c = 0; name = BASS_RecordGetInputName(c); c++) {
			gtk_combo_box_append_text(list, name);
			if (!(BASS_RecordGetInput(c, NULL) & BASS_INPUT_OFF)) { // this 1 is currently "on"
				input = c;
				gtk_combo_box_set_active(list, input);
			}
		}
		if (!c && (name = BASS_RecordGetInputName(-1))) {
			gtk_combo_box_append_text(list, name);
			gtk_combo_box_set_active(list, 0);
			gtk_widget_set_sensitive(GTK_WIDGET(list), FALSE);
		}
		UpdateInputInfo(); // display info

		list = GTK_COMBO_BOX(GetWidget("format"));
		gtk_combo_box_append_text(list, "48000 Hz mono 16-bit");
		gtk_combo_box_append_text(list, "48000 Hz stereo 16-bit");
		gtk_combo_box_append_text(list, "44100 Hz mono 16-bit");
		gtk_combo_box_append_text(list, "44100 Hz stereo 16-bit");
		gtk_combo_box_append_text(list, "22050 Hz mono 16-bit");
		gtk_combo_box_append_text(list, "22050 Hz stereo 16-bit");
		gtk_combo_box_set_active(list, 3);
	}

	{ // initialize file selector
		GtkFileFilter *filter;
		filesel = gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(win), GTK_FILE_CHOOSER_ACTION_SAVE,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
		gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(filesel), TRUE);
		filter = gtk_file_filter_new();
		gtk_file_filter_set_name(filter, "WAV files");
		regcomp(&fregex, "\\.wav$", REG_ICASE | REG_NOSUB | REG_EXTENDED);
		gtk_file_filter_add_custom(filter, GTK_FILE_FILTER_FILENAME, FileExtensionFilter, &fregex, NULL);
		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filesel), filter);
		filter = gtk_file_filter_new();
		gtk_file_filter_set_name(filter, "All files");
		gtk_file_filter_add_pattern(filter, "*");
		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filesel), filter);
	}

	g_timeout_add(100, TimerProc, NULL); // timer to update the level and position displays

	gtk_main();

	gtk_widget_destroy(filesel);
	regfree(&fregex);

	// release all BASS stuff
	BASS_RecordFree();
	BASS_Free();

	return 0;
}
