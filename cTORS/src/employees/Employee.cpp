#include <math.h>
#include "Employee.h"
using namespace std;

inline TimeShift ConvertPBTimeInterval(const PBTimeInterval& pb_time_interval) {
	return {(int) round(pb_time_interval.start()), (int) round(pb_time_interval.end())};
}

vector<TimeShift> ConvertPBTimeIntervals(const PBList<PBTimeInterval>& pb_time_intervals) {
	vector<TimeShift> out(pb_time_intervals.size());
	for(int i=0; i<pb_time_intervals.size(); i++)
		out[i] = ConvertPBTimeInterval(pb_time_intervals[i]);
	return out;
}

Employee::Employee(const PBMemberOfStaff& pb_employee) : Employee(to_string(pb_employee.id()), pb_employee.name(), pb_employee.type(), 
		vector<string>(pb_employee.skills().begin(), pb_employee.skills().end()), ConvertPBTimeIntervals(pb_employee.shifts()),
		ConvertPBTimeIntervals(pb_employee.breakwindows()), pb_employee.breakduration(), pb_employee.canmovetrains()) {}

Employee::~Employee() {}
