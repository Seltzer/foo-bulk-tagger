#include "selectionMetadata.h"



namespace FBT
{




	
///////////////// ExtendedTagInfo implementation
void ExtendedTagInfo::GenerateDecoratedValues()
{
	// Generate decorated tag name
	if (isStandardTag)
	{
		decoratedTagName = realTagName;
	}
	else
	{
		decoratedTagName = pfc::string8("<");
		decoratedTagName += realTagName;
		decoratedTagName += ">";
	}


	// Generate decorated tag values
	decoratedTagValues = pfc::string8("");	
		
	if (hasMultipleValuesProperty)
		decoratedTagValues += "<<multiple values>> ";

	bool processedAValue = false;
	for (pfc::chain_list_v2_t<pfc::string8>::iterator valueIt = realTagValues.first(); valueIt.is_valid(); valueIt++)
	{
		if (processedAValue)
			decoratedTagValues += "; ";
		else
			processedAValue = true;

		decoratedTagValues += *valueIt;
	}
}






///////////////// DirtyTagsFileInfoFilter implementation
DirtyTagsFileInfoFilter::DirtyTagsFileInfoFilter(const pfc::chain_list_v2_t<ExtendedTagInfo*>& dirtyTags)
									: dirtyTags(dirtyTags)
{
}

bool DirtyTagsFileInfoFilter::apply_filter(metadb_handle_ptr track, t_filestats p_stats, file_info& info)
{
	for(pfc::chain_list_v2_t<ExtendedTagInfo*>::iterator it = dirtyTags.first(); it.is_valid(); it++)
	{
		ExtendedTagInfo* tagInfo = *it;
		const char* tagName = tagInfo->realTagName.get_ptr();

		if (tagInfo->realTagValues.get_count() == 1)
		{
			info.meta_set(tagName, tagInfo->realTagValues.first()->get_ptr());
		}
		else
		{
			info.meta_remove_field(tagInfo->realTagName.get_ptr());

			for(pfc::chain_list_v2_t<pfc::string8>::iterator it = tagInfo->realTagValues.first(); it.is_valid(); it++)
				info.meta_add(tagName, (*it).get_ptr());
		}
	}

	return true;
}








///////////////// SelectionMetaTableBuilder implementation
SelectionMetaTableBuilder::SelectionMetaTableBuilder()
	: processedATrack(false)
{
}

SelectionMetaTableBuilder::SelectionMetaTableBuilder(const metadb_handle_list& selection)
	: processedATrack(false)
{
	for (t_size i = 0; i < selection.get_count(); i++)
	{
		file_info_impl info;
		selection[i]->get_info(info);
		ExtractMetadata(selection[i], info);
	}
}

void SelectionMetaTableBuilder::ExtractMetadata(const metadb_handle_ptr track, const file_info& trackData)
{
	for(t_size tag = 0; tag < trackData.meta_get_count(); tag++) 
		AddTagValues(trackData, trackData.meta_enum_name(tag));

	if (!processedATrack)
		processedATrack = true;
}

bool SelectionMetaTableBuilder::AddTagAsKey(const pfc::string8& tag)
{
	if (HasTagAsKey(tag))
		return false;

	if (!file_info::g_is_valid_field_name(tag.get_ptr())) 
		return false;
	
	// Add to superclass metatable
	t_entry& entry = content().find_or_add(tag);
	entry.remove_all();
	
	// Build extended tag data
	ExtendedTagInfo tagData;
	tagData.decoratedTagName = tagData.realTagName = tag;
	tagData.index = extendedTagModel.get_count();
	// If we've previously processed tracks, then they obviously didn't have this tag since we're only seeing it now.
	// Therefore, this tag should has the <<multiple values>> property
	tagData.hasMultipleValuesProperty = processedATrack;
	tagData.isDirty = false;
	
	// TODO fix this
	tagData.isStandardTag = true;
	
	// Add tag data to extendedTagModel
	extendedTagModel.insert_last(tagData);
	
	return true;
}


bool SelectionMetaTableBuilder::HasTagAsKey(const pfc::string8& tag) const
{
	console::printf("SelectionMetaTableBuilder::HasTagAsKey with tag key = %s", tag.get_ptr());
	for (pfc::chain_list_v2_t<ExtendedTagInfo>::const_iterator it  = extendedTagModel.first(); it.is_valid(); it++)
	{
		if ((*it).realTagName == tag)
			return true;
	}

	return false;
}

ExtendedTagInfo* SelectionMetaTableBuilder::GetExtendedTagData(const pfc::string8& tag)
{
	for (pfc::chain_list_v2_t<ExtendedTagInfo>::iterator it  = extendedTagModel.first(); it.is_valid(); it++)
	{
		if ((*it).realTagName == tag)
			return &(*it);
	}

	pfc::dynamic_assert(false);
	return NULL;
}

const ExtendedTagInfo* SelectionMetaTableBuilder::GetExtendedTagData(const pfc::string8& tag) const
{
	for (pfc::chain_list_v2_t<ExtendedTagInfo>::const_iterator it  = extendedTagModel.first(); it.is_valid(); it++)
	{
		if ((*it).realTagName == tag)
			return &(*it);
	}

	pfc::dynamic_assert(false);
	return NULL;
}


void SelectionMetaTableBuilder::AddTagValues(const file_info& track, const pfc::string8& tag)
{
	// Grab meta-table values for the specified tag
	t_entry& values = content()[tag];

	if (!HasTagAsKey(tag))
	{
		// This tag is new to us - add it to the meta table
		bool success = AddTagAsKey(tag);
		pfc::dynamic_assert(success);
	}
	else
	{
		ExtendedTagInfo* extTagInfo = GetExtendedTagData(tag);

		// Check whether the new track will induce the <<multiple values>> property for the specified tag
		// We need to compare the meta table's list of values with the track's list of values
		if (!extTagInfo->hasMultipleValuesProperty)
		{
			// Compare based on size of value sets
			if (values.get_count() != track.meta_get_count_by_name(tag.get_ptr()))
			{
				extTagInfo->hasMultipleValuesProperty = true;
			}
			else
			{
				// Same number of tag values, so do an ordered list comparison of tag values
		
				// Iterate over meta table's values for this tag
				unsigned tagIndex = 0;
				for (t_entry::iterator it = values.first(); it.is_valid(); it++, tagIndex++)
				{
					// Check if track's tag value matches for tagIndex
					if (*it != pfc::string8(track.meta_get(tag.get_ptr(), tagIndex)))
						extTagInfo->hasMultipleValuesProperty = true;
				}
			}
		}
	}

	// Check if the track has any new values which the meta table doesn't have.
		
	// Iterate over the track's set of values for this tag
	ExtendedTagInfo* extTagInfo = GetExtendedTagData(tag);

	for (t_size i = 0; i < track.meta_get_count_by_name(tag.get_ptr()); i++)
	{
		pfc::string8 trackTagValue(track.meta_get(tag.get_ptr(), i));
		
		if (!values.have_item(trackTagValue))
		{
			values.insert_last(trackTagValue);
			extTagInfo->realTagValues.insert_last(trackTagValue);
		}
	}

	extTagInfo->GenerateDecoratedValues();
	
}

const meta_table_builder::t_content& SelectionMetaTableBuilder::GetMetaTable() const
{
	return content();
}

void SelectionMetaTableBuilder::UpdatePropertyGrid(CPropertyGridCtrl& grid) const
{
	if (grid.GetColumnCount() == 0)
	{
		grid.InsertColumn(0, _T("Tag Name"), LVCFMT_LEFT, 100, 0);
		grid.InsertColumn(1, _T("Tag Values"), LVCFMT_LEFT, 200, 0);
	}
	else
	{
		while(grid.GetItemCount())
			grid.DeleteItem(0);
	}
		
	// Iterate over tags
	for (pfc::chain_list_v2_t<ExtendedTagInfo>::const_iterator it = extendedTagModel.first(); it.is_valid(); it++)
	{
		// Grab tag's decorated name as LPCTSTR and create a property item for it
		STRING8_TO_LPCTSTR((*it).decoratedTagName, 256, convertedTagName);
		HPROPERTY tagNameProperty = PropCreateSimple(_T("key"), convertedTagName);
		tagNameProperty->SetItemData((LPARAM) &(*it));
		grid.InsertItem((*it).index, tagNameProperty);

		// Grab tag's decorated values as a LPCTSTR and create an inplace editable property item for it
		STRING8_TO_LPCTSTR((*it).decoratedTagValues, 256, convertedTagValues);
		HPROPERTY tagValuesProperty = PropCreateSimple(_T("value"), convertedTagValues);
		tagValuesProperty->SetItemData((LPARAM) &(*it));
		grid.SetSubItem((*it).index, 1, tagValuesProperty);
	}
	
	grid.Invalidate();
}

void SelectionMetaTableBuilder::WriteTags(metadb_handle_list& tracksToTag, 
					   pfc::list_t<const file_info*> trackInfo,
					   pfc::chain_list_v2_t<ExtendedTagInfo*>& addedTags,
					   HWND dlgHandle)
{
	// Find the subset of original tags which are dirty and add them to addedTags
	for(pfc::chain_list_v2_t<ExtendedTagInfo>::iterator it = extendedTagModel.first(); it.is_valid(); it++)
	{
		if ((*it).isDirty)
			addedTags.insert_last(&(*it));
	}

	static_api_ptr_t<metadb_io_v2> metadb_io;

	// TODO add some error-handling... add a callback
	metadb_io->update_info_async(tracksToTag, 
								 new service_impl_t<DirtyTagsFileInfoFilter>(addedTags),
								 dlgHandle, NULL, NULL);
}

//! Obsolete
void SelectionMetaTableBuilder::DebugToConsole() const
{
	console::printf("SelectionMetaTableBuilder Debug Info:\n");
	
	// Iterate over tag names (keys)
	for (t_content::const_iterator it = content().first(); it.is_valid(); it++)
	{
		console::printf("Tag Name = %s\n", (*it).m_key.get_ptr(), (*it).m_value.get_count());
		
		//if (tagHasMultipleValuesProperty[(*it).m_key])
			//console::printf("\t<<multiple values>>");

		// Iterate over values of tag (values)
		for (t_entry::const_iterator valuesIt = (*it).m_value.first(); valuesIt.is_valid(); valuesIt++)
			console::printf("\t%s", (*valuesIt).get_ptr());
		
		console::print("\n");
	}

}

















}