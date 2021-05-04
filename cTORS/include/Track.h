#pragma once
#ifndef TRACK_H
#define TRACK_H
#include <vector>
#include <unordered_map>
#include <string>
#include "Exceptions.h"
#include "Utils.h"

using namespace std;

enum class TrackPartType { 
	Railroad,			/* A piece of track with one piece of track on both ends. */
	Switch,				/* A piece of track with one piece of track on one end and two pieces of
						track on the other end */
	EnglishSwitch,		/* A piece of track with two pieces of track on both ends. Here, both 
						a_sides tracks are connected to both b_sides tracks, but not a_side to 
						a_side or b_side to b_side. */
	HalfEnglishSwitch,	/* A piece of track with two pieces of track on both ends.Here, 
						a_sides[0] is connected to both b_sides tracks, while a_sides[1] is only 
						connected to b_sides[1]. */
	InterSection,		/* A piece of track with two pieces of track on both ends. Here, 
						a_sides[0] connects to b_sides[1] and a_sides[1] connects to b_sides[0]. */
	Bumper,				/* A piece of track which stops at some point. Here, a_side contains the 
						piece of track and b_side does not exist, as the track doesn't continue. */
	Building			/* A Railroad with a building on/over it */
};

inline TrackPartType ConvertPBTrackPartType(PBTrackPartType pb_track_part_type) {
	return static_cast<TrackPartType>(pb_track_part_type);
}

class Facility;

class Track {
private:
	vector<const Track*> aSides;
	vector<const Track*> bSides;
	unordered_map<const Track*, vector<const Track*>> next;
	vector<const Facility*> facilities;
public:
	const string id;
	const TrackPartType type;
	const double length;
	const string name;
	const bool sawMovementAllowed;
	const bool parkingAllowed;
	const bool isElectrified;
	const bool standingAllowed;

	Track() = delete;
	Track(const string& id, TrackPartType type, double length, const string& name, bool sawMovementAllowed,
		bool parkingAllowed, bool isElectrified, bool standingAllowed);
	Track(const PBTrack& pb_track) : Track(to_string(pb_track.id()), ConvertPBTrackPartType(pb_track.type()), pb_track.length(),
				pb_track.name(), pb_track.sawmovementallowed(), pb_track.parkingallowed(), pb_track.iselectrified(), pb_track.standingallowed()) {}
	Track(const Track& track);
	~Track();
	void AssignNeighbors(vector<const Track*> aside, vector<const Track*> bside);
	
	bool IsASide(const Track* t) const;
	bool IsBSide(const Track* t) const;
	bool IsSameSide(const Track* t1, const Track* t2) const;
	bool IsNeighbor(const Track* t) const;
	inline const TrackPartType GetType() const { return type; }
	
	inline void AddFacility(const Facility* f) {
		facilities.push_back(f);
	}

	inline const vector<const Facility*> &GetFacilities() const {
		return facilities;
	}
	
	inline const vector<const Track*> &GetNextTrackParts(const Track* previous) const {
		return next.at(previous);
	}

	inline const vector<const Track*> GetNeighbors() const {
		vector<const Track*> res = aSides;
		if(bSides.size() > 0)
			res.insert(res.end(), bSides.begin(), bSides.end());
		return res;
	}

	inline const Track* GetOppositeSide(const Track* previous) const { return GetNextTrackParts(previous).front(); }

	inline double GetLength() const { return length; }
	inline const string& toString() const { return name; }
	inline const string& GetID() const { return id; }
	
	
	inline bool operator==(const Track& t) const { return (id == t.id); }
	inline bool operator!=(const Track& t) const { return !(*this == t); }
};

#endif