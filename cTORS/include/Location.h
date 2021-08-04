/** \file Location.h
 * This file describes the Location class and the Path struct 
 */
#pragma once
#ifndef LOCATION_H
#define LOCATION_H
#include "Track.h"
#include "Facility.h"
using namespace std;
#define MAX_PATH_LENGTH INT32_MAX /**< The initial maximum path length */

/**
 * A Path describes a route through the shunting yard.
 * 
 * Note that the length of the path may depend on a Train's type
 */
struct Path {
	list<const Track*> route;	/**< All the tracks that are part of this path, ordered from start position to end position */
	int length;					/**< The length of this route in seconds, possibly different for different train types. */
	
	/** Initialize an empty path with infinite length */
	Path() : length(MAX_PATH_LENGTH) {}
	/** Initialize a path given the parameters */
	Path(list<const Track*> route, int length) : route(route), length(length) {}
	/** Get a string representation of this path */
	string toString() const;
	/** Get the starting track of this path */
	inline const Track* GetStart() const { return route.front(); }
	/** Get the destination track of this path */
	inline const Track* GetDestination() const { return route.back(); }
	/** Get the number of Track%s in this Path */
	inline int GetNumberOfTracks() const { return route.size(); }
	/** Append a path to the Path */
	void Append(const Path& other);
};

/** A Position is a tuple of (Track* previous, Track* current) */
typedef pair<const Track*, const Track*> Position;

/**
 * A Location describes a shunting yard.
 * 
 * It contains a list of Track%s and Facilities.
 * It also has helper functions to find (shortest) Path%s and distances
 */
class Location
{
private:
	static const string locationFileString;
	
	string path;
	vector<Track*> tracks;
	vector<Facility*> facilities;
	unordered_map<Position, double> distanceMatrix;
	/** A shortest path map for each setbackTime. The map is from (start_position, end_position) -> path */
	map<int, unordered_map<pair<Position, Position>, Path>> shortestPath;
	/** Path from a Railroad position to all neighboring RailRoad positions */
	unordered_map<Position, unordered_map<Position, Path>> neighborPaths;
	/** A list of all possible paths. The map is from (start_position, end_position), without setbacks */
	unordered_map<pair<Position, Position>, vector<Path>> possiblePaths;
	/** A list of all possible paths starting from a Position, without setbacks */
	unordered_map<Position, vector<Path>> possibleMovements;
	/** All the tracks indexed by their string id */
	map<string, Track*> trackIndex;
	int movementConstant;
	map<const TrackPartType, int> moveDuration;
	bool byType;
	
	void ImportTracks(const PBLocation& pb_location);
	void ImportFacilities(const PBLocation& pb_location);
	void ImportDistanceMatrix(const PBLocation& pb_location);
public:
	Location() = delete;
	/** Construct a Location from a protobuf file 
	 * 
	 * The parameter byType determines if distances are calculated by TrackPartType or by using the distance matrix
	 */
	Location(const string &path, bool byType);
	/** Default copy constructor */
	Location(const Location& location) = default;
	/** Destruct this location */
	~Location();
	
	/** Get the path of the protobuf file */
	inline const string& GetLocationFilePath() const { return path; }
	/** Get a reference to the Track by its id */
	inline Track* GetTrackByID(const string& id) const { return trackIndex.at(id); }
	/** Get all the Track%s */
	inline const vector<Track*>& GetTracks() const { return tracks; }
	/** Get all the facilities */
	inline const vector<Facility*>& GetFacilities() const { return facilities; }
	/** Get a reference to a Facility by its id */
	const Facility* GetFacilityByID(int id) const;

	/**
	 * Get the distance from one Track to another based on the provided distance matrix
	 * 
	 * Note that this distance is only used when configured appropriatly
	 */
	inline double GetDistance(const Track* from, const Track* to) const { return distanceMatrix.at({from, to}); }
	
	/**
	 * Get the duration for crossing the given track based on its type (Railroad or Switch)
	 */
	inline int GetDurationByType(const Track* track) const { 
		return (track->GetType() == TrackPartType::Railroad && track->GetLength() == 0) ? 0  : moveDuration.at(track->GetType()); }
	
	/**
	 * Get the duration for the route described by the Track%s
	 */
	int GetDistance(const list<const Track*>& tracks) const;

	/**
	 * Get the duration for the route described by the Track%s
	 */
	int GetDistance(const vector<const Track*>& tracks) const;

	/**
	 * Calculate all the neighboring paths. 
	 * 
	 * Calling this method once is required if you want to use GetNeighborPath.
	 */
	void CalcNeighboringPaths();
	
	/**
	 * Calculate all the shortest paths. 
	 * 
	 * Calling this method once (per TrainUnitType) is required if you want to use GetShortestPath.
	 * The parameter type is used because different train types have different Setback times.
	 */
	void CalcShortestPaths(const TrainUnitType* type);
	
	/**
	 * Calculate all the possible paths. 
	 * 
	 * Calling this method once is required if you want to use GetPossiblePaths.
	 */
	void CalcAllPossiblePaths();

	/**
	 * Get the shortest path from a certain position to a destination.
	 * 
	 * Both from and to are Position%s, a tuple describing the ShuntingUnit's current
	 * position and its previous position. The current position must always be a track of type Railroad
	 * Call CalcShortestPaths (once) to calculate all the shortest paths
	 */
	inline Path GetShortestPath(const TrainUnitType* type, const Position& from, const Position& to) const {
		return shortestPath.at(type->setbackTime).at({from, to}); }
	
	/**
	 * Get all the neighboring paths from a certain position
	 * 
	 * A neighboring path is a path to a neighboring Railroad track. Every track has immediate neighbors
	 * as defined by the A-side and B-side attributes. This method returns all (non-immediate) Railroad 
	 * neighbors that can be reached by traversing zero or more non-Railroad tracks.
	 * 
	 * Call CalcNeighboringPaths (once) to calculate all the neighboring paths
	 */
	inline const unordered_map<Position,Path>& GetNeighboringPaths(const Position& from) const { return neighborPaths.at(from); }
	
	/**
	 * Get the neighboring path from a certain position to a certain destination
	 * 
	 * A neighboring path is a path to a neighboring Railroad track. Every track has immediate neighbors
	 * as defined by the A-side and B-side attributes. This method returns a path to a (non-immediate) Railroad 
	 * neighbor that can be reached by traversing zero or more non-Railroad tracks.
	 * 
	 * Call CalcNeighboringPaths (once) to calculate all the neighboring paths
	 */
	const Path& GetNeighborPath(const Position& from, const Track* destination) const;

	/**
	 * Get all the possible Path%s from Position from to Position to (without setbacks)
	 * 
	 * Call CalcAllPossiblePaths (once) to callculate all possible paths
	 */
	inline const vector<Path>& GetPossiblePaths(const Position& from, const Position& to) const { return possiblePaths.at({from, to}); }

	/**
	 * Get all the possible Path%s from Position from to any other Position (without setbacks)
	 * 
	 * Call CalcAllPossiblePaths (once) to callculate all possible paths
	 */
	inline const vector<Path>& GetPossiblePaths(const Position& from) const { return possibleMovements.at(from); }
};

#endif