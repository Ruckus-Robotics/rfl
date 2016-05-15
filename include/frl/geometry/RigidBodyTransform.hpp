#ifndef RIGID_BODY_TRANSFORM_HPP
#define RIGID_BODY_TRANSFORM_HPP

#include <eigen3/Eigen/Eigen>
#include "frl/geometry/Point3.hpp"

namespace frl
{
	namespace geometry
	{
		template<typename T>
		class RigidBodyTransform
        {
        public:
            RigidBodyTransform()
            {
                setIdentity();
            }

            template<typename TYPE>
            RigidBodyTransform(const RigidBodyTransform<TYPE> &transform)
            {
                set(transform);
            }

            template<typename TYPE>
            RigidBodyTransform(const Eigen::Matrix<TYPE, 4, 4> &matrix)
            {
                set(matrix);
            }

            template<typename T1, typename T2>
            RigidBodyTransform(const Eigen::Matrix<T1, 3, 3> &matrix, const Eigen::Matrix<T2, 3, 1> &vector)
            {
                set(matrix, vector);
            };

            template<typename T1, typename T2>
            RigidBodyTransform(const Eigen::AngleAxis<T1> &axisAngle, const Eigen::Matrix<T2, 3, 1> &vector)
            {
                set(axisAngle, vector);
            }

            template<typename T1, typename T2>
            RigidBodyTransform(const Eigen::Quaternion<T1> &quaternion, const Eigen::Matrix<T2, 3, 1> &vector)
            {
                set(quaternion, vector);
            };

            template<typename TYPE>
            RigidBodyTransform(const Eigen::Matrix<TYPE, 3, 3> &matrix)
            {
                setRotation(matrix);
                setTranslation(0.0, 0.0, 0.0);
            }

            template<typename TYPE>
            RigidBodyTransform(const Eigen::Quaternion<TYPE> &quat)
            {
                setRotation(quat.w(),quat.x(),quat.y(),quat.z());
                setTranslation(0.0, 0.0, 0.0);
            }

            template<typename TYPE>
            RigidBodyTransform(const Eigen::AngleAxis<TYPE> &axisAngle)
            {
                setRotation(axisAngle);
                setTranslation(0.0, 0.0, 0.0);
            }

            ~RigidBodyTransform()
            { };

            /**
            * Set transformation matrix to Identity, meaning no rotation or
            * translation.
            */
            void setIdentity()
            {

                qw = 1.0;
                qx = 0.0;
                qy = 0.0;
                qz = 0.0;

                x = 0.0;
                y = 0.0;
                z = 0.0;
            }

            template<typename TYPE>
            void set(const RigidBodyTransform<TYPE> &transform)
            {
                qw = transform.qw;
                qx = transform.qx;
                qy = transform.qy;
                qz = transform.qz;

                x = transform.x;
                y = transform.y;
                z = transform.z;
            }

            /**
             * Set elements of transform
             *
             * @param matrix
             */
            template<typename TYPE>
            void set(const Eigen::Matrix<TYPE, 4, 4> &matrix)
            {
                setRotationWithRotationMatrix(matrix(0,0),matrix(1,0),matrix(2,0),matrix(0,1),matrix(1,1),matrix(2,1),matrix(0,2),matrix(1,2),matrix(2,2));
                setTranslation(matrix(0,3),matrix(1,3),matrix(2,3));
            }

            /**
            * Set this transform to have translation described in vector
            * and a rotation equal to matrix.
            *
            * @param Eigen::Matrix matrix
            * @param Eigen::Matrix vector
            */
            template<typename T1, typename T2>
            void set(const Eigen::Matrix<T1, 3, 3> &matrix, const Eigen::Matrix<T2, 3, 1> &vector)
            {
                setRotation(matrix);
                setTranslation(vector);
            }

            /**
            * Set this transform to have translation described in vector
            * and a rotation equal to axisAngles.
            *
            * @param Eigen::AxisAngle axisAngle
            * @param Eigen::Matrix vector
            */
            template<typename T1, typename T2>
            void set(const Eigen::AngleAxis<T1> &axisAngle, const Eigen::Matrix<T2, 3, 1> &vector)
            {
                setRotation(axisAngle);
                setTranslation(vector);
            }

            /**
            * Set this transform to have translation described in vector
            * and a rotation equal to quat.
            *
            * @param Eigen::Quaternion quat
            * @param Eigen::Matrix vector
            */
            template<typename T1, typename T2>
            void set(const Eigen::Quaternion<T1> &quat, const Eigen::Matrix<T2, 3, 1> &vector)
            {
                setRotation(quat);
                setTranslation(vector);
            }


            /**
             * Set translational portion of the transformation matrix
             *
             * @param x The x-component of the translation
             * @param y The y-component of the translation
             * @param z The z-component of the translation
             */
            template<typename TYPE>
            void setTranslation(const TYPE x, const TYPE y, const TYPE z)
            {
                this->x = x;
                this->y = y;
                this->z = z;
            }

            /**
             * Set translational portion of the transformation matrix
             *
             * @param vector
             */
            template<typename TYPE>
            void setTranslation(const Eigen::Matrix<TYPE, 3, 1> &vector)
            {
                this->x = vector(0);
                this->y = vector(1);
                this->z = vector(2);
            }

            void zeroTranslation()
            {
                x = 0.0;
                y = 0.0;
                z = 0.0;
            }

            template<typename TYPE>
            void setRotation(Eigen::Matrix<TYPE, 3, 3> matrix)
            {
                setRotationWithRotationMatrix(matrix(0,0),matrix(1,0),matrix(2,0),matrix(0,1),matrix(1,1),matrix(2,1),matrix(0,2),matrix(1,2),matrix(2,2));
            }

            void setRotation(const T qw, const T qx, const T qy, const T qz)
            {
                this->qx = qx;
                this->qy = qy;
                this->qz = qz;
                this->qw = qw;
            }

            template<typename TYPE>
            void setRotation(const Eigen::Quaternion<TYPE> &quat)
            {
                this->qx = quat.x();
                this->qy = quat.y();
                this->qz = quat.z();
                this->qw = quat.w();
            }

            template<typename TYPE>
			void setRotation(const Eigen::AngleAxis<TYPE> &axisAngle)
            {
                setRotationWithAxisAngle(axisAngle.axis()[0], axisAngle.axis()[1], axisAngle.axis()[2], axisAngle.angle());
            }

			template<typename TYPE>
			void setRotationWithAxisAngle(const TYPE axisAngleX, const TYPE axisAngleY, const TYPE &axisAngleZ, const TYPE axisAngleTheta)
            {
                qx = axisAngleX;
                qy = axisAngleY;
                qz = axisAngleZ;
                T n = sqrt(qx*qx + qy*qy + qz*qz);
                // Division by zero might happen here. Meh
                T s = sin(0.5*axisAngleTheta)/n;
                qx *= s;
                qy *= s;
                qz *= s;
                qw = cos(0.5*axisAngleTheta);
            }

            template<typename TYPE>
			void setRotationAndZeroTranslation(const Eigen::Matrix<TYPE,3,3> &matrix)
            {
                setRotation(matrix);
                setTranslation(0.0,0.0,0.0);
            }

            template<typename TYPE>
			void setRotationAndZeroTranslation(const Eigen::Quaternion<TYPE> &quat)
            {
                setRotation(quat);
                setTranslation(0.0,0.0,0.0);
            }

            template<typename TYPE>
			void setRotationAndZeroTranslation(const Eigen::AngleAxis<TYPE> &axisAngle)
            {
                setRotation(axisAngle);
                setTranslation(0.0,0.0,0.0);
            }

            /**
            * Set this transform to have an identity rotation and a translation given
            * by the Eigen::Vector3d vector.
            *
            * @param vector
            */
            template<typename TYPE>
			void setTranslationAndIdentityRotation(const Eigen::Matrix<TYPE,3,1> &vector)
            {
                setTranslation(vector(0), vector(1), vector(2));
                setRotationToIdentity();
            }

            /**
            * Sets rotation to the identity, does not effect the translational component of the Transform
            *
            */
			void setRotationToIdentity()
            {
                this->mat00 = 1.0;
                this->mat01 = 0.0;
                this->mat02 = 0.0;
                this->mat10 = 0.0;
                this->mat11 = 1.0;
                this->mat12 = 0.0;
                this->mat20 = 0.0;
                this->mat21 = 0.0;
                this->mat22 = 1.0;
            }

            /**
            * Set the rotational component of the transform to the rotation matrix
            * created given an X-Y-Z rotation described by the angles in vector which
            * describe angles of rotation about the X, Y, and Z axis, respectively. The
            * orientation of each rotation is not effected by any of the other
            * rotations. This method sets the translational component of this
            * transform3d to zeros.
            *
            * @param vector
            */
            template<typename TYPE>
			void setEulerXYZ(const Eigen::Matrix<TYPE,3,1> &vector)
            {
                setEulerXYZ(vector(0), vector(1), vector(2));
            }

            /**
             * Set the rotational component of the transform to the rotation matrix
             * created given an X-Y-Z rotation described by the angles in vector which
             * describe angles of rotation about the X, Y, and Z axis, respectively. The
             * orientation of each rotation is not effected by any of the other
             * rotations. This method sets the translational component of this
             * transform3d to zeros.
             *
             * @param rotX
             * @param rotY
             * @param rotZ
             */
            template<typename TYPE>
			void setEulerXYZ(const TYPE roll, const TYPE pitch, const TYPE yaw)
            {
                T halfYaw = yaw/2;
                T halfRoll = roll/2;
                T halfPitch = pitch/2;

                T cosYaw = cos(halfYaw);
                T cosRoll = cos(halfRoll);
                T cosPitch = cos(halfPitch);
                T sinYaw = sin(halfYaw);
                T sinRoll = sin(halfRoll);
                T sinPitch = sin(halfPitch);

                T cosRollCosPitch = cosRoll*cosPitch;
                T sinRollSinPitch = sinRoll*sinPitch;
                T cosRollSinPitch = cosRoll*sinPitch;
                T sinRollCosPitch = sinRoll*cosPitch;

                qw = cosRollCosPitch*cosYaw - sinRollSinPitch*sinYaw;
                qx = sinRollCosPitch*cosYaw + cosRollSinPitch*sinYaw;
                qy = cosRollSinPitch*cosYaw - sinRollCosPitch*sinYaw;
                qz = sinRollSinPitch*cosYaw + cosRollCosPitch*sinYaw;

                normalize();
            }

            /**
            * Computes the RPY angles from the rotation matrix for rotations about the
            * X, Y, and Z axes respectively. Note that this method is here for the
            * purpose of unit testing the method setEuler. This particular solution is
            * only valid for -pi/2 < vector.y < pi/2 and for vector.y != 0.
            *
            * @param vector
            */
            template<typename TYPE>
			void getEulerXYZ(Eigen::Matrix<TYPE,3,1> &vector) const
            {
                vector(0) = atan2(mat21, mat22);
                vector(1) = atan2(-mat20, sqrt(mat21 * mat21 + mat22 * mat22));
                vector(2) = atan2(mat10, mat00);
            }

            /**
            * Return rotation in quaternion form.
            *
            * @param Eigen::Quaternion quat
            */
            template<typename TYPE>
            void getQuaternion(Eigen::Quaternion<TYPE> &q) const
            {
                q.x() = qx;
                q.y() = qy;
                q.z() = qz;
                q.w() = qw;
            }

            /**
            * Return rotation matrix
            *
            * @param matrix
            */
            template<typename TYPE>
			void getRotation(Eigen::Matrix<TYPE,3,3> &matrix) const
            {
                matrix(0, 0) = mat00;
                matrix(0, 1) = mat01;
                matrix(0, 2) = mat02;
                matrix(1, 0) = mat10;
                matrix(1, 1) = mat11;
                matrix(1, 2) = mat12;
                matrix(2, 0) = mat20;
                matrix(2, 1) = mat21;
                matrix(2, 2) = mat22;
            }

            /**
            * Return rotation in AxisAngle form.
            *
            * @param axisAngle
            */
            template<typename TYPE>
			void getRotation(Eigen::AngleAxis<TYPE> &axisAngle) const
            {
                getRotation(axisAngle,1.0e-12);
            }

            /**
            * Return translational part
            *
            * @param vector
            */
            template<typename TYPE>
			void getTranslation(Eigen::Matrix<TYPE,3,1> &vector) const
            {
                vector(0) = x;
                vector(1) = y;
                vector(2) = z;
            }

            /**
            * Return translational part
            *
            * @param point
            */
            template<typename TYPE>
			void getTranslation(Point3<TYPE> &point) const
            {
                point.x = x;
                point.y = y;
                point.z = z;
            }

            /**
            * Pack transform into matrix
            *
            * @param matrix
            */
            template<typename TYPE>
			void get(Eigen::Matrix<TYPE,4,4> &matrix) const
            {
                //@TODO update
                matrix(0, 0) = mat00;
                matrix(0, 1) = mat01;
                matrix(0, 2) = mat02;
                matrix(0, 3) = mat03;

                matrix(1, 0) = mat10;
                matrix(1, 1) = mat11;
                matrix(1, 2) = mat12;
                matrix(1, 3) = mat13;

                matrix(2, 0) = mat20;
                matrix(2, 1) = mat21;
                matrix(2, 2) = mat22;
                matrix(2, 3) = mat23;

                matrix(3, 0) = 0;
                matrix(3, 1) = 0;
                matrix(3, 2) = 0;
                matrix(3, 3) = 1;
            }

            template<typename TYPE>
			void get(Eigen::Matrix<TYPE,3,3> &matrix, Eigen::Matrix<TYPE,3,1> &vector) const
            {
                getRotation(matrix);
                getTranslation(vector);
            }

            template<typename TYPE>
			void get(Eigen::Quaternion<TYPE> &quat, Eigen::Matrix<TYPE,3,1> &vector) const
            {
                getRotation(quat);
                getTranslation(vector);
            }

            template<typename TYPE>
			void get(Eigen::Quaternion<TYPE> &quat, Point3<TYPE> &point) const
            {
                getRotation(quat);
                getTranslation(point);
            }

            template<typename TYPE>
			void applyTranslation(const Eigen::Matrix<TYPE,3,1> &translation)
            {
                Point3<TYPE> temp(translation(0),translation(1),translation(2));
                transform(temp);
                mat03 = temp.x;
                mat13 = temp.y;
                mat23 = temp.z;
            }

            /**
            * Transform the Point3 point by this transform and place result back in
            * point.
            *
            * @param point
            */
            template<typename TYPE>
			void transform(Point3<TYPE> &point)
            {
                TYPE x = mat00 * point.x + mat01 * point.y + mat02 * point.z + mat03;
                TYPE y = mat10 * point.x + mat11 * point.y + mat12 * point.z + mat13;
                point.z = mat20 * point.x + mat21 * point.y + mat22 * point.z + mat23;

                point.x = x;
                point.y = y;
            }

            /**
            * Transform vector by multiplying it by this transform and put result back
            * into vector.
            *
            * @param vector
            */
            template<typename TYPE>
			void transform(Eigen::Matrix<TYPE,4,1> &vector)
            {
                TYPE x = mat00 * vector(0) + mat01 * vector(1) + mat02 * vector(2) + mat03;
                TYPE y = mat10 * vector(0) + mat11 * vector(1) + mat12 * vector(2) + mat13;
                vector(2) = mat20 * vector(0) + mat21 * vector(1) + mat22 * vector(2) + mat23;
                vector(0) = x;
                vector(1) = y;
                vector(3) = 1.0;
            }

            /**
             * Transform vector by multiplying it by this transform and put result back
             * into vector.
             *
             * @param vector
             */
            template<typename TYPE>
			void transform(Eigen::Matrix<TYPE,3,1> &vector)
            {
                TYPE x = mat00 * vector(0) + mat01 * vector(1) + mat02 * vector(2);
                TYPE y = mat10 * vector(0) + mat11 * vector(1) + mat12 * vector(2);
                vector(2) = mat20 * vector(0) + mat21 * vector(1) + mat22 * vector(2);

                vector(0) = x;
                vector(1) = y;
            }

            /**
            * Transform vector by multiplying it by this transform and put result back
            * into vector.
            *
            * @param vector
            */
            template<typename TYPE>
			void transform(const Eigen::Matrix<TYPE,3,1> &vectorIn, Eigen::Matrix<TYPE,3,1> &vectorOut)
            {
                vectorOut(0) = mat00 * vectorIn(0) + mat01 * vectorIn(1) + mat02 * vectorIn(2);
                vectorOut(1) = mat10 * vectorIn(0) + mat11 * vectorIn(1) + mat12 * vectorIn(2);
                vectorOut(2) = mat20 * vectorIn(0) + mat21 * vectorIn(1) + mat22 * vectorIn(2);
            }

            /**
            * Transform vectorIn using this transform and store result in vectorOut.
            *
            * @param vectorIn
            * @param vectorOut
            */
            template<typename TYPE>
			void transform(const Eigen::Matrix<TYPE,4,1> &vectorIn, Eigen::Matrix<TYPE,4,1> &vectorOut)
            {
                vectorOut(0) = mat00 * vectorIn(0) + mat01 * vectorIn(1) + mat02 * vectorIn(2) + mat03;
                vectorOut(1) = mat10 * vectorIn(0) + mat11 * vectorIn(1) + mat12 * vectorIn(2) + mat13;
                vectorOut(2) = mat20 * vectorIn(0) + mat21 * vectorIn(1) + mat22 * vectorIn(2) + mat23;
                vectorOut(3) = 1.0;
            }

            /**
            * Transform the Point3d pointIn by this transform and place result in
            * pointOut.
            *
            * @param point
            */
            template<typename TYPE>
			void transform(const Point3<TYPE> &pointIn, Point3<TYPE> &pointOut)
            {
                pointOut.x = mat00 * pointIn.x + mat01 * pointIn.y + mat02 * pointIn.z + mat03;
                pointOut.y = mat10 * pointIn.x + mat11 * pointIn.y + mat12 * pointIn.z + mat13;
                pointOut.z = mat20 * pointIn.x + mat21 * pointIn.y + mat22 * pointIn.z + mat23;
            }

            /**
            *  Apply a x-axis rotation to the current transform.
            */
			template<typename TYPE>
			void applyRotationX(const TYPE angle)
            {
                RigidBodyTransform<T> temp;
                temp.rotX(angle);
                multiply(temp);
            }

            /**
            *  Apply a y-axis rotation to the current transform.
            */
			template<typename TYPE>
			void applyRotationY(const TYPE angle)
            {
                RigidBodyTransform<T> temp;
                temp.rotY(angle);
                multiply(temp);
            }

            /**
            *  Apply a z-axis rotation to the current transform.
            */
			template<typename TYPE>
			void applyRotationZ(const TYPE angle)
            {
                RigidBodyTransform<T> temp;
                temp.rotZ(angle);
                multiply(temp);
            }

			template<typename TYPE>
			void rotX(const TYPE angle)
            {
                TYPE cosAngle = cos(angle);
                TYPE sinAngle = sin(angle);

                this->mat00 = 1.0;
                this->mat01 = 0.0;
                this->mat02 = 0.0;
                this->mat03 = 0.0;
                this->mat10 = 0.0;
                this->mat11 = cosAngle;
                this->mat12 = -sinAngle;
                this->mat13 = 0.0;
                this->mat20 = 0.0;
                this->mat21 = sinAngle;
                this->mat22 = cosAngle;
                this->mat23 = 0.0;
            }

            /**
            * Create RigidBodyTransform with zero translation and the rotation matrix being a
            * rotation about the y-axis by angle.
            *
            * @param angle
            */
			template<typename TYPE>
			void rotY(const TYPE angle)
            {
                TYPE cosAngle = cos(angle);
                TYPE sinAngle = sin(angle);

                mat00 = cosAngle;
                mat01 = 0.0;
                mat02 = sinAngle;
                mat03 = 0.0;
                mat10 = 0.0;
                mat11 = 1.0;
                mat12 = 0.0;
                mat13 = 0.0;
                mat20 = -sinAngle;
                mat21 = 0.0;
                mat22 = cosAngle;
                mat23 = 0.0;
            }

            /**
             * Create RigidBodyTransform with zero translation and the rotation matrix being a
             * rotation about the z-axis by angle.
             *
             * @param angle
             */
			template<typename TYPE>
			void rotZ(const TYPE angle)
            {
                TYPE cosAngle = cos(angle);
                TYPE sinAngle = sin(angle);

                mat00 = cosAngle;
                mat01 = -sinAngle;
                mat02 = 0.0;
                mat03 = 0.0;
                mat10 = sinAngle;
                mat11 = cosAngle;
                mat12 = 0.0;
                mat13 = 0.0;
                mat20 = 0.0;
                mat21 = 0.0;
                mat22 = 1.0;
                mat23 = 0.0;
            }

            /**
            * Multiplies this RigidBodyTransform by transform and stores the result in this,
            * i.e. this = this*transform
            *
            * @param transform
            */
            template<typename TYPE>
			void multiply(const RigidBodyTransform<TYPE> &transform)
            {
                T tmpQw = qw*transform.qw - qx*transform.qx - qy*transform.qy - qz*transform.qz;
                T tmpQx = qx*transform.qw + qw*transform.qx - qz*transform.qy + qy*transform.qz;
                T tmpQy = qy*transform.qw + qz*transform.qx + qw*transform.qy - qx*transform.qz;
                T tmpQz = qz*transform.qw - qy*transform.qx + qx*transform.qy + qw*transform.qz;

                T tmpX = (pow(qw,2) + pow(qx,2) - pow(qy,2) - pow(qz,2))*transform.x +
                         2*(qx*qy - qw*qz)*transform.y + 2*(qw*qy + qx*qz)*transform.z + x;

                T tmpY = 2*(qx*qy + qw*qz)*transform.x + (pow(qw,2) - pow(qx,2) + pow(qy,2) - pow(qz,2))*transform.y +
                        2*(-qw*qx + qy*qz)*transform.z + y;

                T tmpZ = (-2*qw*qy + 2*qx*qz)*transform.x + 2*(qw*qx + qy*qz)*transform.y +
                        (pow(qw,2) - pow(qx,2) - pow(qy,2) + pow(qz,2))*transform.z + z;

                qw = tmpQw;
                qx = tmpQx;
                qy = tmpQy;
                qz = tmpQz;

                x = tmpX;
                y = tmpY;
                z = tmpZ;
            }

            /**
            * Multiplies transform1 and transform and puts result into this. this =
            * transform1*transform
            *
            * @param transform1
            * @param transform
            */
            template<typename TYPE>
			void multiply(const RigidBodyTransform<TYPE> &transform1, const RigidBodyTransform<TYPE> &transform2)
            {
                T tmpQw = transform1.qw*transform2.qw - transform1.qx*transform2.qx - transform1.qy*transform2.qy - transform1.qz*transform2.qz;
                T tmpQx = transform1.qx*transform2.qw + transform1.qw*transform2.qx - transform1.qz*transform2.qy + transform1.qy*transform2.qz;
                T tmpQy = transform1.qy*transform2.qw + transform1.qz*transform2.qx + transform1.qw*transform2.qy - transform1.qx*transform2.qz;
                T tmpQz = transform1.qz*transform2.qw - transform1.qy*transform2.qx + transform1.qx*transform2.qy + transform1.qw*transform2.qz;

                T tmpX = (pow(transform1.qw,2) + pow(transform1.qx,2) - pow(transform1.qy,2) - pow(transform1.qz,2))*transform2.x +
                         2.0*(transform1.qx*transform1.qy - transform1.qw*transform1.qz)*transform2.y + 2.0*(transform1.qw*transform1.qy + transform1.qx*transform1.qz)*transform2.z + transform1.x;

                T tmpY = 2.0*(transform1.qx*transform1.qy + transform1.qw*transform1.qz)*transform2.x + (pow(transform1.qw,2) - pow(transform1.qx,2) + pow(transform1.qy,2) - pow(transform1.qz,2))*transform2.y +
                         2.0*(-transform1.qw*transform1.qx + transform1.qy*transform1.qz)*transform2.z + transform1.y;

                T tmpZ = (-2.0*transform1.qw*transform1.qy + 2*transform1.qx*transform1.qz)*transform2.x + 2.0*(transform1.qw*transform1.qx + transform1.qy*transform1.qz)*transform2.y +
                         (pow(transform1.qw,2) - pow(transform1.qx,2) - pow(transform1.qy,2) + pow(transform1.qz,2))*transform2.z + transform1.z;

                qw = tmpQw;
                qx = tmpQx;
                qy = tmpQy;
                qz = tmpQz;

                x = tmpX;
                y = tmpY;
                z = tmpZ;
            }

			bool isRotationMatrixEpsilonIdentity(const double epsilon) const
            {
                return fabs(qx) < epsilon && fabs(qy) < epsilon && fabs(qz) < epsilon && fabs(1-qw);
            }

            /**
            * Compute the inverse of the RigidBodyTransform passed in as an
            * argument exploiting the orthogonality of the rotation matrix
            * and store the result in this.
            * @param transform
            */
            template<typename TYPE>
			void invert(const RigidBodyTransform<TYPE> &transform)
            {
                set(transform);
                invert();
            }

			void invert()
            {
                T n = norm();
                qx/=-n;
                qy/=-n;
                qz/=-n;
                qw/=n;

                T tempX = pow(qw,2)*x + pow(qx,2)*x - (pow(qy,2) + pow(qz,2))*x + qw*(-2*qz*y + 2*qy*z) +2*qx*(qy*y + qz*z);
                T tempY = 2*qx*qy*x + 2*qw*qz*x + pow(qw,2)*y - pow(qx,2)*y + pow(qy,2)*y - pow(qz,2)*y - 2*qw*qx*z + 2*qy*qz*z;
                z = qw (-2*qy*x + 2*qx*y) + 2*qz*(qx*x + qy*y) + pow(qw,2)*z - (pow(qx,2) + pow(qy,2) - pow(qz,2))*z;

                x = tempX;
                y = tempY;
            }

			void invertRotationButKeepTranslation()
            {
                T n = norm();
                qx/=-n;
                qy/=-n;
                qz/=-n;
                qw/=n;
            }

            /**
            * Check if the elements of this are within epsilon of the elements of
            * transform.
            *
            * @param transform
            * @param epsilon
            * @return
            */
            template<typename TYPE>
			bool epsilonEquals(const RigidBodyTransform<TYPE> &transform, const double &epsilon) const
            {
                if (!fabs(qx - transform.qx) < epsilon)
                {
                    return false;
                }

                if (!fabs(qy - transform.qy) < epsilon)
                {
                    return false;
                }

                if (!fabs(qz - transform.qz) < epsilon)
                {
                    return false;
                }

                if (!fabs(qw - transform.qw) < epsilon)
                {
                    return false;
                }

                if (!fabs(x - transform.x) < epsilon)
                {
                    return false;
                }

                if (!fabs(y - transform.y) < epsilon)
                {
                    return false;
                }

                if (!fabs(z - transform.z) < epsilon)
                {
                    return false;
                }

                return true;
            }

            /**
            * Orthonormalization of the rotation matrix using Gram-Schmidt method.
            */
			void normalize()
            {
                T norm = norm();

                qx/=norm;
                qy/=norm;
                qz/=norm;
                qw/=norm;
            }

            T norm()
            {
                return sqrt(qx*qx + qy*qy + qz*qz + qw*qw);
            }

            T normSquared()
            {
                return qx*qx + qy*qy + qz*qz + qw*qw;
            }

            template<typename TYPE>
			static Eigen::Matrix<TYPE,3,1> getTranslationDifference(const RigidBodyTransform<TYPE> &transform1, const RigidBodyTransform<TYPE> &transform2)
            {
                Eigen::Matrix<TYPE,3,1> pos1;
                Eigen::Matrix<TYPE,3,1> pos2;
                transform1.getTranslation(pos1);
                transform2.getTranslation(pos2);

                return (pos2 - pos1);
            }

            template<typename TYPE>
			void getRotation(Eigen::AngleAxis<TYPE> &axisAngle, const double epsilon) const
            {
//                Eigen::Matrix<TYPE,3,3> m;
//                m << mat00, mat01, mat02, mat10, mat11, mat12, mat20, mat21, mat22;
//                std::cout << m << std::endl;
//                axisAngle.fromRotationMatrix(m);
                axisAngle.axis()[0] = mat21 - mat12;
                axisAngle.axis()[1] = mat02 - mat20;
                axisAngle.axis()[2] = mat10 - mat01;

                TYPE mag = axisAngle.axis()[0] * axisAngle.axis()[0] + axisAngle.axis()[1] * axisAngle.axis()[1] + axisAngle.axis()[2] * axisAngle.axis()[2];

                if (mag > epsilon)
                {
                    mag = sqrt(mag);
                    TYPE sin = 0.5 * mag;
                    TYPE cos = 0.5 * (mat00 + mat11 + mat22 - 1.0);

                    axisAngle.angle() = atan2(sin, cos);

                    TYPE invMag = 1.0 / mag;
                    axisAngle.axis()[0] = axisAngle.axis()[0] * invMag;
                    axisAngle.axis()[1] = axisAngle.axis()[1] * invMag;
                    axisAngle.axis()[2] = axisAngle.axis()[2] * invMag;
                }
                else
                {
                    if (isRotationMatrixEpsilonIdentity(10.0 * epsilon))
                    {
                        axisAngle.angle() = 0.0;
                        axisAngle.axis()[0] = 1.0;
                        axisAngle.axis()[1] = 0.0;
                        axisAngle.axis()[2] = 0.0;
                        return;
                    }
                    else
                    {
                        axisAngle.angle() = M_PI;

                        TYPE xx = (mat00 + 1.0) / 2.0;
                        TYPE yy = (mat11 + 1.0) / 2.0;
                        TYPE zz = (mat22 + 1.0) / 2.0;
                        TYPE xy = (mat01 + mat10) / 4.0;
                        TYPE xz = (mat02 + mat20) / 4.0;
                        TYPE yz = (mat12 + mat21) / 4.0;
                        TYPE cos45 = cos(M_PI / 4.0);

                        if ((xx > yy) && (xx > zz))
                        {
                            // mat00 is the largest diagonal term
                            if (xx < epsilon)
                            {
                                axisAngle.axis()[0] = 0.0;
                                axisAngle.axis()[1] = cos45;
                                axisAngle.axis()[2] = cos45;
                            }
                            else
                            {
                                axisAngle.axis()[0] = sqrt(xx);
                                axisAngle.axis()[1] = xy / axisAngle.axis()[0];
                                axisAngle.axis()[2] = xz / axisAngle.axis()[0];
                            }
                        }
                        else if (yy > zz)
                        {
                            // mat11 is the largest diagonal term
                            if (yy < epsilon)
                            {
                                axisAngle.axis()[0] = cos45;
                                axisAngle.axis()[1] = 0.0;
                                axisAngle.axis()[2] = cos45;
                            }
                            else
                            {
                                axisAngle.axis()[1] = sqrt(yy);
                                axisAngle.axis()[0] = xy / axisAngle.axis()[1];
                                axisAngle.axis()[2] = yz / axisAngle.axis()[1];
                            }
                        }
                        else
                        {
                            // mat22 is the largest diagonal term
                            if (zz < epsilon)
                            {
                                axisAngle.axis()[0] = cos45;
                                axisAngle.axis()[1] = cos45;
                                axisAngle.axis()[2] = 0.0;
                            }
                            else
                            {
                                axisAngle.axis()[2] = sqrt(zz);
                                axisAngle.axis()[0] = xz / axisAngle.axis()[2];
                                axisAngle.axis()[1] = yz / axisAngle.axis()[2];
                            }
                        }
                    }
                }
            }

            template<typename TYPE>
            RigidBodyTransform<T>& operator*=(const RigidBodyTransform<TYPE> &transform)
            {
                this->multiply(transform);
                return *this;
            }

            friend std::ostream& operator<<(std::ostream &os, const RigidBodyTransform &transform)
            {
                os << "[ " << transform.mat00 << ',' << transform.mat01 << "," << transform.mat02 << "," << transform.mat03 << "]" << "\n" <<
                "[ " << transform.mat10 << ',' << transform.mat11 << "," << transform.mat12 << "," << transform.mat13 << "]" << "\n" <<
                "[ " << transform.mat20 << ',' << transform.mat21 << "," << transform.mat22 << "," << transform.mat23 << "]" << "\n" <<
                "[ " << 0 << ',' << 0 << "," << 0 << "," << 1 << "]";
                return os;
            }

            template<typename TYPE>
            RigidBodyTransform<T>& operator=(RigidBodyTransform<TYPE> rhs)
            {
                mat00 = rhs.mat00;
                mat01 = rhs.mat01;
                mat02 = rhs.mat02;
                mat03 = rhs.mat03;
                mat10 = rhs.mat10;
                mat11 = rhs.mat11;
                mat12 = rhs.mat12;
                mat13 = rhs.mat13;
                mat20 = rhs.mat20;
                mat21 = rhs.mat21;
                mat22 = rhs.mat22;
                mat23 = rhs.mat23;

                return *this;
            }

            T mat00;
            T mat01;
            T mat02;
            T mat03;
            T mat10;
            T mat11;
            T mat13;
            T mat12;
            T mat20;
            T mat21;
            T mat22;
            T mat23;

            T x,y,z;
            T qx,qy,qz,qw;

        private:

            void setRotationWithRotationMatrix(const T xx, const T xy, const T xz, const T yx, const T yy, const T yz, const T zx, const T zy, const T zz)
            {
                T t = xx + yy + zz;

                if (t >= 0)
                {
                    T s = sqrt(t + 1);
                    qw = 0.5 * s;
                    s = 0.5 / s;
                    qx = (yz - zy) * s;
                    qy = (zx - xz) * s;
                    qz = (xy - yx) * s;
                }
                else if ((xx > yy) && (xx > zz))
                {
                    T s = sqrt(1.0 + xx - yy - zz);
                    qx = s * 0.5;
                    s = 0.5 / s;
                    qy = (xy + yx) * s;
                    qz = (zx + xz) * s;
                    qw = (yz - zy) * s;
                }
                else if (yy > zz)
                {
                    T s = sqrt(1.0 + yy - xx - zz);
                    qy = s * 0.5;
                    s = 0.5 / s;
                    qx = (xy + yx) * s;
                    qz = (yz + zy) * s;
                    qw = (zx - xz) * s;
                }
                else
                {
                    T s = sqrt(1.0 + zz - xx - yy);
                    qz = s * 0.5;
                    s = 0.5 / s;
                    qx = (zx + xz) * s;
                    qy = (yz + zy) * s;
                    qw = (xy - yx) * s;
                }
            }
		};

        template<typename TYPE>
        inline RigidBodyTransform<TYPE> operator*(RigidBodyTransform<TYPE> transform1, const RigidBodyTransform<TYPE> &transform2)
        {
            transform1*=transform2;
            return transform1;
        }

        template<typename TYPE>
        inline bool operator==(const RigidBodyTransform<TYPE> &lhs, const RigidBodyTransform<TYPE> &rhs)
        {
            return lhs.epsilonEquals(rhs,1e-10);
        }

        typedef RigidBodyTransform<double> RigidBodyTransform3d;
        typedef RigidBodyTransform<float> RigidBodyTransform3f;
	}
}

#endif
