#include <gtest/gtest.h>
#include "frl/frames/FrameVector.hpp"
#include "ReferenceFrameTestHelper.hpp"
#include "GeometryUtilitiesTestHelper.hpp"

using namespace frame_utilities;

class FrameVectorTest : public ::testing::Test
{
protected:

    virtual void SetUp()
    {
        std::srand( time(NULL) );
    }

    virtual void TearDown()
    {
    }

    std::unique_ptr<ReferenceFrame> root = ReferenceFrame::createARootFrame("root1");
    std::shared_ptr<RandomUnchangingFrame> frame1 = RandomUnchangingFrame::create("frame1", root.get());
    std::shared_ptr<RandomUnchangingFrame> frame2 = RandomUnchangingFrame::create("frame2", frame1.get());

    int nTests = 1000;

private:

};

TEST_F(FrameVectorTest, testConstructors)
{
    FrameVector frameVector("boop",root.get(),1,2,3);

    Eigen::Vector3d vector = frameVector.getVector();
    EXPECT_TRUE(vector(0)==1);
    EXPECT_TRUE(vector(1)==2);
    EXPECT_TRUE(vector(2)==3);

    EXPECT_TRUE(frameVector.getName()=="boop");

    EXPECT_TRUE(frameVector.getReferenceFrame()->getName()=="root1");

    Eigen::Vector3d vector2(3,2,1);
    FrameVector frameVector2("beep",root.get(),vector2);

    Eigen::Vector3d vectorCheck = frameVector2.getVector();
    EXPECT_TRUE(vectorCheck(0)==3);
    EXPECT_TRUE(vectorCheck(1)==2);
    EXPECT_TRUE(vectorCheck(2)==1);

    EXPECT_TRUE(frameVector2.getName()=="beep");

    EXPECT_TRUE(frameVector2.getReferenceFrame()->getName()=="root1");
}

TEST_F(FrameVectorTest, testDot)
{
    FrameVector frameVector1("One",frame1.get(),-1,2,-3);
    FrameVector frameVector2("Two",frame2.get(),1,2,3);
    FrameVector frameVector3("Three",frame1.get(),4,5,-6);

    try
    {
        frameVector1.dot(frameVector2);
        EXPECT_TRUE(false);
    }
    catch( ... )
    {
        EXPECT_TRUE(true);
    }

    double value = frameVector1.dot(frameVector3);

    EXPECT_TRUE(value=24);
}

TEST_F(FrameVectorTest, testCross)
{
    Eigen::Vector3d result;
    Eigen::Vector3d expectedResult;

    for(int i = 0; i<nTests; i++)
    {
        Eigen::Vector3d v1(geometry_utilities::GeometryUtilitiesTestHelper::getRandomDouble(),geometry_utilities::GeometryUtilitiesTestHelper::getRandomDouble(),geometry_utilities::GeometryUtilitiesTestHelper::getRandomDouble());
        Eigen::Vector3d v2(geometry_utilities::GeometryUtilitiesTestHelper::getRandomDouble(),geometry_utilities::GeometryUtilitiesTestHelper::getRandomDouble(),geometry_utilities::GeometryUtilitiesTestHelper::getRandomDouble());
        Eigen::Vector3d v3(geometry_utilities::GeometryUtilitiesTestHelper::getRandomDouble(),geometry_utilities::GeometryUtilitiesTestHelper::getRandomDouble(),geometry_utilities::GeometryUtilitiesTestHelper::getRandomDouble());

        FrameVector frameVector1("One",frame1.get(),v1);
        FrameVector frameVector2("Two",frame2.get(),v2);
        FrameVector frameVector3("Three",frame1.get(),v3);

        try
        {
            frameVector1.cross(frameVector2);
            EXPECT_TRUE(false);
        }
        catch( ... )
        {

        }

        Eigen::Vector3d result = frameVector1.cross(frameVector3);
        Eigen::Vector3d expectedResult = v1.cross(v3);
        EXPECT_TRUE(result(0)==expectedResult(0));
        EXPECT_TRUE(result(1)==expectedResult(1));
        EXPECT_TRUE(result(2)==expectedResult(2));
    }
}

TEST_F(FrameVectorTest, testAngleBetweenVectors)
{
    FrameVector frameVector1("One",frame1.get(),2,3,1);
    FrameVector frameVector2("Two",frame1.get(),4,1,2);

    double angle = frameVector1.getAngleBetweenVectors(frameVector2);
    double expectedResult = acos(13/(sqrt(14)*sqrt(21)));
    EXPECT_TRUE(angle==expectedResult);
}

TEST_F(FrameVectorTest, testChangeFrame)
{
    geometry_utilities::RigidBodyTransform transform1;

    transform1.setIdentity();
    Eigen::Vector3d rpy(M_PI/2, 0, 0);
    Eigen::Vector3d translation(5.0, 0.0, 0.0);
    transform1.setEuler(rpy);
    transform1.setTranslation(translation);

    std::shared_ptr<ReferenceFrame> frameA(new RandomUnchangingFrame("A", root.get(), transform1));

    transform1.setIdentity();
    rpy.setZero();
    rpy << 0, M_PI/2, 0.0;
    translation << 5.0, 0, 0;
    transform1.setEuler(rpy);
    transform1.setTranslation(translation);

    std::shared_ptr<ReferenceFrame> frameB(new RandomUnchangingFrame("B", frameA.get(), transform1));

    transform1.setIdentity();
    rpy.setZero();
    rpy << 0, 0, M_PI/2;
    translation << 5.0, 0, 0;
    transform1.setEuler(rpy);
    transform1.setTranslation(translation);

    std::shared_ptr<ReferenceFrame> frameC(new RandomUnchangingFrame("C", frameB.get(), transform1));

    double x = 3.0;
    double y = 1.0;
    double z = -9.0;

    FrameVector frameVector("FrameVector", frameC.get(), x, y, z);
    frameVector.changeFrame(frameB.get());

    EXPECT_TRUE(frameVector.getX()-(-1)<1e-15 );
    EXPECT_TRUE(frameVector.getY()-3<1e-15);
    EXPECT_TRUE(frameVector.getZ()-(-9)<1e-15);

    frameVector.changeFrame(frameA.get());

    EXPECT_TRUE(frameVector.getX()-(-9)<1e-15 );
    EXPECT_TRUE(frameVector.getY()-3<1e-15);
    EXPECT_TRUE(frameVector.getZ()-1<1e-15);
}

TEST_F(FrameVectorTest, testVectorLength)
{
    Eigen::Vector3d result;
    Eigen::Vector3d expectedResult;

    for(int i = 0; i<nTests; i++)
    {
        Eigen::Vector3d v1(geometry_utilities::GeometryUtilitiesTestHelper::getRandomDouble(),geometry_utilities::GeometryUtilitiesTestHelper::getRandomDouble(),geometry_utilities::GeometryUtilitiesTestHelper::getRandomDouble());

        FrameVector frameVector1("One",frame1.get(),v1);

        double result = frameVector1.length();
        double expectedResult = v1.norm();
        EXPECT_TRUE(result==expectedResult);
    }
}