#ifndef POINT_3D_HPP
#define POINT_3D_HPP

#include <eigen3/Eigen/Eigen>
#include <vector>
#include <math.h>
#include "frl/utils/Utilities.hpp"
#include <stdexcept>

namespace frl
{
    namespace geometry
    {
        template<class T>
        class Point3
        {
        public:
            template<typename TYPE>
            Point3(const TYPE x, const TYPE y, const TYPE z)
            {
                set(x, y, z);
            }

            template<typename TYPE>
            Point3(const std::vector<TYPE> &vector)
            {
                set(vector[0], vector[1], vector[2]);
            }

            Point3(const Point3 &point)
            {
                set(point.x, point.y, point.z);
            }

            Point3(const T array[3])
            {
                set(array[0], array[1], array[2]);
            }

            Point3()
            {
                set(0.0, 0.0, 0.0);
            }

            ~Point3()
            { };

            template<typename TYPE>
            void set(const std::vector<TYPE> &vector)
            {
                if(vector.size() != 3)
                {
                    throw std::runtime_error("Vector must be size 3!");
                }
                set(vector[0], vector[1], vector[2]);
            }

            template<typename TYPE>
            void set(const TYPE x, const TYPE y, const TYPE z)
            {
                this->x = x;
                this->y = y;
                this->z = z;
            }

            bool equals(const Point3 &point)
            {
                return (this->x == point.x && this->y == point.y && this->z == point.z);
            }

            template<typename TYPE>
            bool epsilonEquals(const Point3<TYPE> &point, const double epsilon)
            {
                return (fabs(this->x - point.x) < epsilon && fabs(this->y - point.y) < epsilon && fabs(this->z - point.z) < epsilon);
            }

            void clampMin(const T min)
            {
                frl::utils::clampMin(x,min);
                frl::utils::clampMin(y,min);
                frl::utils::clampMin(z,min);
            }

            void clampMax(const T max)
            {
                frl::utils::clampMax(x,max);
                frl::utils::clampMax(y,max);
                frl::utils::clampMax(z,max);
            }

            void clampMinMax(const T min, const T max)
            {
                clampMin(min);
                clampMax(max);
            }

            void absoluteValue()
            {
                this->x = fabs(this->x);
                this->y = fabs(this->y);
                this->z = fabs(this->z);
            }

            T distanceSquared(const Point3 &point) const
            {
                return frl::utils::computeDistanceBetweenPointsSquared(x,y,z,point.x,point.y,point.z);
            }

            T distance(const Point3 point) const
            {
                return frl::utils::computeDistanceBetweenPoints(x,y,z,point.x,point.y,point.z);
            }

            T distanceL1(const Point3 &point) const
            {
                return frl::utils::distanceL1(x,y,z,point.x,point.y,point.z);
            }

            T distanceLinf(const Point3 &point) const
            {
                return frl::utils::distanceLinf(x,y,z,point.x,point.y,point.z);
            }

            T getX() const
            {
                return this->x;
            };

            T getY() const
            {
                return this->y;
            };

            T getZ() const
            {
                return this->z;
            };

            void setX(T x)
            {
                this->x = x;
            }

            void setY(T y)
            {
                this->y = y;
            }

            void setZ(T z)
            {
                this->z = z;
            }

            Point3<T>& operator+=(const Point3 &point)
            {
                this->x += point.x;
                this->y += point.y;
                this->z += point.z;

                return *this;
            }

            Point3<T>& operator-=(const Point3 &point)
            {
                this->x -= point.x;
                this->y -= point.y;
                this->z -= point.z;

                return *this;
            }

            Point3<T>& operator*=(const Point3 &point)
            {
                this->x *= point.x;
                this->y *= point.y;
                this->z *= point.z;

                return *this;
            }

            Point3<T>& operator/=(const Point3 &point)
            {
                this->x /= point.x;
                this->y /= point.y;
                this->z /= point.z;

                return *this;
            }

            T x, y, z;
        };

        template<class T>
        inline Point3<T> operator+(Point3<T> leftHandSide, const Point3<T> &point)
        {
            leftHandSide.x += point.x;
            leftHandSide.y += point.y;
            leftHandSide.z += point.z;

            return leftHandSide;
        }

        template<class T>
        inline Point3<T> operator-(Point3<T> leftHandSide, const Point3<T> &point)
        {
            leftHandSide.x -= point.x;
            leftHandSide.y -= point.y;
            leftHandSide.z -= point.z;

            return leftHandSide;
        }

        template<class T,typename TYPE>
        inline Point3<T> operator*(Point3<T> leftHandSide, const TYPE &scale)
        {
            leftHandSide.x *= scale;
            leftHandSide.y *= scale;
            leftHandSide.z *= scale;

            return leftHandSide;
        }
//
//        inline bool operator==(const Point3& lhs, const Point3& rhs)
//        {
//            if(lhs.x!=rhs.x)
//            {
//                return false;
//            }
//
//            if(lhs.y!=rhs.y)
//            {
//                return false;
//            }
//
//            if(lhs.z!=rhs.z)
//            {
//                return false;
//            }
//
//            return true;
//        }
//        inline bool operator!=(const Point3& lhs, const Point3& rhs){return !operator==(lhs,rhs);}
//
//        std::ostream &operator<<(std::ostream &os, const Point3 &point)
//        {
//            os << "x: " << point.x << '\n' << "y: " << point.y << '\n' << "z: " << point.z << "\n";
//            return os;
//        }

        typedef Point3<float> Point3f;
        typedef Point3<double> Point3d;
    }

}
#endif