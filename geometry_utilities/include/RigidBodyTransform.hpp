#ifndef RIGID_BODY_TRANSFORM_HPP
#define RIGID_BODY_TRANSFORM_HPP

#include <eigen3/Eigen/Eigen>
#include <tf2/LinearMath/Quaternion.h>
#include "Point3d.hpp"
#include "AxisAngle.hpp"

namespace geometry_utilities
{

    class RigidBodyTransform
    {
    public:
        RigidBodyTransform();
        RigidBodyTransform(const RigidBodyTransform& transform);
        RigidBodyTransform(const Eigen::Matrix4d& matrix);
        RigidBodyTransform(const Eigen::Matrix3d& matrix, const Eigen::Vector3d& vector);
        RigidBodyTransform(const tf2::Quaternion& quat, const Eigen::Vector3d& vector);
        RigidBodyTransform(const AxisAngle& axisAngle, const Eigen::Vector3d& vector);

        void setIdentity();

        void set(const RigidBodyTransform& transform);
        void set(const Eigen::Matrix4d& matrix);
        void set(const Eigen::Matrix3d& matrix, const Eigen::Vector3d& vector);
        void set(const tf2::Quaternion& quat, const Eigen::Vector3d& vector);
        void set(const AxisAngle& axisAngle, const Eigen::Vector3d& vector);

        void setTranslation(const double& x, const double& y, const double& z);
        void setTranslation(const Eigen::Vector3d& vector);
        void setAsTranspose(const Eigen::Matrix4d& matrix);
        void zeroTranslation();

        void setRotation(const Eigen::Matrix3d& matrix);
        void setRotation(const tf2::Quaternion& quat);
        void setRotation(const AxisAngle& axisAngle);

        void setRotationWithQuaternion(const double& qx, const double& qy, const double& qz, const double& qw);
        void setRotationWithAxisAngle(const double& axisAngleX, const double& axisAngleY, const double& axisAngleZ, const double& axisAngleTheta);
        void setRotationAndZeroTranslation(const Eigen::Matrix3d& matrix);
        void setTranslationAndIdentityRotation(const Eigen::Vector3d& vector);
        void setRotationToIdentity();
        void setEuler(const Eigen::Vector3d& vector);
        void setEuler(const double& rotX, const double& rotY, const double& rotZ);

        void getEulerXYZ(Eigen::Vector3d& vector) const;
        void getRotation(Eigen::Matrix3d& matrix) const;
        void getRotation(tf2::Quaternion& quat) const;

        void getTranslation(Eigen::Vector3d& vector) const;
        void getTranslation(Point3d& point) const;

        void get(Eigen::Matrix3d& matrix, Eigen::Vector3d& vector) const;
        void get(Eigen::Matrix3d& matrix) const;
        void get(Eigen::Vector3d& vector) const;
        void get(tf2::Quaternion& quat, Eigen::Vector3d& vector) const;
        void get(tf2::Quaternion& quat, Point3d& point) const;
        void get(tf2::Quaternion& quat) const;

        void applyTranslation(const Eigen::Vector3d& translation);

        void transform(Point3d& point);
        void transform(Eigen::Vector4d& vector);
        void transform(Eigen::Vector3d vector);
        void transform(const Eigen::Vector3d& vectorIn, Eigen::Vector3d& vectorOut);
        void transform(const Eigen::Vector4d& vectorIn, Eigen::Vector4d& vectorOut);
        void transform(Point3d pointIn, Point3d pointOut);

        void applyRotationX(const double& angle);
        void rotX(const double& angle);
        void rotY(const double& angle);
        void rotZ(const double& angle);

        void multiply(const RigidBodyTransform& transform);
        void multiply(const RigidBodyTransform& transform1, const RigidBodyTransform& transform);

        bool isRotationMatrixSingular(double epsilon) const;
        bool isRotationMatrixEpsilonIdentity(const double& epsilon) const;

        void invert(RigidBodyTransform transform);
        void invert();
        void invertOrthogonal();
        void invertRotationButKeepTranslation();

        bool epsilonEquals(const RigidBodyTransform& transform, const double& epsilon) const;
        bool equals(const RigidBodyTransform& transform) const;

        double determinant() const;

        void normalize();

        static bool almostZero(const double& a);
        static Eigen::Vector3d getTranslationDifference( const RigidBodyTransform& transform1, const RigidBodyTransform& transform2 );

    private:
        double mat00;
        double mat01;
        double mat02;
        double mat03;
        double mat10;
        double mat11;
        double mat13;
        double mat12;
        double mat20;
        double mat21;
        double mat22;
        double mat23;
    };

}

#endif