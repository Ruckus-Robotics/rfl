#include <gtest/gtest.h>
#include "ReferenceFrame.hpp"
#include "ReferenceFrameTestHelper.hpp"

class ReferenceFrameTest : public ::testing::Test
{
	protected:

		virtual void SetUp()
		{
		}
		virtual void TearDown()
		{
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

		int nTests = 100;

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