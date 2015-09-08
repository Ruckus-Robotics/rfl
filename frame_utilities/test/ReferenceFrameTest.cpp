#include <gtest/gtest.h>
#include "ReferenceFrame.hpp"
#include "ReferenceFrameTestHelper.hpp"

using namespace frame_utilities;

class ReferenceFrameTest : public ::testing::Test
{
	protected:

		virtual void SetUp()
		{
			allFrames.push_back(root1.get());
			allFrames.push_back(frame1.get());
			allFrames.push_back(frame2.get());
			allFrames.push_back(frame3.get());

			allFrames.push_back(root2.get());
			allFrames.push_back(frame4.get());
			allFrames.push_back(frame5.get());
			allFrames.push_back(frame6.get());
			allFrames.push_back(frame7.get());
			allFrames.push_back(frame8.get());

			frames1.push_back(root1.get());
			frames1.push_back(frame1.get());
			frames1.push_back(frame2.get());
			frames1.push_back(frame3.get());

			frames2.push_back(root2.get());
			frames2.push_back(frame4.get());
			frames2.push_back(frame5.get());
			frames2.push_back(frame6.get());
			frames2.push_back(frame7.get());
			frames2.push_back(frame8.get());
		}
		virtual void TearDown()
		{
			allFrames.clear();
			frames1.clear();
			frames2.clear();
		}

		std::unique_ptr<ReferenceFrame> root1 = ReferenceFrame::createARootFrame("root1");
		std::unique_ptr<ReferenceFrame> root2 = ReferenceFrame::createARootFrame("root2");

		std::shared_ptr<RandomUnchangingFrame> frame1 = RandomUnchangingFrame::create("frame1", root1.get());
		std::shared_ptr<RandomUnchangingFrame> frame2 = RandomUnchangingFrame::create("frame2", frame1.get());
		std::shared_ptr<RandomUnchangingFrame> frame3 = RandomUnchangingFrame::create("frame3", frame2.get());

		std::shared_ptr<RandomlyChangingFrame> frame4 = RandomlyChangingFrame::create("frame4", root2.get());
		std::shared_ptr<RandomlyChangingFrame> frame5 = RandomlyChangingFrame::create("frame5", frame4.get());
		std::shared_ptr<RandomUnchangingFrame> frame6 = RandomUnchangingFrame::create("frame6", root2.get());
		std::shared_ptr<RandomlyChangingFrame> frame7 = RandomlyChangingFrame::create("frame7", frame6.get());
		std::shared_ptr<RandomlyChangingFrame> frame8 = RandomlyChangingFrame::create("frame8", frame7.get());

		std::vector<ReferenceFrame*> allFrames;
		std::vector<ReferenceFrame*> frames1;
		std::vector<ReferenceFrame*> frames2;

		int nTests = 1000;

	private:

};

TEST_F(ReferenceFrameTest, testRootsHaveNullParent)
{
	ASSERT_TRUE(root1->getParentFrame() == nullptr);
	ASSERT_TRUE(root2->getParentFrame() == nullptr);
}

TEST_F(ReferenceFrameTest, testWorldFramePointerStuff)
{
	const ReferenceFrame* worldFrame1 = ReferenceFrame::getWorldFrame();
	const ReferenceFrame* worldFrame2 = ReferenceFrame::getWorldFrame();
}

TEST_F(ReferenceFrameTest, testRootFramesArentTheSame)
{
	ASSERT_FALSE(root1 == root2);
}

TEST_F(ReferenceFrameTest, testGetRootFrame)
{
	ASSERT_TRUE(frame2->getRootFrame() == root1.get());
	ASSERT_TRUE(frame7->getRootFrame() == frame5->getRootFrame());

	//Throws error if these frames dont have the same root frames.
	frame7.get()->verifyFramesHaveSameRoot(frame6.get());

	try
	{
		frame7.get()->verifyFramesHaveSameRoot(frame1.get());
		ASSERT_TRUE(false);
	}
	catch ( ... )
	{
		ASSERT_TRUE(true);
	}
}

TEST_F(ReferenceFrameTest, testGetTransformBetweenFrames)
{

	for (int i = 0; i < nTests; i++)
	{
		ReferenceFrameTestHelper::updateAllFrames(allFrames);

		ReferenceFrame* tmpFrame1 = ReferenceFrameTestHelper::getARandomFrame(allFrames);
		ReferenceFrame* tmpFrame2 = ReferenceFrameTestHelper::getARandomFrame(allFrames);

		if (tmpFrame1->getRootFrame() != tmpFrame2->getRootFrame())
		{
			continue;
		}

		tf::Transform transform1 = tmpFrame1->getTransformToDesiredFrame(tmpFrame2);
		tf::Transform transform2 = tmpFrame2->getTransformToDesiredFrame(tmpFrame1);

		tf::Transform shouldBeIdentity = transform1 * transform2;

		if (!(ReferenceFrameTestHelper::isTransformIdentityWithinEpsilon(shouldBeIdentity, 1e-5)))
		{
			std::cout << "Failure frame 1: " << tmpFrame1->getName() << std::endl;
			std::cout << "Failure frame 2: " << tmpFrame2->getName() << std::endl;

			throw std::runtime_error("ERROR!");
		}
	}
}

TEST_F(ReferenceFrameTest, testGetTransformToParent)
{
	for (int i = 1; i < allFrames.size(); i++)
	{
		ReferenceFrame* tmpFrame2 = allFrames[i];

		ReferenceFrame* parentFrame = tmpFrame2->getParentFrame();

		if (parentFrame != nullptr)
		{
			EXPECT_TRUE(ReferenceFrameTestHelper::areTransformsEpsilonEqual(tmpFrame2->getTransformToParent(), tmpFrame2->getTransformToDesiredFrame(parentFrame), 1e-5));
		}
	}
}

TEST_F(ReferenceFrameTest, testGetTransformToRoot)
{
	for (int j = 0; j < nTests; j++)
	{
		ReferenceFrameTestHelper::updateAllFrames(allFrames);

		for (int i = 0; i < allFrames.size(); i++)
		{
			ReferenceFrame* frame = allFrames[i];
			tf::Transform transformToRoot = ReferenceFrameTestHelper::getTransformToRootByClimbingTree(frame);

			EXPECT_TRUE(ReferenceFrameTestHelper::areTransformsEpsilonEqual(transformToRoot, frame->getTransformToRoot(), 1e-5));
		}
	}
}

TEST_F(ReferenceFrameTest, testGetTransformToSelf)
{
	for (int i = 0; i < nTests; i++)
	{
		ReferenceFrameTestHelper::updateAllFrames(allFrames);

		for (int j = 0; j < allFrames.size(); j++)
		{
			ReferenceFrame* tmpFrame = allFrames[j];
			tf::Transform shouldBeIdentity = tmpFrame->getTransformToDesiredFrame(tmpFrame);
			EXPECT_TRUE(ReferenceFrameTestHelper::areTransformsEpsilonEqual(shouldBeIdentity, tf::Transform::getIdentity(), 1e-5));
		}
	}
}