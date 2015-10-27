#ifndef REFERENCE_FRAME_HPP
#define REFERENCE_FRAME_HPP

/** This class and its implementation are an adaptation
**  of the ReferenceFrame.java by Jerry Pratt and the IHMC robotics group.
**  All credit goes to them.
**/

#include <memory>
#include <tf/LinearMath/Transform.h>
#include <tf/LinearMath/Quaternion.h>
#include <tf/LinearMath/Vector3.h>
#include <string>
#include <vector>
#include <climits>

namespace frame_utilities
{

class ReferenceFrame
{
	public:
		ReferenceFrame(const std::string &frameName, ReferenceFrame* parentFrame, const tf::Transform &transformToParent, bool isWorldFrame, bool isBodyCenteredFrame);
		ReferenceFrame(const ReferenceFrame &referenceFrameToCopy);
		ReferenceFrame(const std::string &frameName, ReferenceFrame* parentFrame, bool isWorldFrame, bool isBodyCenteredFrame);
		ReferenceFrame(const std::string &frameName, std::unique_ptr<ReferenceFrame> parentframe, const tf::Transform &transformToParent, bool isWorldFrame, bool isBodyCenteredFrame);
		ReferenceFrame(const std::string &frameName, bool isWorldFrame, bool isBodyCenteredFrame);
		ReferenceFrame(const std::string &frameName, ReferenceFrame* parentFrame, const tf::Transform &transfomToParent, bool isBodyCenteredFrame);
		ReferenceFrame() {}
		~ReferenceFrame();

		void update();
		void getTransformToDesiredFrame(tf::Transform &transformToPack, ReferenceFrame* desiredFrame);
		tf::Transform getTransformToDesiredFrame(ReferenceFrame* desiredFrame);
		void verifyFramesHaveSameRoot(ReferenceFrame* desiredFrame);
		void setTransformToParent(const tf::Transform &transformToParent);
		void checkReferenceFramesMatch(ReferenceFrame* referenceFrame);

		tf::Transform getTransformToRoot()
		{
			computeTransform();
			return this->transformToRoot;
		}

		tf::Transform getInverseTransformToRoot()
		{
			return this->inverseTransformToRoot;
		}

		ReferenceFrame* getRootFrame()
		{
			return this->framesStartingWithRootEndingWithThis[0];
		}

		ReferenceFrame* getParentFrame()
		{
			return this->parentFrame;
		}

		std::string getName()
		{
			return this->frameName;
		}

		const std::vector<ReferenceFrame*> getFramesStartingWithRootEndingWithThis()
		{
			return this->framesStartingWithRootEndingWithThis;
		}

		static std::unique_ptr<ReferenceFrame> createAWorldFrame(const std::string &frameName);
		static std::unique_ptr<ReferenceFrame> createARootFrame(const std::string &frameName);
		static ReferenceFrame* getWorldFrame();

		//Super classes are expected to override this method.
		virtual void updateTransformToParent(tf::Transform &transformToParent) {};

		inline tf::Transform getTransformToParent()
		{
			return this->transformToParent;
		}

	protected:

	private:
		static std::vector<ReferenceFrame*> constructVectorOfFramesStartingWithRootEndingWithThis(ReferenceFrame* thisFrame);

		void computeTransform();
		tf::Transform createIdentityTransform();

		static long nextTransformToRootID;
		long transformToRootID = LLONG_MIN;
		static std::unique_ptr<ReferenceFrame> worldFrame;
		std::vector<ReferenceFrame*> framesStartingWithRootEndingWithThis;
		std::string frameName;
		ReferenceFrame *parentFrame;
		tf::Transform transformToParent;
		tf::Transform transformToRoot;
		tf::Transform inverseTransformToRoot;
		bool isWorldFrame;
		bool isBodyCenteredFrame;
};

}

#endif