#include "Track.h"

Track::Track(const string& id, TrackPartType type, double length, const string& name,
	bool sawMovementAllowed, bool parkingAllowed, bool isElectrified) :
	id(id), type(type), length(length),	name(name), sawMovementAllowed(sawMovementAllowed),
	parkingAllowed(parkingAllowed), isElectrified(isElectrified) { }

Track::Track(const Track& track) :
	id(track.id), type(track.type), length(track.length),
	name(track.name), sawMovementAllowed(track.sawMovementAllowed),
	parkingAllowed(track.parkingAllowed), isElectrified(track.isElectrified),
	aSides(track.aSides), bSides(track.bSides), next(track.next) { }

void Track::AssignNeighbors(vector<const Track*> aside, vector<const Track*> bside)
{
	if (type == TrackPartType::Railroad && (aside.size() != 1 || bside.size() != 1))
		throw InvalidLocationException("Track " + toString() + "(" + id + "): Railroad must have one track at each side");
	if (type == TrackPartType::Bumper && (aside.size() != 1 ||  bside.size() != 0))
		throw InvalidLocationException("Track " + toString() + "(" + id + "): Bumper must have one track at at most one side");
	//write error functions for Switch, EnglishSwitch, HalfEnglishSwitch, InterSection, Building
	aSides = aside;
	bSides = bside;
	if (type == TrackPartType::HalfEnglishSwitch) {
		next[aside[0]] = bside;
		next[aside[1]] = vector<const Track*> { bside[1] };
	} else if (type == TrackPartType::InterSection) {
		next[aside[0]] = vector<const Track*>{ bside[1] };
		next[aside[1]] = vector<const Track*>{ bside[0] };
	} else {
		for (const Track* t : aside)
			next[t] = bside;
	}
	if (type == TrackPartType::HalfEnglishSwitch) {
		next[bside[0]] = aside;
		next[bside[1]] = vector<const Track*>{ aside[1] };
	} else if (type == TrackPartType::InterSection) {
		next[bside[0]] = vector<const Track*>{ aside[1] };
		next[bside[1]] = vector<const Track*>{ aside[0] };
	} else {
		for (const Track* t : bside)
			next[t] = aside;
	}
}

bool Track::IsASide(const Track* t) const {
	if(t==nullptr) return false;
	auto it = find(aSides.begin(), aSides.end(), t);
	if(it != aSides.end())
		return true;
	return false; 
}

bool Track::IsBSide(const Track* t) const {
	if(t==nullptr) return false;
	auto it = find(bSides.begin(), bSides.end(), t);
	if(it != bSides.end())
		return true;
	return false; 
}

bool Track::IsSameSide(const Track* t1, const Track* t2) const {
	return (IsASide(t1) && IsASide(t2)) || (IsBSide(t1) && IsBSide(t2));
}

bool Track::IsNeighbor(const Track* t) const {
	return t != nullptr && IsASide(t) || IsBSide(t);
}