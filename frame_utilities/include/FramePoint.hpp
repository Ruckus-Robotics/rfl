#include "FrameTuple.hpp"

namespace frame_utilities
{

class FramePoint : public FrameTuple
{
	public:
		FramePoint();
		FramePoint(const std::string &name, ReferenceFrame* referenceFrame, const double &x, const double &y, const double &z);
		FramePoint(const std::string &name, ReferenceFrame* referenceFrame, double array[3]);
		FramePoint(const std::string &name, ReferenceFrame* referenceFrame, std::vector<double> vector);
		FramePoint(const FramePoint &framePoint);
		FramePoint(const std::string &name, ReferenceFrame* referenceFrame);
};

}