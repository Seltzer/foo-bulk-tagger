/*
 * MusicBrainz -- The Internet music metadatabase
 *
 * Copyright (C) 2006 Lukas Lalinsky
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * 
 */
 
#include <musicbrainz3/metadata.h>

using namespace MusicBrainz;

class Metadata::MetadataPrivate
{
public:
	MetadataPrivate() :
		artist(NULL),
		track(NULL),
		release(NULL),
		releaseGroup(NULL),
		label(NULL)
		{}
	
	Artist *artist;
	Track *track;
	Release *release;
	ReleaseGroup *releaseGroup;
	Label *label;
	UserList userList;
	ArtistResultList artistResults;
	LabelResultList labelResults;
	TrackResultList trackResults;
	ReleaseResultList releaseResults;
	ReleaseGroupResultList releaseGroupResults;
};

Metadata::Metadata()
{
	d = new MetadataPrivate();
}

Metadata::~Metadata()
{
	if (d->artist)
		delete d->artist;
	
	if (d->track)
		delete d->track;
	
	if (d->release)
		delete d->release;

	if (d->releaseGroup)
		delete d->releaseGroup;

	if (d->label)
		delete d->label;
	
	for (UserList::iterator i = d->userList.begin(); i != d->userList.end(); i++) 
		delete *i;
	d->userList.clear();
	
	for (ArtistResultList::iterator i = d->artistResults.begin(); i != d->artistResults.end(); i++) 
		delete *i;
	d->artistResults.clear();
	
	for (LabelResultList::iterator i = d->labelResults.begin(); i != d->labelResults.end(); i++) 
		delete *i;
	d->labelResults.clear();
	
	for (ReleaseResultList::iterator i = d->releaseResults.begin(); i != d->releaseResults.end(); i++) 
		delete *i;
	d->releaseResults.clear();
	
	for (ReleaseGroupResultList::iterator i = d->releaseGroupResults.begin(); i != d->releaseGroupResults.end(); i++) 
		delete *i;
	d->releaseGroupResults.clear();
	
	for (TrackResultList::iterator i = d->trackResults.begin(); i != d->trackResults.end(); i++) 
		delete *i;
	d->trackResults.clear();
	
	delete d;
}

void 
Metadata::setArtist(Artist *value)
{
	if (d->artist)
		delete d->artist;
    d->artist = value;
}

void 
Metadata::setTrack(Track *value)
{
	if (d->track)
		delete d->track;
    d->track = value;
}

void
Metadata::setRelease(Release *value)
{
	if (d->release)
		delete d->release;
    d->release = value;
}

void
Metadata::setReleaseGroup(ReleaseGroup *value)
{
	if (d->releaseGroup)
		delete d->releaseGroup;
    d->releaseGroup = value;
}

Artist * 
Metadata::getArtist(bool remove)
{
	Artist *ret = d->artist;
	if (remove)
		d->artist = NULL;
    return ret;
}

Track * 
Metadata::getTrack(bool remove)
{
	Track *ret = d->track;
	if (remove)
		d->track = NULL;
    return ret;
}

Release * 
Metadata::getRelease(bool remove)
{
	Release *ret = d->release;
	if (remove)
		d->release = NULL;
    return ret;
}

ReleaseGroup *
Metadata::getReleaseGroup(bool remove)
{
	ReleaseGroup *ret = d->releaseGroup;
	if (remove)
		d->releaseGroup = NULL;
    return ret;
}

Label * 
Metadata::getLabel(bool remove)
{
	Label *ret = d->label;
	if (remove)
		d->label = NULL;
    return ret;
}

void 
Metadata::setLabel(Label *value)
{
	if (d->label)
		delete d->label;
    d->label = value;
}

UserList &
Metadata::getUserList()
{
	return d->userList;
}

ArtistResultList &
Metadata::getArtistResults()
{
	return d->artistResults;
}

LabelResultList &
Metadata::getLabelResults()
{
	return d->labelResults;
}

TrackResultList &
Metadata::getTrackResults()
{
	return d->trackResults;
}

ReleaseResultList &
Metadata::getReleaseResults()
{
	return d->releaseResults;
}

ReleaseGroupResultList &
Metadata::getReleaseGroupResults()
{
	return d->releaseGroupResults;
}

UserList 
Metadata::getUserList(bool remove)
{
	UserList list = d->userList;
	d->userList.clear();
	return list;
}

ArtistResultList 
Metadata::getArtistResults(bool remove)
{
	ArtistResultList list = d->artistResults;
	d->artistResults.clear();
	return list;
}

LabelResultList 
Metadata::getLabelResults(bool remove)
{
	LabelResultList list = d->labelResults;
	d->labelResults.clear();
	return list;
}

TrackResultList 
Metadata::getTrackResults(bool remove)
{
	TrackResultList list = d->trackResults;
	d->trackResults.clear();
	return list;
}

ReleaseResultList 
Metadata::getReleaseResults(bool remove)
{
	ReleaseResultList list = d->releaseResults;
	d->releaseResults.clear();
	return list;
}

ReleaseGroupResultList 
Metadata::getReleaseGroupResults(bool remove)
{
	ReleaseGroupResultList list = d->releaseGroupResults;
	d->releaseGroupResults.clear();
	return list;
}
