#include "selection.h"
#include "matchingHeuristics.h"







namespace FBT
{




///////////////// SelectionMetaTableBuilder implementation
SelectionMetaTableBuilder::SelectionMetaTableBuilder()
	: processedATrack(false)
{
}

SelectionMetaTableBuilder::SelectionMetaTableBuilder(const metadb_handle_list& selection)
	: processedATrack(false)
{
	for (t_size i = 0; i < selection.get_count(); i++)
		AddTrackToSelection(selection[i]);
}

void SelectionMetaTableBuilder::AddTrackToSelection(const metadb_handle_ptr track)
{
	file_info_impl info;
	track->get_info(info);
	
	for(t_size tag = 0; tag < info.meta_get_count(); tag++) 
		AddTagValues(info, info.meta_enum_name(tag));

	if (!processedATrack)
		processedATrack = true;
}

bool SelectionMetaTableBuilder::AddTagAsKey(const pfc::string8& tag)
{
	if (HasTagAsKey(tag))
		return false;

	if (!file_info::g_is_valid_field_name(tag.get_ptr())) 
		return false;
	
	t_entry& entry = content().find_or_add(tag);
	entry.remove_all();
	
	// If we've previously processed tracks, then they obviously didn't have this tag since we're only seeing it now.
	// Therefore, this tag should has the <<multiple values>> property
	tagHasMultipleValuesProperty[tag] = processedATrack;

	return true;
}


bool SelectionMetaTableBuilder::HasTagAsKey(const pfc::string8& tag) const
{
	return tagHasMultipleValuesProperty.have_item(tag);
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
		// Check whether the new track will induce the <<multiple values>> property for the specified tag
		// We need to compare the meta table's list of values with the track's list of values
		if (!tagHasMultipleValuesProperty[tag])
		{
			// Compare based on size of value sets
			if (values.get_count() != track.meta_get_count_by_name(tag.get_ptr()))
			{
				tagHasMultipleValuesProperty[tag] = true;
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
						tagHasMultipleValuesProperty[tag] = true;
				}
			}
		}
	}

	// Check if the track has any new values which the meta table doesn't have.
		
	// Iterate over the track's set of values for this tag
	for (t_size i = 0; i < track.meta_get_count_by_name(tag.get_ptr()); i++)
	{
		pfc::string8 trackTagValue(track.meta_get(tag.get_ptr(), i));
		
		if (!values.have_item(trackTagValue))
			values.insert_last(trackTagValue);
	}
	
}

pfc::string8 SelectionMetaTableBuilder::GetDecoratedTagValues(const pfc::string8& tag) const
{
	pfc::dynamic_assert(tagHasMultipleValuesProperty.have_item(tag));


	pfc::string8 ret;
	
	if (tagHasMultipleValuesProperty[tag])
		ret += "<<multiple values>>";
	
	
	bool processedAValue = false;
	for (t_entry::const_iterator valueIt = content()[tag].first(); valueIt.is_valid(); valueIt++)
	{
		if (processedAValue)
			ret += "; ";
		else
			processedAValue = true;

		ret += *valueIt;
	}
	
	return ret;
}

void SelectionMetaTableBuilder::ClearSelection()
{
	reset();
	tagHasMultipleValuesProperty.remove_all();
	processedATrack = false;
}

bool SelectionMetaTableBuilder::TagHasMultipleValuesProperty(const pfc::string8& tag) const
{
	pfc::dynamic_assert(tagHasMultipleValuesProperty.have_item(tag));

	return tagHasMultipleValuesProperty[tag];
}


const meta_table_builder::t_content& SelectionMetaTableBuilder::GetMetaTable() const
{
	return content();
}

void SelectionMetaTableBuilder::UpdatePropertyGrid(CPropertyGridCtrl& grid) const
{
	/*
	while(grid.DeleteItem(0));
	grid.DeleteColumn(0);
	grid.DeleteColumn(1);*/
		
	grid.InsertColumn(0, _T("Tag Name"), LVCFMT_LEFT, 200, 0);
	grid.InsertColumn(1, _T("Tag Values"), LVCFMT_LEFT, 200, 0);
		
	// Iterate over tag names (keys)
	unsigned rowCount = 0;
	for (t_content::const_iterator it = content().first(); it.is_valid(); it++, rowCount++)
	{
		// Grab tag name as LPCTSTR and create a read-only property item for it
		STRING8_TO_LPCTSTR((*it).m_key, 256, tagName);
		grid.InsertItem(rowCount, PropCreateReadOnlyItem(_T(""), tagName)); 
		
		// Grab tag's decorated values as a LPCTSTR and create an inplace editable property item for it
		pfc::string8 tagValues = this->GetDecoratedTagValues((*it).m_key);
		STRING8_TO_LPCTSTR(tagValues, 256, convertedTagValues);
		grid.SetSubItem(rowCount, 1, PropCreateSimple(_T(""), convertedTagValues));
	}
}


void SelectionMetaTableBuilder::DebugToConsole() const
{
	console::printf("SelectionMetaTableBuilder Debug Info:\n");
	
	// Iterate over tag names (keys)
	for (t_content::const_iterator it = content().first(); it.is_valid(); it++)
	{
		console::printf("Tag Name = %s\n", (*it).m_key.get_ptr(), (*it).m_value.get_count());
		
		if (tagHasMultipleValuesProperty[(*it).m_key])
			console::printf("\t<<multiple values>>");

		// Iterate over values of tag (values)
		for (t_entry::const_iterator valuesIt = (*it).m_value.first(); valuesIt.is_valid(); valuesIt++)
			console::printf("\t%s", (*valuesIt).get_ptr());
		
		console::print("\n");
	}

}







///////////////// SelectionToMatch implementation
SelectionToMatch::SelectionToMatch()
{
}
	
SelectionToMatch::SelectionToMatch(const metadb_handle_list& tracks)
	: tracks(tracks), matchAttempted(false)
{
}

void SelectionToMatch::AddTrack(metadb_handle_ptr track)
{
	tracks.add_item(track);
}

void SelectionToMatch::AddTracks(const metadb_handle_list& tracksToAdd)
{
	tracks += tracksToAdd;
	tracks.remove_duplicates();
}

const metadb_handle_list& SelectionToMatch::GetTracks()
{
	return tracks;
}


void SelectionToMatch::BuildMetaTable()
{
	metaTableBuilder.ClearSelection();

	for (t_size i = 0; i < tracks.get_count(); i++)
		metaTableBuilder.AddTrackToSelection(tracks[i]);
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




















}