#ifndef _CONTEXTMENU_ITEM_FBT_H_
#define _CONTEXTMENU_ITEM_FBT_H_


#include "../../SDK/foobar2000.h"

#include <musicbrainz3/webservice.h>
#include <musicbrainz3/query.h>
#include <musicbrainz3/model.h>

#include "common.h"

#include "../plugin.h"
//#include "ui/matchWithReleasesDialog.h"


using namespace MusicBrainz;







// {054DAED3-A724-4a76-8102-8D068DBC0158}
static const GUID guid_FetchTags = 
		{ 0x54daed3, 0xa724, 0x4a76, { 0x81, 0x2, 0x8d, 0x6, 0x8d, 0xbc, 0x1, 0x58 } };

// {C1397CB7-70F5-4607-8859-4A2511A2B7E2}
static const GUID guid_Config = 
	{ 0xc1397cb7, 0x70f5, 0x4607, { 0x88, 0x59, 0x4a, 0x25, 0x11, 0xa2, 0xb7, 0xe2 } };

// {3989D790-50E8-489c-9F65-623103172604}
static const GUID contextmenu_fbt_id = 
		{ 0x3989d790, 0x50e8, 0x489c, { 0x9f, 0x65, 0x62, 0x31, 0x3, 0x17, 0x26, 0x4 } };



static service_factory_single_t<contextmenu_group_popup_impl> 
		fmt_menu(contextmenu_fbt_id, contextmenu_groups::tagging, COMPONENT_NAME_MINIMAL, 0);



class contextmenu_fbt 
	: public contextmenu_item_simple
{


private:
	enum MenuIndex 
	{
		FetchTags,
		Config
	};

public:	

	GUID get_parent() { return contextmenu_fbt_id; }

	unsigned get_num_items() { return 2; }
	
	void get_item_name(unsigned p_index, pfc::string_base& p_out)
	{
		switch (p_index) 
		{
			case FetchTags:
				p_out = "Fetch Tags";
			break;

			case Config:
				p_out = "Config";
			break;
		}
	}

	void get_item_default_path(unsigned p_index, pfc::string_base& p_out) { p_out = COMPONENT_NAME_MINIMAL; }

	GUID get_item_guid(unsigned p_index)
	{
		switch (p_index)	
		{
			case FetchTags:
				return guid_FetchTags;
			case Config:
				return guid_Config;
		}

		return GUID_NULL;
	}

	bool get_item_description(unsigned p_index, pfc::string_base& p_out)
	{
		switch (p_index) 
		{
			case FetchTags:
				p_out = "Fetch tags from MusicBrainz";
			break;
		
			case Config:
				p_out = "Configure Bulk Tagger plugin";
			break;
		}

		return true;
	}

	void context_command(unsigned p_index,const pfc::list_base_const_t<metadb_handle_ptr>& p_data,const GUID& p_caller)
	{
		WHERES_WALLY

		unsigned displayflags;
		pfc::string8 out;
		context_get_display(p_index, p_data, out, displayflags, p_caller);
		if(displayflags & FLAG_GRAYED) return ;
		
			
		console::printf("%d items selected", p_data.get_size());
		

		file_info_impl info;	
		switch (p_index) 
		{
			case FetchTags:
				if (p_data[0]->get_info(info))
				{
					const char* field = info.meta_get("Artist",0);

					if(field != NULL)
						console::printf("Field = %s", field);
					
					Query q;
					ArtistResultList results;

					Plugin::GetInstance()->SpawnMatchWithReleasesDialog();
//					MatchWithReleasesDialog::ShowWindow();
					
					/*
					try 
					{
						// Search for all artists matching the given name. Limit the results
						// to the 5 best matches.
		
						ArtistFilter f = ArtistFilter().name(field).limit(5);
						results = q.getArtists(&f);
					}
					catch (WebServiceError &e) 
					{
					}

					// No error occurred, so display the results of the search. It consists of
					// ArtistResult objects, where each contains an artist.
	
					for (ArtistResultList::iterator i = results.begin(); i != results.end(); i++) 
					{
						ArtistResult *result = *i;
						Artist *artist = result->getArtist();
						console::printf("iteration");
						
						/*
						cout << "Score   : " << result->getScore() << endl;
						cout << "Id      : " << artist->getId() << endl;
						cout << "Name    : " << artist->getName() << endl;
						cout << "SortName: " << artist->getSortName() << endl;
						cout << endl;
					}*/
				}
			break;
		
			case Config:
				//g_discogs.update_tags_dialog =
				//	new update_tags_dialog(IDD_DIALOG_UPDATE_TAGS, core_api::get_main_window(), p_data);
			break;
		}
	}

	bool context_get_display(unsigned p_index, const pfc::list_base_const_t<metadb_handle_ptr>& p_data, 
							pfc::string_base& p_out, unsigned& p_displayflags, const GUID& p_caller) 
	{
		WHERES_WALLY
		
		PFC_ASSERT(p_index >= 0 && p_index < get_num_items());
		get_item_name(p_index,p_out);

		switch (p_index) 
		{
			case FetchTags:
				p_displayflags = 0;
			break;

			case Config:
				p_displayflags = 0;
			break;
		}

		return true;
	}
};

static contextmenu_item_factory_t<contextmenu_fbt> contextmenu_fbt_factory;

#endif // _CONTEXTMENU_ITEM_FBT_H_
