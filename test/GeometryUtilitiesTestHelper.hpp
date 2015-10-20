#include "geometry_utilities/Tuple3d.hpp"
#include <random>
#include <memory>
#include <math.h>
#include <eigen3/Eigen/Eigen>
#include "geometry_utilities/AxisAngle.hpp"
#include <tf2/LinearMath/Quaternion.h>
#include "geometry_utilities/Quaternion.hpp"

namespace geometry_utilities
{

class GeometryUtilitiesTestHelper
{
	public:
		static double getRandomDouble()
		{
			std::srand( time(NULL) );
			// Generates random double between -1000 & 1000
			return (2000.0 * rand() / RAND_MAX - 1000);
		}

		static double getRandomAngle()
		{
			std::srand( time(NULL) );
			return 2 * (M_PI - 0.01) * rand() / RAND_MAX - (M_PI - 0.01);
		}

		static std::vector<double> getRandom3dVector()
		{
			std::vector<double> vector(3);
			vector[0] = getRandomDouble();
			vector[1] = getRandomDouble();
			vector[2] = getRandomDouble();

			return vector;
		}

		static Tuple3d getRandomTuple3d()
		{
			Tuple3d tuple;
			tuple.setX(getRandomDouble());
			tuple.setY(getRandomDouble());
			tuple.setZ(getRandomDouble());

			return tuple;
		}

		static Eigen::Matrix3d createRandomRotationMatrix()
		{
			Eigen::Matrix3d rotX = createRandomRotationMatrixX();
			Eigen::Matrix3d rotY = createRandomRotationMatrixY();
			Eigen::Matrix3d rotZ = createRandomRotationMatrixZ();

			return rotX * rotY * rotZ;
		}

		static Eigen::Matrix3d createRandomRotationMatrixX()
		{
			Eigen::Matrix3d rotX;
			rotX(0, 0) = 1;
			rotX(0, 1) = 0;
			rotX(0, 2) = 0;
			rotX(1, 0) = 0;
			rotX(2, 0) = 0;

			std::srand( time(NULL) );
			double angle = (2 * M_PI * rand() / RAND_MAX - M_PI);
			rotX(1, 1) = cos(angle);
			rotX(2, 2) = cos(angle);
			rotX(1, 2) = -sin(angle);
			rotX(2, 1) = sin(angle);

			return rotX;
		}

		static Eigen::Matrix3d createRandomRotationMatrixY()
		{
			Eigen::Matrix3d rotY;
			rotY(2, 1) = 0;
			rotY(0, 1) = 0;
			rotY(1, 0) = 0;
			rotY(1, 1) = 1;
			rotY(1, 2) = 0;

			std::srand( time(NULL) );
			double angle = (2 * M_PI * rand() / RAND_MAX - M_PI);
			rotY(0, 0) = cos(angle);
			rotY(2, 2) = cos(angle);
			rotY(2, 0) = -sin(angle);
			rotY(0, 2) = sin(angle);

			return rotY;
		}

		static Eigen::Matrix3d createRandomRotationMatrixZ()
		{
			Eigen::Matrix3d rotZ;
			rotZ(0, 2) = 0;
			rotZ(1, 2) = 0;
			rotZ(2, 0) = 0;
			rotZ(2, 1) = 0;
			rotZ(2, 2) = 1;

			std::srand( time(NULL) );
			double angle = (2 * M_PI * rand() / RAND_MAX - M_PI);
			rotZ(0, 0) = cos(angle);
			rotZ(1, 1) = cos(angle);
			rotZ(0, 1) = -sin(angle);
			rotZ(1, 0) = sin(angle);

			return rotZ;
		}

		static Eigen::Matrix4d createRandomTransformationMatrix()
		{
			Eigen::Matrix3d rotationMatrix = createRandomRotationMatrix();

			Eigen::Matrix4d transform;

			transform(0, 0) = rotationMatrix(0, 0);
			transform(0, 1) = rotationMatrix(0, 1);
			transform(0, 2) = rotationMatrix(0, 2);
			transform(1, 0) = rotationMatrix(1, 0);
			transform(1, 1) = rotationMatrix(1, 1);
			transform(1, 2) = rotationMatrix(1, 2);
			transform(2, 0) = rotationMatrix(2, 0);
			transform(2, 1) = rotationMatrix(2, 1);
			transform(2, 2) = rotationMatrix(2, 2);

			transform(0, 3) = getRandomDouble();
			transform(1, 3) = getRandomDouble();
			transform(2, 3) = getRandomDouble();

			transform(3, 3) = 1.0;

			return transform;
		}

		static Eigen::Matrix4d createRandomMatrix4d()
		{
			Eigen::Matrix4d matrix;

			std::srand( time(NULL) );

			matrix(0, 0) = (0.5 * rand() / RAND_MAX - 0.25);
			matrix(0, 1) = (0.5 * rand() / RAND_MAX - 0.25);
			matrix(0, 2) = (0.5 * rand() / RAND_MAX - 0.25);
			matrix(0, 3) = (0.5 * rand() / RAND_MAX - 0.25);
			matrix(1, 0) = (0.5 * rand() / RAND_MAX - 0.25);
			matrix(1, 1) = (0.5 * rand() / RAND_MAX - 0.25);
			matrix(1, 2) = (0.5 * rand() / RAND_MAX - 0.25);
			matrix(1, 3) = (0.5 * rand() / RAND_MAX - 0.25);
			matrix(2, 0) = (0.5 * rand() / RAND_MAX - 0.25);
			matrix(2, 1) = (0.5 * rand() / RAND_MAX - 0.25);
			matrix(2, 2) = (0.5 * rand() / RAND_MAX - 0.25);
			matrix(2, 3) = (0.5 * rand() / RAND_MAX - 0.25);
			matrix(3, 0) = 0;
			matrix(3, 1) = 0;
			matrix(3, 2) = 0;
			matrix(3, 3) = 1;

			return matrix;
		}

		static bool checkOrthogonality(Eigen::Matrix4d matrix)
		{
			bool xMag = (1.0 - (sqrt(pow(matrix(0, 0), 2) + pow(matrix(1, 0), 2) + pow(matrix(2, 0), 2))) < 1e-8);
			bool yMag = (1.0 - (sqrt(pow(matrix(0, 1), 2) + pow(matrix(1, 1), 2) + pow(matrix(2, 1), 2))) < 1e-8);
			bool zMag = (1.0 - (sqrt(pow(matrix(0, 2), 2) + pow(matrix(1, 2), 2) + pow(matrix(2, 2), 2))) < 1e-8);

			return (xMag && yMag && zMag);
		}

		static AxisAngle createRandomAxisAngle()
		{
			double x, y, z;
			x = getRandomDouble();
			y = getRandomDouble();
			z = getRandomDouble();

			double mag = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

			x *= 1 / mag;
			y *= 1 / mag;
			z *= 1 / mag;

			std::srand( time(NULL) );

			double angle = (2 * M_PI * rand() / RAND_MAX - M_PI);

			AxisAngle ret(x, y, z, angle);
			return ret;
		}

		static tf2::Quaternion createRandomTf2Quaternion()
		{
			tf2::Quaternion quaternion;
			quaternion.setRPY(getRandomAngle(), getRandomAngle(), getRandomAngle());
			quaternion.normalize();

			return quaternion;
		}

		static Quaternion createRandomQuaternion()
		{
			Quaternion quaternion;
			quaternion.set(getRandomDouble(), getRandomDouble(), getRandomDouble(), getRandomDouble());
			quaternion.normalize();

			return quaternion;
		}

		static bool areAxisAngleEpsilonEqual(const AxisAngle &a1, const AxisAngle &a2, const double &eps)
		{
			if ((fabs(a1.x - a2.x) < eps && fabs(a1.y - a2.y) < eps && fabs(a1.z - a2.z) < eps && fabs(a1.angle - a2.angle) < eps) ||
			        (fabs(-a1.x - a2.x) < eps && fabs(-a1.y - a2.y) < eps && fabs(-a1.z - a2.z) < eps && fabs(-a1.angle - a2.angle) < eps))
			{
				return true;
			}

			if ((fabs(a1.x - a2.x) < eps && fabs(a1.y - a2.y) < eps && fabs(a1.z - a2.z) < eps))
			{
				if (M_PI - fabs(a1.angle) < 1e-4 && M_PI - fabs(a2.angle) < 1e-4)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}

		static bool areQuaternionsEpsilonEqual(const Quaternion &q1, const Quaternion &q2, const double &eps)
		{
			if ((fabs(q1.getX() - q2.getX()) < eps && fabs(q1.getY() - q2.getY()) < eps && fabs(q1.getZ() - q2.getZ()) < eps && fabs(q1.getW() - q2.getW()) < eps) ||
			        (fabs(-q1.getX() - q2.getX()) < eps && fabs(-q1.getY() - q2.getY()) < eps && fabs(-q1.getZ() - q2.getZ()) < eps && fabs(-q1.getW() - q2.getW()) < eps))
			{
				return true;
			}

			return false;
		}

		static bool areVector3dsEpsilonEqual(const Eigen::Vector3d &v1, const Eigen::Vector3d &v2, const double &eps)
		{
			return (fabs(v1(0) - v2(0)) < eps && fabs(v1(1) - v2(1)) < eps && fabs(v1(2) - v2(2)) < eps);
		}

		static bool areVector4dsEpsilonEqual(const Eigen::Vector4d &v1, const Eigen::Vector4d &v2, const double &eps)
		{
			return (fabs(v1(0) - v2(0)) < eps && fabs(v1(1) - v2(1)) < eps && fabs(v1(2) - v2(2)) < eps && fabs(v1(3) - v2(3)) < eps);
		}

		static Eigen::Vector3d createRandomVector3d()
		{
			Eigen::Vector3d vector;

			vector(0) = getRandomDouble();
			vector(1) = getRandomDouble();
			vector(2) = getRandomDouble();

			return vector;
		}

		static bool areMatrix3dEpsilonEqual(const Eigen::Matrix3d &m1, const Eigen::Matrix3d &m2, double epsilon)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					bool tmp = fabs(m1(i, j) - m2(i, j)) < epsilon;
					if (!tmp)
					{
						return false;
					}
				}
			}

			return true;
		}

		static bool areMatrix4dEpsilonEqual(const Eigen::Matrix4d &m1, const Eigen::Matrix4d &m2, double epsilon)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					bool tmp = fabs(m1(i, j) - m2(i, j)) < epsilon;
					if (!tmp)
					{
						return false;
					}
				}
			}

			return true;
		}
};

}