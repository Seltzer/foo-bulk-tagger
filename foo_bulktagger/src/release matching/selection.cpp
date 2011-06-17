#include "selection.h"
#include "matchingHeuristics.h"


#define WIN32
#include "protocol.h"

AudioData* loadWaveFile(char *file);
AudioData* loadDataUsingLAME(char *file);






namespace FBT
{



///////////////// SelectionToMatch implementation
SelectionToMatch::SelectionToMatch()
{
}
	
SelectionToMatch::SelectionToMatch(const metadb_handle_list& tracks)
	: matchAttempted(false)
{
	AddTracks(tracks);
}

void SelectionToMatch::AddTrack(metadb_handle_ptr track)
{
	tracks.add_item(track);

	file_info_impl info;
	track->get_info(info);
	trackInfo += new file_info_impl(info);
}

void SelectionToMatch::AddTracks(const metadb_handle_list& tracksToAdd)
{
	file_info_impl info;

	for(t_size index = 0; index < tracksToAdd.get_count(); index++)
	{
		tracks.add_item(tracksToAdd[index]);
		tracksToAdd[index]->get_info(info);
		trackInfo += new file_info_impl(info);		
	}
	
	tracks.remove_duplicates();
}

const metadb_handle_list& SelectionToMatch::GetTracks()
{
	return tracks;
}


void SelectionToMatch::BuildMetaTable()
{
	unsigned trackIndex = 0;

	for (t_size t = 0; t < tracks.get_count(); t++)
		metaTableBuilder.ExtractMetadata(tracks[t], *(trackInfo[t]) );
}

const SelectionMetaTableBuilder& SelectionToMatch::GetMetaTableBuilder() const
{
	return metaTableBuilder;
}

void SelectionToMatch::FindMatches(MatchingHeuristic& heuristic)
{
	potentialMatches = heuristic.FindMatches(*this);
	
	matchAttempted = true;
}

void SelectionToMatch::WriteTags(pfc::chain_list_v2_t<ExtendedTagInfo*>& addedTags, HWND dlgHandle)
{
	metaTableBuilder.WriteTags(tracks, trackInfo, addedTags, dlgHandle);
}


void GetPrint(AudioData* data)
{
	console::printf("creating print");
	// Get the fingerprint
	if (!data->createPrint()) 
	{
		console::printf("** Failed to generate print.\n");
		return;
	}
	console::printf("finished creating print");
	

	// Get the metadata
	TrackInformation *info = data->getMetadata("a7f6063296c0f1c9b75c7f511861b89b", "1.0", true);
	if (!info) 
	{
		console::printf("** Failed to get metadata.\n");
		return;
	}

	// Print results.
	console::printf(" Title: %s\n", info->getTrack().c_str());
	console::printf("Artist: %s\n", info->getArtist().c_str());
	console::printf("  PUID: %s\n", info->getPUID().c_str());
	
}


void ProcessSong(char* pathToSong)
{
	AudioData* data = NULL;
	
	service_ptr_t<input_entry> tt;
	bool success = input_entry::g_find_service_by_path(tt, pathToSong);
	pfc::dynamic_assert(success);

	service_ptr_t<input_decoder> decoder;
	abort_callback_dummy abortCallback;
	tt->open_for_decoding(decoder, NULL, pathToSong, abortCallback);


	// Track to subsongs
	pfc::dynamic_assert(decoder->get_subsong_count() == 1);
	decoder->initialize(0, input_flag_no_seeking, abortCallback);
	console::print("Decoder initialised - about to start");

	unsigned char* byteArray = new unsigned char[30000000];
	unsigned bytesWritten = 0;

	double duration = 0.00f;

	
	// Subsong to audio chunks (and mem_block_container_impl)
	audio_chunk_impl currentChunk;
	mem_block_container_impl currentBlock;
	
	audio_sample* samplesArray = NULL;
	

	unsigned sampleRate;

	while(decoder->run(currentChunk, abortCallback))
	{
		console::print("\tDecoded chunk");
	
		currentBlock.reset();
		int initialSize = currentChunk.get_sample_count() * 2 * 2;
		currentBlock.set_size(initialSize);
		console::printf("\t\tinitial size = %d", initialSize);
		
		currentChunk.to_raw_data(currentBlock, 16);
		console::printf("\t\tto_raw_data");
		sampleRate = currentChunk.get_sample_rate();
		console::printf("\t\tsample rate = %d", sampleRate);
		duration += currentChunk.get_duration();


		console::printf("\t\tcurrentBlock.get_size() = %d", currentBlock.get_size());
		memcpy(byteArray + bytesWritten, currentBlock.get_ptr(), currentBlock.get_size());
		bytesWritten += currentBlock.get_size();


		console::printf("\t\tbytesWritten = %d", bytesWritten);

		// Audio chunk to samples (floats)
		samplesArray = currentChunk.get_data();
	

		//console::printf("data length = %d", currentChunk.get_data_length());
		//console::printf("samples count = %d", currentChunk.get_sample_count());
		console::print("\tFinished decoding");

	}
	console::print("finished");
	
	
	console::printf("Duration = %d", duration);
	data = new AudioData();
	console::printf("bytesWritten / 2 = %d", bytesWritten / 2);
	data->setData(byteArray, OFA_LITTLE_ENDIAN, bytesWritten / 2, sampleRate, true, duration * 1000, ".mp3");
	
	
	GetPrint(data);
	delete data;

}



void SelectionToMatch::DoPuidStuff()
{
	char* path = const_cast<char*>(tracks[0]->get_path());
	ProcessSong(path + 7);
}


















}