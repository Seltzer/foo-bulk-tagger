#ifndef SELECTION_METADATA_H_
#define SELECTION_METADATA_H_

#include "../common.h"

#include <atlbase.h>
#include <atlapp.h>
#include <atlcrack.h>
#include <atlctrls.h>
#include "wtl_widgets\PropertyGrid.h"

class CPropertyGridCtrl;


namespace FBT
{


	struct ExtendedTagInfo
	{
		// Index of tag (affects order of tags in view)
		unsigned index;
		bool isStandardTag;
		bool isDirty;

		pfc::string8 realTagName, decoratedTagName;

		pfc::chain_list_v2_t<pfc::string8> realTagValues;
		pfc::string8 decoratedTagValues;
		bool hasMultipleValuesProperty;	

		void GenerateDecoratedValues();
	};

	

	// TODO Consider integrating into SelectionMetaTableBuilder... strategy pattern maybe?
	class DirtyTagsFileInfoFilter : public file_info_filter 
	{
	
	public:
		DirtyTagsFileInfoFilter(const pfc::chain_list_v2_t<ExtendedTagInfo*>& dirtyTags);
		virtual bool apply_filter(metadb_handle_ptr p_location, t_filestats p_stats, file_info& p_info);

	private:
		pfc::chain_list_v2_t<ExtendedTagInfo*> dirtyTags;
	};



	// For building up a table of metadata representing a selection (multiple tracks)
	class SelectionMetaTableBuilder : private meta_table_builder
	{

	public:
		SelectionMetaTableBuilder();
		SelectionMetaTableBuilder(const metadb_handle_list& selection);

		void ExtractMetadata(const metadb_handle_ptr, const file_info&);
		
	
		const meta_table_builder::t_content& GetMetaTable() const;
		
		// Update a CPropertyGridCtrl based on the metatable
		void UpdatePropertyGrid(CPropertyGridCtrl&) const;

		// This writes any dirty data for existing tags as well as the tags specified by addedTags to tracks
		void WriteTags(metadb_handle_list& tracksToTag, 
					   pfc::list_t<const file_info*> trackInfo,
					   pfc::chain_list_v2_t<ExtendedTagInfo*>& addedTags,
					   HWND dlgHandle);

		// TODO
		bool HasTagAsKey(const pfc::string8& tag) const;
		ExtendedTagInfo* GetExtendedTagData(const pfc::string8& tag);
		const ExtendedTagInfo* GetExtendedTagData(const pfc::string8& tag) const;


		//! Obsolete
		void DebugToConsole() const;
		
	private:
		// Pre-Conditions: 
		//		- Tag doesn't already exist as a key
		//		- Tag is valid
		// Returns true if added successfully, false otherwise.
		bool AddTagAsKey(const pfc::string8& tag);
		void AddTagValues(const file_info& track, const pfc::string8& tag);
		pfc::string8 GetDecoratedTagValues(const pfc::string8& tag) const;
				
		// An ExtendedTagInfo for each tag
		pfc::chain_list_v2_t<ExtendedTagInfo> extendedTagModel;
		bool processedATrack;
	};





}








#endif