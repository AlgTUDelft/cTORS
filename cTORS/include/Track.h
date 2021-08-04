/** \file Track.h
 * This file defines the Track class and the TrackPartType enum
 */
#pragma once
#ifndef TRACK_H
#define TRACK_H
#include "Exceptions.h"
#include "Utils.h"

using namespace std;

/**
 * TrackPartType describes the type of a track part.
 * 
 * From a graph perspective, the Railroad tracks are 'nodes', the other types (except Building) represent connections.
 */
enum class TrackPartType { 
	Railroad,			/**< A piece of track with one piece of track on both ends. */
	Switch,				/**< A piece of track with one piece of track on one end and two pieces of
						track on the other end */
	EnglishSwitch,		/**< A piece of track with two pieces of track on both ends. Here, both 
						a_sides tracks are connected to both b_sides tracks, but not a_side to 
						a_side or b_side to b_side. */
	HalfEnglishSwitch,	/**< A piece of track with two pieces of track on both ends.Here, 
						a_sides[0] is connected to both b_sides tracks, while a_sides[1] is only 
						connected to b_sides[1]. */
	InterSection,		/**< A piece of track with two pieces of track on both ends. Here, 
						a_sides[0] connects to b_sides[1] and a_sides[1] connects to b_sides[0]. */
	Bumper,				/**< A piece of track which stops at some point. Here, a_side contains the 
						piece of track and b_side does not exist, as the track doesn't continue. */
	Building			/**< A Railroad with a building on/over it */
};

/**
 * Convert a protobuf track_part_type to a TrackPartType
 */
inline TrackPartType ConvertPBTrackPartType(PBTrackPartType pb_track_part_type) {
	return static_cast<TrackPartType>(pb_track_part_type);
}

class Facility;

/**
 * A Track describes a track part and its connections.
 */
class Track {
private:
	vector<const Track*> aSides; /**< the A-side tracks of this track */
	vector<const Track*> bSides; /**< the B-side tracks of this track */
	/** a map connecting neighboring tracks to reachable tracks if the key track is the previous track */
	unordered_map<const Track*, vector<const Track*>> next;
	vector<const Facility*> facilities; /**< the facilities located at this track */
public:
	const string id; 				/**< The id of this track */
	const TrackPartType type; 		/**< The type of this track */
	const double length; 			/**< The length of thist track */
	const string name; 				/**< The name of this track */
	const bool sawMovementAllowed; 	/**< Whether saw movements are allowed on this track */
	const bool parkingAllowed;		/**< Whether parking is allowed on this track */
	const bool isElectrified;		/**< Whether this track is electrified or not */

	Track() = delete;

	/**
	 * Construct a track from the given parameters
	 */
	Track(const string& id, TrackPartType type, double length, const string& name,
		bool sawMovementAllowed, bool parkingAllowed, bool isElectrified);
	
	/**
	 * Construct a track from the protobuf object
	 */
	Track(const PBTrack& pb_track) : Track(to_string(pb_track.id()), ConvertPBTrackPartType(pb_track.type()), pb_track.length(),
				pb_track.name(), pb_track.sawmovementallowed(), pb_track.parkingallowed(), pb_track.iselectrified()) {}
	
	/**
	 * Get a copy of the track
	 */
	Track(const Track& track);

	/**
	 * Deconstructor for the track
	 */
	~Track() = default;
	
	/**
	 * Assign the neighbors for this track
	 */
	void AssignNeighbors(vector<const Track*> aside, vector<const Track*> bside);
	
	/**
	 * @return true iff track t is part of this track's A-side
	 */
	bool IsASide(const Track* t) const;
	
	/**
	 * @return true iff track t is part of this track's B-side
	 */
	bool IsBSide(const Track* t) const;

	/**
	 * @return true iff track t1 and t2 are both at the same side for this track,
	 * that is, either both at the A-side or both at the B-side.
	 */
	bool IsSameSide(const Track* t1, const Track* t2) const;
	
	/**
	 * @return true iff track t is a neighbor of this track, that is
	 * if it is either at the A-side or B-side of this track.
	 */
	bool IsNeighbor(const Track* t) const;
	
	/**
	 * Get the TrackPartType of this track
	 */
	inline const TrackPartType GetType() const { return type; }
	
	/**
	 * Add a Facility to this track
	 */
	inline void AddFacility(const Facility* f) {
		facilities.push_back(f);
	}

	/**
	 * Get all the facilities that are located at this track
	 */
	inline const vector<const Facility*> &GetFacilities() const {
		return facilities;
	}
	
	/**
	 * Get the next track parts from this track, if the previous track is previous
	 * 
	 * For example:
	 * 1. If the type of this track is Railroad, and the A-side track is given as previous, the B-side track is return
	 * 2. If the type of this track is Intersection and aSides[0] is given, bSides[1] is returned
	 * 3. If the type of this track is Switch and the A-side track is given, both B-side tracks are returned.
	 */
	inline const vector<const Track*> &GetNextTrackParts(const Track* previous) const {
		return next.at(previous);
	}

	/**
	 * Get all the neighbors of this track, both A-side and B-side.
	 */
	inline const vector<const Track*> GetNeighbors() const {
		vector<const Track*> res = aSides;
		if(bSides.size() > 0)
			res.insert(res.end(), bSides.begin(), bSides.end());
		return res;
	}

	/**
	 * Get a neighbor of this track at the opposite side to previous
	 */
	inline const Track* GetOppositeSide(const Track* previous) const { return GetNextTrackParts(previous).front(); }

	/**
	 * Get the A-side tracks
	 */
	inline const vector<const Track*>& GetASideTracks() const { return aSides; }
	
	/**
	 * Get the B-side tracks
	 */
	inline const vector<const Track*>& GetBSideTracks() const { return bSides; }
	
	/**
	 * Get the length of this track
	 */
	inline double GetLength() const { return length; }
	
	/**
	 * Get a string representation of this track
	 */
	inline const string& toString() const { return name; }
	
	/**
	 * Get the id of this track
	 */
	inline const string& GetID() const { return id; }
	
	/**
	 * @return true iff the id of this track equals the id of the other track
	 */
	inline bool operator==(const Track& t) const { return (id == t.id); }
	
	/**
	 * @return true iff the id of this track does not equal the id of the other track
	 */
	inline bool operator!=(const Track& t) const { return !(*this == t); }
};

//!\cond NO_DOC
STREAM_OPERATOR(Track);
//!\endcond

#endif