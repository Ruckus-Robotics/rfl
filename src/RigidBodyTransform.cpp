#include <geometry_utilities/RigidBodyTransform.hpp>
#include <math.h>
#include <algorithm>

/**
 *
 * This class creates a 4x4 rigid body transformation matrix. The top
 * left 3x3 is an orthogonal rotation matrix, while the top right 3x1 is a vector
 * describing a translation.
 *
 * T = | xx yx zx px |
 *     | xy yy zy py |
 *     | xz yz zz pz |
 *     | 0 0 0 1 |
 */

/**
 * Set to identity
 */

namespace geometry_utilities
{
RigidBodyTransform::RigidBodyTransform()
{
	setIdentity();
}

/**
 * Set this transform equal to the RigidBodyTransformed sent as an argument.
 */
RigidBodyTransform::RigidBodyTransform(const RigidBodyTransform& transform)
{
	set(transform);
}

/**
 * Create transformation matrix from Eigen::Matrix4d
 *
 * @param mat4d
 */
RigidBodyTransform::RigidBodyTransform(const Eigen::Matrix4d& matrix)
{
	set(matrix);
}

/**
 * Create transformation matrix from rotation matrix and vector translation
 *
 * @param matrix
 * @param vector
 */
RigidBodyTransform::RigidBodyTransform(const Eigen::Matrix3d& matrix, const Eigen::Vector3d& vector)
{
	set(matrix, vector);
}

/**
 * Create RigidBodyTransform from quaternion describing a rotation and vector
 * describing a translation.
 *
 * @param tf2::Quaternion
 * @param Eigen::Vector3d
 */
RigidBodyTransform::RigidBodyTransform(const tf2::Quaternion& quat, const Eigen::Vector3d& vector)
{
	set(quat, vector);
}

/**
* Construct a RigidBodyTransform from a rotation matrix, setting the translational vector to zero.
*
* @param Eigen::Matrix3d
*/

RigidBodyTransform::RigidBodyTransform(const Eigen::Matrix3d &matrix)
{
	setRotation(matrix);
	setTranslation(0.0, 0.0, 0.0);
}

/**
* Constructs a RigidBodyTransfor with rotation element described by quaternion and
* zero translational component.
*
* @param Quaternion
*/

RigidBodyTransform::RigidBodyTransform(const Quaternion &quat)
{
	setRotation(quat);
	setTranslation(0.0, 0.0, 0.0);
}

/**
* Construct a RigidBodyTransform with rotation element described by axisAngle and
* zero translational component.
*
*/
RigidBodyTransform::RigidBodyTransform(const AxisAngle &axisAngle)
{
	setRotation(axisAngle);
	setTranslation(0.0, 0.0, 0.0);
}

RigidBodyTransform::RigidBodyTransform(const Quaternion &quat, const Eigen::Vector3d& vector)
{
	set(quat, vector);
}

RigidBodyTransform::RigidBodyTransform(const AxisAngle& axisAngle, const Eigen::Vector3d& vector)
{
	set(axisAngle, vector);
}

void RigidBodyTransform::setRotation(const AxisAngle& axisAngle)
{
	setRotationWithAxisAngle(axisAngle.x, axisAngle.y, axisAngle.z, axisAngle.angle);
}

void RigidBodyTransform::setRotationWithAxisAngle(const double& axisAngleX, const double& axisAngleY, const double& axisAngleZ, const double& axisAngleTheta)
{
	double mag = sqrt(axisAngleX * axisAngleX + axisAngleY * axisAngleY + axisAngleZ * axisAngleZ);

	if (almostZero(mag))
	{
		setIdentity();
	}
	else
	{
		mag = 1.0 / mag;
		double ax = axisAngleX * mag;
		double ay = axisAngleY * mag;
		double az = axisAngleZ * mag;

		double sinTheta = sin(axisAngleTheta);
		double cosTheta = cos(axisAngleTheta);
		double t = 1.0 - cosTheta;

		double xz = ax * az;
		double xy = ax * ay;
		double yz = ay * az;

		mat00 = (t * ax * ax + cosTheta);
		mat01 = (t * xy - sinTheta * az);
		mat02 = (t * xz + sinTheta * ay);

		mat10 = (t * xy + sinTheta * az);
		mat11 = (t * ay * ay + cosTheta);
		mat12 = (t * yz - sinTheta * ax);

		mat20 = (t * xz - sinTheta * ay);
		mat21 = (t * yz + sinTheta * ax);
		mat22 = (t * az * az + cosTheta);
	}
}

void RigidBodyTransform::setRotation(const tf2::Quaternion& quat)
{
	setRotationWithQuaternion(quat.getAxis().getX(), quat.getAxis().getY(), quat.getAxis().getZ(), quat.getW());
}

void RigidBodyTransform::setRotation(const Quaternion& quat)
{
	setRotationWithQuaternion(quat.getX(), quat.getY(), quat.getZ(), quat.getW());
}

void RigidBodyTransform::setRotationWithQuaternion(const double& qx, const double& qy, const double& qz, const double& qw)
{
	double yy2 = 2.0 * qy * qy;
	double zz2 = 2.0 * qz * qz;
	double xx2 = 2.0 * qx * qx;
	double xy2 = 2.0 * qx * qy;
	double wz2 = 2.0 * qw * qz;
	double xz2 = 2.0 * qx * qz;
	double wy2 = 2.0 * qw * qy;
	double yz2 = 2.0 * qy * qz;
	double wx2 = 2.0 * qw * qx;

	this->mat00 = (1.0 - yy2 - zz2);
	this->mat01 = (xy2 - wz2);
	this->mat02 = (xz2 + wy2);
	this->mat10 = (xy2 + wz2);
	this->mat11 = (1.0 - xx2 - zz2);
	this->mat12 = (yz2 - wx2);
	this->mat20 = (xz2 - wy2);
	this->mat21 = (yz2 + wx2);
	this->mat22 = (1.0 - xx2 - yy2);
}

/**
 * Set the 3x3 rotation matrix equal to mat3d.
 *
 * @param matrix
 */
void RigidBodyTransform::setRotation(const Eigen::Matrix3d& matrix)
{
	this->mat00 = matrix(0, 0);
	this->mat01 = matrix(0, 1);
	this->mat02 = matrix(0, 2);
	this->mat10 = matrix(1, 0);
	this->mat11 = matrix(1, 1);
	this->mat12 = matrix(1, 2);
	this->mat20 = matrix(2, 0);
	this->mat21 = matrix(2, 1);
	this->mat22 = matrix(2, 2);
}

/**
 * Set translational portion of the transformation matrix
 *
 * @param vector
 */
void RigidBodyTransform::setTranslation(const Eigen::Vector3d& vector)
{
	setTranslation(vector(0), vector(1), vector(2));
}

void RigidBodyTransform::setTranslation(const double& x, const double& y, const double& z)
{
	this->mat03 = x;
	this->mat13 = y;
	this->mat23 = z;
}

/**
*  Add a translation to the current transform. It is equivalent to:
*
*      transform.setTranslationAndIdentityRotation(translation);
*      this = this*transform
*/
void RigidBodyTransform::applyTranslation(const Eigen::Vector3d& translation)
{
	Point3d temp(translation);
	transform(temp);
	mat03 = temp.x;
	mat13 = temp.y;
	mat23 = temp.z;
}

/**
*  Add a rotation to the current transform.
*/
void RigidBodyTransform::applyRotationX(const double& angle)
{
	RigidBodyTransform temp;
	temp.rotX(angle);
	multiply(temp);
}

/**
*  Add a rotation to the current transform.
*/
void RigidBodyTransform::applyRotationY(const double &angle)
{
	RigidBodyTransform temp;
	temp.rotY(angle);
	multiply(temp);
}

/**
*  Add a rotation to the current transform.
*/
void RigidBodyTransform::applyRotationZ(const double &angle)
{
	RigidBodyTransform temp;
	temp.rotZ(angle);
	multiply(temp);
}

// /**
//  * Set translational portion of the transformation matrix
//  *
//  * @param vec3d
//  */
// void setTranslation(Vector3f vector)
// {
//  setTranslation(vector.x, vector.y, vector.z);
// }

// /**
//  * Set elements of this transform equal to the elements of transform.
//  *
//  * @param transform
//  */
void RigidBodyTransform::set(const RigidBodyTransform& transform)
{
	this->mat00 = transform.mat00;
	this->mat01 = transform.mat01;
	this->mat02 = transform.mat02;
	this->mat03 = transform.mat03;
	this->mat10 = transform.mat10;
	this->mat11 = transform.mat11;
	this->mat12 = transform.mat12;
	this->mat13 = transform.mat13;
	this->mat20 = transform.mat20;
	this->mat21 = transform.mat21;
	this->mat22 = transform.mat22;
	this->mat23 = transform.mat23;
}

/**
 * Set this transform to have translation described in vector
 * and a rotation equal to the Eigen::Matrix3d matrix.
 *
 * @param Eigen::Matrix3d matrix
 * @param Eigen::Vector3d vector
 */
void RigidBodyTransform::set(const Eigen::Matrix3d& matrix, const Eigen::Vector3d& vector)
{
	setRotation(matrix);
	setTranslation(vector);
}

/**
 * Set this transform to have zero translation and a rotation equal to the
 * tf2::Quaternion quat.
 *
 * @param tf2::Quaternion quat
 */
void RigidBodyTransform::setRotationAndZeroTranslation(const tf2::Quaternion &quat)
{
	setRotation(quat);
	setTranslation(0, 0, 0);
}

/**
 * Set this transform to have zero translation and a rotation equal to the
 * Quaternion quat.
 *
 * @param Quaternion quat
 */
void RigidBodyTransform::setRotationAndZeroTranslation(const Quaternion &quat)
{
	setRotation(quat);
	setTranslation(0, 0, 0);
}

// /**
//  * Set this transform to have translation described in vector and a rotation
//  * equal to the tf2::Quaternion quat.
//  *
//  * @param tf2::Quaternion quat
//  */
void RigidBodyTransform::set(const tf2::Quaternion& quat, const Eigen::Vector3d& vector)
{
	setRotation(quat);
	setTranslation(vector);
}

// /**
//  * Set this transform to have translation described in vector and a rotation
//  * equal to the tf2::Quaternion quat.
//  *
//  * @param Quaternion quat
//  */
void RigidBodyTransform::set(const Quaternion& quat, const Eigen::Vector3d& vector)
{
	setRotation(quat);
	setTranslation(vector);
}

/**
 * Sets this transform to have rotation described by axisAngle and zero
 * translation.
 *
 * @param axisAngle
 */
void RigidBodyTransform::setRotationAndZeroTranslation(const AxisAngle &axisAngle)
{
	setRotation(axisAngle);
	setTranslation(0, 0, 0);
}

void RigidBodyTransform::set(const AxisAngle& axisAngle, const Eigen::Vector3d& vector)
{
	setRotation(axisAngle);
	setTranslation(vector(0), vector(1), vector(2));
}

/**
 * Set this transform to have zero translation
 */
void RigidBodyTransform::zeroTranslation()
{
	setTranslation(0, 0, 0);
}

/**
 * Set this transform to have zero translation and a rotation equal to the
 * Matrix3f matrix.
 *
 * @param matrix
 */
void RigidBodyTransform::setRotationAndZeroTranslation(const Eigen::Matrix3d& matrix)
{
	setRotation(matrix);
	setTranslation(0, 0, 0);
}

/**
 * Set this transform to have an identity rotation and a translation given
 * by the Eigen::Vector3d vector.
 *
 * @param vector
 */
void RigidBodyTransform::setTranslationAndIdentityRotation(const Eigen::Vector3d& vector)
{
	setTranslation(vector(0), vector(1), vector(2));
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
 * Sets rotation to the identity, does not effect the translational component of the Transform
 *
 * @param vector
 */
void RigidBodyTransform::setRotationToIdentity()
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
 * Set elements of transform equal to elements of the Eigen::Matrix4d.
 *
 * @param matrix
 */
void RigidBodyTransform::set(const Eigen::Matrix4d& matrix)
{
	this->mat00 = matrix(0, 0);
	this->mat01 = matrix(0, 1);
	this->mat02 = matrix(0, 2);
	this->mat03 = matrix(0, 3);
	this->mat10 = matrix(1, 0);
	this->mat11 = matrix(1, 1);
	this->mat12 = matrix(1, 2);
	this->mat13 = matrix(1, 3);
	this->mat20 = matrix(2, 0);
	this->mat21 = matrix(2, 1);
	this->mat22 = matrix(2, 2);
	this->mat23 = matrix(2, 3);
}

/**
 * This method is for when the Eigen::Matrix4d matrix is column major and needs to
 * be transposed.
 *
 * @param matrix
 */
void RigidBodyTransform::setAsTranspose(const Eigen::Matrix4d& matrix)
{
	double tmp10 = matrix(1, 0);
	double tmp20 = matrix(2, 0);
	double tmp21 = matrix(2, 1);
	double tmp30 = matrix(3, 0);
	double tmp31 = matrix(3, 1);
	double tmp32 = matrix(3, 2);

	mat00 = matrix(0, 0);
	mat11 = matrix(1, 1);
	mat22 = matrix(2, 2);
	mat10 = matrix(0, 1);
	mat20 = matrix(0, 2);
	mat21 = matrix(1, 2);
	mat01 = tmp10;
	mat03 = tmp30;
	mat13 = tmp31;
	mat23 = tmp32;
	mat02 = tmp20;
	mat12 = tmp21;
}

/**
 * Set transformation matrix to Identity, meaning no rotation or
 * translation.
 */
void RigidBodyTransform::setIdentity()
{
	this->mat00 = 1.0;
	this->mat01 = 0.0;
	this->mat02 = 0.0;
	this->mat03 = 0.0;
	this->mat10 = 0.0;
	this->mat11 = 1.0;
	this->mat12 = 0.0;
	this->mat13 = 0.0;
	this->mat20 = 0.0;
	this->mat21 = 0.0;
	this->mat22 = 1.0;
	this->mat23 = 0.0;
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
void RigidBodyTransform::setEuler(const Eigen::Vector3d& vector)
{
	setEuler(vector(0), vector(1), vector(2));
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
void RigidBodyTransform::setEuler(const double& rotX, const double& rotY, const double& rotZ)
{
	double sina = sin(rotX);
	double sinb = sin(rotY);
	double sinc = sin(rotZ);
	double cosa = cos(rotX);
	double cosb = cos(rotY);
	double cosc = cos(rotZ);

	this->mat00 = cosb * cosc;
	this->mat01 = -(cosa * sinc) + (sina * sinb * cosc);
	this->mat02 = (sina * sinc) + (cosa * sinb * cosc);
	this->mat10 = cosb * sinc;
	this->mat11 = (cosa * cosc) + (sina * sinb * sinc);
	this->mat12 = -(sina * cosc) + (cosa * sinb * sinc);
	this->mat20 = -sinb;
	this->mat21 = sina * cosb;
	this->mat22 = cosa * cosb;
	this->mat03 = 0.0;
	this->mat13 = 0.0;
	this->mat23 = 0.0;
}

/**
 * Computes the RPY angles from the rotation matrix for rotations about the
 * X, Y, and Z axes respectively. Note that this method is here for the
 * purpose of unit testing the method setEuler. This particular solution is
 * only valid for -pi/2 < vector.y < pi/2 and for vector.y != 0.
 *
 * @param vector
 */
void RigidBodyTransform::getEulerXYZ(Eigen::Vector3d& vector) const
{
	vector(0) = atan2(mat21, mat22);
	vector(1) = atan2(-mat20, sqrt(mat21 * mat21 + mat22 * mat22));
	vector(2) = atan2(mat10, mat00);
}

/**
 * Return rotation matrix of type Eigen::Matrix3d
 *
 * @param matrix
 */
void RigidBodyTransform::getRotation(Eigen::Matrix3d& matrix) const
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
 * Return rotation in quaternion form.
 *
 * @param tf2::Quaternion quat
 */
void RigidBodyTransform::getRotation(tf2::Quaternion& quat) const
{
	double trace = mat00 + mat11 + mat22;
	double val;

	double x, y, z, w;

	if (trace > 0.0)
	{
		val = sqrt(trace + 1.0) * 2.0;
		x = (mat21 - mat12) / val;
		y = (mat02 - mat20) / val;
		z = (mat10 - mat01) / val;
		w = 0.25 * val;
	}
	else
		if (mat11 > mat22)
		{
			double temp = std::max(0.0, 1.0 + mat11 - mat00 - mat22);
			val = sqrt(temp) * 2.0;
			x = (mat01 + mat10) / val;
			y = 0.25 * val;
			z = (mat12 + mat21) / val;
			w = (mat02 - mat20) / val;
		}
		else
			if ((mat00 > mat11) && (mat00 > mat22))
			{
				val = sqrt(1.0 + mat00 - mat11 - mat22) * 2.0;
				x = 0.25 * val;
				y = (mat01 + mat10) / val;
				z = (mat02 + mat20) / val;
				w = (mat21 - mat12) / val;
			}
			else
			{
				val = sqrt(1.0 + mat22 - mat00 - mat11) * 2.0;
				x = (mat02 + mat20) / val;
				y = (mat12 + mat21) / val;
				z = 0.25 * val;
				w = (mat10 - mat01) / val;
			}

	tf2::Quaternion tmpQuat(x, y, z, w);
	tmpQuat.normalize();
	quat = tmpQuat;
}

/**
 * Return rotation in quaternion form.
 *
 * @param Quaternion quat
 */
void RigidBodyTransform::getRotation(Quaternion& quat) const
{
	double trace = mat00 + mat11 + mat22;
	double val;

	double x, y, z, w;

	if (trace > 0.0)
	{
		val = sqrt(trace + 1.0) * 2.0;
		x = (mat21 - mat12) / val;
		y = (mat02 - mat20) / val;
		z = (mat10 - mat01) / val;
		w = 0.25 * val;
	}
	else
		if (mat11 > mat22)
		{
			double temp = std::max(0.0, 1.0 + mat11 - mat00 - mat22);
			val = sqrt(temp) * 2.0;
			x = (mat01 + mat10) / val;
			y = 0.25 * val;
			z = (mat12 + mat21) / val;
			w = (mat02 - mat20) / val;
		}
		else
			if ((mat00 > mat11) && (mat00 > mat22))
			{
				val = sqrt(1.0 + mat00 - mat11 - mat22) * 2.0;
				x = 0.25 * val;
				y = (mat01 + mat10) / val;
				z = (mat02 + mat20) / val;
				w = (mat21 - mat12) / val;
			}
			else
			{
				val = sqrt(1.0 + mat22 - mat00 - mat11) * 2.0;
				x = (mat02 + mat20) / val;
				y = (mat12 + mat21) / val;
				z = 0.25 * val;
				w = (mat10 - mat01) / val;
			}

	quat.set(x, y, z, w);
	quat.normalize();
}

/**
 * Return rotation in AxisAngle form.
 *
 * @param axisAngle
 */
void RigidBodyTransform::getRotation(AxisAngle &axisAngle) const
{
	getRotation(axisAngle, 1.0e-12);
}

void RigidBodyTransform::getRotation(AxisAngle &axisAngle, const double &epsilon) const
{
	axisAngle.x = mat21 - mat12;
	axisAngle.y = mat02 - mat20;
	axisAngle.z = mat10 - mat01;
	double mag = axisAngle.x * axisAngle.x + axisAngle.y * axisAngle.y + axisAngle.z * axisAngle.z;

	if (mag > epsilon)
	{
		mag = sqrt(mag);
		double sin = 0.5 * mag;
		double cos = 0.5 * (mat00 + mat11 + mat22 - 1.0);

		axisAngle.angle = atan2(sin, cos);

		double invMag = 1.0 / mag;
		axisAngle.x = axisAngle.x * invMag;
		axisAngle.y = axisAngle.y * invMag;
		axisAngle.z = axisAngle.z * invMag;
	}
	else
	{
		if (isRotationMatrixEpsilonIdentity(10.0 * epsilon))
		{
			axisAngle.set(0.0, 1.0, 0.0, 0.0);
			return;
		}
		else
		{
			axisAngle.angle = M_PI;

			double xx = (mat00 + 1.0) / 2.0;
			double yy = (mat11 + 1.0) / 2.0;
			double zz = (mat22 + 1.0) / 2.0;
			double xy = (mat01 + mat10) / 4.0;
			double xz = (mat02 + mat20) / 4.0;
			double yz = (mat12 + mat21) / 4.0;
			double cos45 = cos(M_PI / 4.0);

			if ((xx > yy) && (xx > zz))
			{
				// mat00 is the largest diagonal term
				if (xx < epsilon)
				{
					axisAngle.x = 0.0;
					axisAngle.y = cos45;
					axisAngle.z = cos45;
				}
				else
				{
					axisAngle.x = sqrt(xx);
					axisAngle.y = xy / axisAngle.x;
					axisAngle.z = xz / axisAngle.x;
				}
			}
			else
				if (yy > zz)
				{
					// mat11 is the largest diagonal term
					if (yy < epsilon)
					{
						axisAngle.x = cos45;
						axisAngle.y = 0.0;
						axisAngle.z = cos45;
					}
					else
					{
						axisAngle.y = sqrt(yy);
						axisAngle.x = xy / axisAngle.y;
						axisAngle.z = yz / axisAngle.y;
					}
				}
				else
				{
					// mat22 is the largest diagonal term
					if (zz < epsilon)
					{
						axisAngle.x = cos45;
						axisAngle.y = cos45;
						axisAngle.z = 0.0;
					}
					else
					{
						axisAngle.z = sqrt(zz);
						axisAngle.x = xz / axisAngle.z;
						axisAngle.y = yz / axisAngle.z;
					}
				}
		}
	}
}

bool RigidBodyTransform::isRotationMatrixSingular(double epsilon) const
{
	return (fabs(mat01 - mat10) < epsilon) && (fabs(mat02 - mat20) < epsilon) && (fabs(mat12 - mat21) < epsilon);
}

bool RigidBodyTransform::isRotationMatrixEpsilonIdentity(const double& epsilon) const
{
	return (fabs(mat01 + mat10) < epsilon) && (fabs(mat02 + mat20) < epsilon) && (fabs(mat12 + mat21) < epsilon)
	       && (fabs(mat00 + mat11 + mat22 - 3) < epsilon);
}

/**
 * Return translational part as Eigen::Vector3d
 *
 * @param vector
 */
void RigidBodyTransform::getTranslation(Eigen::Vector3d& vector) const
{
	vector(0) = mat03;
	vector(1) = mat13;
	vector(2) = mat23;
}

/**
 * Return translational part as Point3d
 *
 * @param point
 */
void RigidBodyTransform::getTranslation(Point3d& point) const
{
	point.x = mat03;
	point.y = mat13;
	point.z = mat23;
}

/**
* Pack transform into Eigen::Matrix4d
*
* @param matrix
*/
void RigidBodyTransform::get(Eigen::Matrix4d &matrix) const
{
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

/**
 * Pack rotation part into Eigen::Matrix3d and translation part into Eigen::Vector3d
 *
 * @param matrix
 * @param vector
 */
void RigidBodyTransform::get(Eigen::Matrix3d& matrix, Eigen::Vector3d& vector) const
{
	getRotation(matrix);
	getTranslation(vector);
}

/**
 * Return rotation portion of this transform.
 *
 * @param matrix
 */
void RigidBodyTransform::get(Eigen::Matrix3d& matrix) const
{
	getRotation(matrix);
}

/**
 * Return translational portion of this transform.
 *
 * @param vector
 */
void RigidBodyTransform::get(Eigen::Vector3d& vector) const
{
	getTranslation(vector);
}

/**
 * Convert and pack rotation part of transform into tf2::Quaternion and pack
 * translation into Eigen::Vector3d.
 *
 * @param quat
 * @param vector
 */
void RigidBodyTransform::get(tf2::Quaternion& quat, Eigen::Vector3d& vector) const
{
	getRotation(quat);
	getTranslation(vector);
}

/**
 * Convert and pack rotation part of transform into tf2::Quaternion and pack
 * translation into Point3d.
 *
 * @param quat
 * @param point
 */
void RigidBodyTransform::get(tf2::Quaternion& quat, Point3d& point) const
{
	getRotation(quat);
	getTranslation(point);
}

/**
 * Convert and pack rotation part of transform into tf2::Quaternion.
 *
 * @param quat
 * @param vector
 */
void RigidBodyTransform::get(tf2::Quaternion& quat) const
{
	getRotation(quat);
}

/**
 * Multiplies this RigidBodyTransform by transform and stores the result in this,
 * i.e. this = this*transform
 *
 * @param transform
 */
void RigidBodyTransform::multiply(const RigidBodyTransform& transform)
{
	double tmp00 = mat00 * transform.mat00 + mat01 * transform.mat10 + mat02 * transform.mat20;
	double tmp01 = mat00 * transform.mat01 + mat01 * transform.mat11 + mat02 * transform.mat21;
	double tmp02 = mat00 * transform.mat02 + mat01 * transform.mat12 + mat02 * transform.mat22;
	double tmp03 = mat00 * transform.mat03 + mat01 * transform.mat13 + mat02 * transform.mat23 + mat03;

	double tmp10 = mat10 * transform.mat00 + mat11 * transform.mat10 + mat12 * transform.mat20;
	double tmp11 = mat10 * transform.mat01 + mat11 * transform.mat11 + mat12 * transform.mat21;
	double tmp12 = mat10 * transform.mat02 + mat11 * transform.mat12 + mat12 * transform.mat22;
	double tmp13 = mat10 * transform.mat03 + mat11 * transform.mat13 + mat12 * transform.mat23 + mat13;

	double tmp20 = mat20 * transform.mat00 + mat21 * transform.mat10 + mat22 * transform.mat20;
	double tmp21 = mat20 * transform.mat01 + mat21 * transform.mat11 + mat22 * transform.mat21;
	double tmp22 = mat20 * transform.mat02 + mat21 * transform.mat12 + mat22 * transform.mat22;
	double tmp23 = mat20 * transform.mat03 + mat21 * transform.mat13 + mat22 * transform.mat23 + mat23;

	mat00 = tmp00;
	mat01 = tmp01;
	mat02 = tmp02;
	mat03 = tmp03;
	mat10 = tmp10;
	mat11 = tmp11;
	mat12 = tmp12;
	mat13 = tmp13;
	mat20 = tmp20;
	mat21 = tmp21;
	mat22 = tmp22;
	mat23 = tmp23;
}

/**
 * Multiplies transform1 and transform and puts result into this. this =
 * transform1*transform
 *
 * @param transform1
 * @param transform
 */
void RigidBodyTransform::multiply(const RigidBodyTransform& transform1, const RigidBodyTransform& transform)
{
	double tmp00 = transform1.mat00 * transform.mat00 + transform1.mat01 * transform.mat10 + transform1.mat02 * transform.mat20;
	double tmp01 = transform1.mat00 * transform.mat01 + transform1.mat01 * transform.mat11 + transform1.mat02 * transform.mat21;
	double tmp02 = transform1.mat00 * transform.mat02 + transform1.mat01 * transform.mat12 + transform1.mat02 * transform.mat22;
	double tmp03 = transform1.mat00 * transform.mat03 + transform1.mat01 * transform.mat13 + transform1.mat02 * transform.mat23 + transform1.mat03;

	double tmp10 = transform1.mat10 * transform.mat00 + transform1.mat11 * transform.mat10 + transform1.mat12 * transform.mat20;
	double tmp11 = transform1.mat10 * transform.mat01 + transform1.mat11 * transform.mat11 + transform1.mat12 * transform.mat21;
	double tmp12 = transform1.mat10 * transform.mat02 + transform1.mat11 * transform.mat12 + transform1.mat12 * transform.mat22;
	double tmp13 = transform1.mat10 * transform.mat03 + transform1.mat11 * transform.mat13 + transform1.mat12 * transform.mat23 + transform1.mat13;

	double tmp20 = transform1.mat20 * transform.mat00 + transform1.mat21 * transform.mat10 + transform1.mat22 * transform.mat20;
	double tmp21 = transform1.mat20 * transform.mat01 + transform1.mat21 * transform.mat11 + transform1.mat22 * transform.mat21;
	double tmp22 = transform1.mat20 * transform.mat02 + transform1.mat21 * transform.mat12 + transform1.mat22 * transform.mat22;
	double tmp23 = transform1.mat20 * transform.mat03 + transform1.mat21 * transform.mat13 + transform1.mat22 * transform.mat23 + transform1.mat23;

	mat00 = tmp00;
	mat01 = tmp01;
	mat02 = tmp02;
	mat03 = tmp03;
	mat10 = tmp10;
	mat11 = tmp11;
	mat12 = tmp12;
	mat13 = tmp13;
	mat20 = tmp20;
	mat21 = tmp21;
	mat22 = tmp22;
	mat23 = tmp23;
}

/**
 * Compute the inverse of the RigidBodyTransform passed in as an
 * argument exploiting the orthogonality of the rotation matrix
 * and store the result in this.
 * @param transform
 */
void RigidBodyTransform::invert(RigidBodyTransform transform)
{
	set(transform);
	invert();
}

/**
* Inverte this transform.
*/
void RigidBodyTransform::invert()
{
	invertOrthogonal();
}

/**
 * Invert this assuming an orthogonal rotation portion.
 */
void RigidBodyTransform::invertOrthogonal()
{
	double tmp01 = mat01;
	double tmp02 = mat02;
	double tmp12 = mat12;

// For orthogonal matrix, R^{-1} = R^{T}
	mat01 = mat10;
	mat02 = mat20;
	mat12 = mat21;
	mat10 = tmp01;
	mat20 = tmp02;
	mat21 = tmp12;

// New translation vector becomes -R^{T} * p
	double newTransX = -(mat23 * mat02 + mat00 * mat03 + mat01 * mat13);
	double newTransY = -(mat03 * mat10 + mat23 * mat12 + mat11 * mat13);
	mat23 = -(mat22 * mat23 + mat03 * mat20 + mat13 * mat21);
	mat03 = newTransX;
	mat13 = newTransY;
}

void RigidBodyTransform::invertRotationButKeepTranslation()
{
	double tmp01 = mat01;
	double tmp02 = mat02;
	double tmp12 = mat12;

// For orthogonal matrix, R^{-1} = R^{T}
	mat01 = mat10;
	mat02 = mat20;
	mat12 = mat21;
	mat10 = tmp01;
	mat20 = tmp02;
	mat21 = tmp12;
}

/**
 * Create RigidBodyTransform with zero translation and the rotation matrix being a
 * rotation about the x-axis by angle.
 *
 * @param angle
 */
void RigidBodyTransform::rotX(const double& angle)
{
	double cosAngle = cos(angle);
	double sinAngle = sin(angle);

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
void RigidBodyTransform::rotY(const double& angle)
{
	double cosAngle = cos(angle);
	double sinAngle = sin(angle);

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
void RigidBodyTransform::rotZ(const double& angle)
{
	double cosAngle = cos(angle);
	double sinAngle = sin(angle);

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
 * Check if the elements of this are within epsilon of the elements of
 * transform.
 *
 * @param transform
 * @param epsilon
 * @return
 */
bool RigidBodyTransform::epsilonEquals(const RigidBodyTransform& transform, const double& epsilon) const
{
	if (!fabs(mat00 - transform.mat00) < epsilon)
	{
		return false;
	}

	if (!fabs(mat01 - transform.mat01) < epsilon)
	{
		return false;
	}

	if (!fabs(mat02 - transform.mat02) < epsilon)
	{
		return false;
	}

	if (!fabs(mat03 - transform.mat03) < epsilon)
	{
		return false;
	}

	if (!fabs(mat10 - transform.mat10) < epsilon)
	{
		return false;
	}

	if (!fabs(mat11 - transform.mat11) < epsilon)
	{
		return false;
	}

	if (!fabs(mat12 - transform.mat12) < epsilon)
	{
		return false;
	}

	if (!fabs(mat13 - transform.mat13) < epsilon)
	{
		return false;
	}

	if (!fabs(mat20 - transform.mat20) < epsilon)
	{
		return false;
	}

	if (!fabs(mat21 - transform.mat21) < epsilon)
	{
		return false;
	}

	if (!fabs(mat22 - transform.mat22) < epsilon)
	{
		return false;
	}

	if (!fabs(mat23 - transform.mat23) < epsilon)
	{
		return false;
	}

	return true;
}

/**
 * Returns true if each element of this is equal to each element of
 * transform within a default tolerance of 1e-10.
 *
 * @param transform
 * @return
 */
bool RigidBodyTransform::equals(const RigidBodyTransform& transform) const
{
	return epsilonEquals(transform, 1e-10);
}

/**
 * Transform vector by multiplying it by this transform and put result back
 * into vector.
 *
 * @param vector
 */
void RigidBodyTransform::transform(Eigen::Vector4d& vector)
{
	if (vector(3) != 1.0)
	{
		throw std::runtime_error("Final element of vector must be 1.");
	}

	double x = mat00 * vector(0) + mat01 * vector(1) + mat02 * vector(2) + mat03;
	double y = mat10 * vector(0) + mat11 * vector(1) + mat12 * vector(2) + mat13;
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
void RigidBodyTransform::transform(Eigen::Vector3d vector)
{
	double x = mat00 * vector(0) + mat01 * vector(1) + mat02 * vector(2);
	double y = mat10 * vector(0) + mat11 * vector(1) + mat12 * vector(2);
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
void RigidBodyTransform::transform(const Eigen::Vector3d& vectorIn, Eigen::Vector3d& vectorOut)
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
void RigidBodyTransform::transform(const Eigen::Vector4d& vectorIn, Eigen::Vector4d& vectorOut)
{
	vectorOut(0) = mat00 * vectorIn(0) + mat01 * vectorIn(1) + mat02 * vectorIn(2) + mat03;
	vectorOut(1) = mat10 * vectorIn(0) + mat11 * vectorIn(1) + mat12 * vectorIn(2) + mat13;
	vectorOut(2) = mat20 * vectorIn(0) + mat21 * vectorIn(1) + mat22 * vectorIn(2) + mat23;
	vectorOut(3) = 1.0;
}

// /**
//  * Transform the Point3d point by this transform and place result back in
//  * point.
//  *
//  * @param point
//  */
void RigidBodyTransform::transform(Point3d& point)
{
	double x = mat00 * point.x + mat01 * point.y + mat02 * point.z + mat03;
	double y = mat10 * point.x + mat11 * point.y + mat12 * point.z + mat13;
	point.z = mat20 * point.x + mat21 * point.y + mat22 * point.z + mat23;

	point.x = x;
	point.y = y;
}

/**
 * Transform the Point3d pointIn by this transform and place result in
 * pointOut.
 *
 * @param point
 */
void RigidBodyTransform::transform(Point3d pointIn, Point3d pointOut)
{
	pointOut.x = mat00 * pointIn.x + mat01 * pointIn.y + mat02 * pointIn.z + mat03;
	pointOut.y = mat10 * pointIn.x + mat11 * pointIn.y + mat12 * pointIn.z + mat13;
	pointOut.z = mat20 * pointIn.x + mat21 * pointIn.y + mat22 * pointIn.z + mat23;
}

/**
 * Return the determinant of this transform.
 *
 * @return
 */
double RigidBodyTransform::determinant() const
{
	return (mat00 * (mat11 * mat22 - mat12 * mat21) - mat01 * (mat10 * mat22 - mat12 * mat20) + mat02 * (mat10 * mat21 - mat11 * mat20));
}

/**
 * Orthonormalization of the rotation matrix using Gram-Schmidt method.
 */
void RigidBodyTransform::normalize()
{
	double xdoty = mat00 * mat01 + mat10 * mat11 + mat20 * mat21;
	double xdotx = mat00 * mat00 + mat10 * mat10 + mat20 * mat20;
	double tmp = xdoty / xdotx;

	mat01 -= tmp * mat00;
	mat11 -= tmp * mat10;
	mat21 -= tmp * mat20;

	double zdoty = mat02 * mat01 + mat12 * mat11 + mat22 * mat21;
	double zdotx = mat02 * mat00 + mat12 * mat10 + mat22 * mat20;
	double ydoty = mat01 * mat01 + mat11 * mat11 + mat21 * mat21;

	tmp = zdotx / xdotx;
	double tmp1 = zdoty / ydoty;

	mat02 = mat02 - (tmp * mat00 + tmp1 * mat01);
	mat12 = mat12 - (tmp * mat10 + tmp1 * mat11);
	mat22 = mat22 - (tmp * mat20 + tmp1 * mat21);

// Compute orthogonalized vector magnitudes and normalize
	double magX = sqrt(mat00 * mat00 + mat10 * mat10 + mat20 * mat20);
	double magY = sqrt(mat01 * mat01 + mat11 * mat11 + mat21 * mat21);
	double magZ = sqrt(mat02 * mat02 + mat12 * mat12 + mat22 * mat22);

	mat00 = mat00 / magX;
	mat10 = mat10 / magX;
	mat20 = mat20 / magX;
	mat01 = mat01 / magY;
	mat11 = mat11 / magY;
	mat21 = mat21 / magY;
	mat02 = mat02 / magZ;
	mat12 = mat12 / magZ;
	mat22 = mat22 / magZ;
}

bool RigidBodyTransform::almostZero(const double& a)
{
	return ((a < 1.0e-5) && (a > -1.0e-5));
}

Eigen::Vector3d RigidBodyTransform::getTranslationDifference( const RigidBodyTransform& transform1, const RigidBodyTransform& transform2 )
{
	Eigen::Vector3d pos1;
	Eigen::Vector3d pos2;
	transform1.getTranslation(pos1);
	transform2.getTranslation(pos2);

	return (pos2 - pos1);
}
}