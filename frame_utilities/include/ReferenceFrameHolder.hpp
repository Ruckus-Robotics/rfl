#ifndef REFERENCE_FRAME_HOLDER_HPP
#define REFERENCE_FRAME_HOLDER_HPP

#include <ReferenceFrame.hpp>

/** This class and its implementation are an adaptation
**  of the ReferenceFrameHolder.java by Jerry Pratt and the IHMC robotics group.
**  All credit goes to them.
**/

namespace frame_utilities
{

class ReferenceFrameHolder
{
	public:
		virtual ReferenceFrame* getReferenceFrame() = 0;

		void checkReferenceFramesMatch(ReferenceFrame* referenceFrame)
		{
			getReferenceFrame()->checkReferenceFramesMatch(referenceFrame);
		}

		void checkReferenceFramesMatch(ReferenceFrameHolder* referenceFrameHolder)
		{
			checkReferenceFramesMatch(referenceFrameHolder->getReferenceFrame());
		}
};

}

#endif