#pragma once

#include <cmath>

namespace maths{

/**
 * A class to represent 3d vectors
*/
class Vector3{

    private:
        /**
         * The vector values
        */
        float mVect[3];

    public: // CONSTRUCTORS

        /**
         * Basic constructor
         * @param x The x coordinate
         * @param y The y coordinate
         * @param z The z coordinate
        */
        Vector3(float x=0.0f, float y=0.0f, float z=0.0f){
            mVect[0] = x;
            mVect[1] = y;
            mVect[2] = z;
        }

        /**
         * Constructor by copy
         * @param v The vector to copy
        */
        Vector3(Vector3* v){
            mVect[0] = v->x();
            mVect[1] = v->y();
            mVect[2] = v->z();
        }


    public: // GETTERS
        /**
         * Getter for the vector array
         * @return The 3d vector
        */
        float* getVect(){return mVect;};

        /**
         * Getter for the x coordinate
         * @return The vector's x coordinate
        */
        float x() const {return mVect[0];};

        /**
         * Getter for the y coordinate
         * @return The vector's y coordinate
        */
        float y() const {return mVect[1];};

        /**
         * Getter for the z coordinate
         * @return The vector's z coordinate
        */
        float z() const {return mVect[2];};

    public: // SETTERS
        /**
         * Setter for the x coordinate
         * @param x The new x coordinate
        */
        void x(float x){mVect[0]=x;};

        /**
         * Setter for the y coordinate
         * @param y The new y coordinate
        */
        void y(float y){mVect[1]=y;};

        /**
         * Setter for the z coordinate
         * @param z The new z coordinate
        */
        void z(float z){mVect[2]=z;};

    public: // VECTORS OPERATIONS
        /**
         * Vector addition
        */
        Vector3 operator + (Vector3 v){
            return * new Vector3(x() + v.x(), y() + v.y(), z() + v.z());
        };

        /**
         * Vector substraction
        */
        Vector3 operator - (Vector3 v){
            return * new Vector3(x() - v.x(), y() - v.y(), z() - v.z());
        };

        /**
         * Vector scalar multiplication
        */
        Vector3 operator * (float f){
            return * new Vector3(x()*f, y()*f, z()*f);
        };

        /**
         * Vector multiplication
        */
        float operator * (Vector3 v){
            return (x()*v.x() + y()*v.y() + z()*v.z());
        };

        /**
         * Vector scalar division
        */
        Vector3 operator / (float f){
            return * new Vector3(x()/f, y()/f, z()/f);
        };

        /**
         * Norm of the vector
         * @return The norm as a float
        */
        float norm() { return sqrtf(x()*x() + y()*y() + z()*z()); }
	    
        /**
         * Normalized a vector
         * @return A new vector corresponding to the normalised vector
        */
        Vector3 normalize() { 
            float n = norm();  
            float newX = x()/n; 
            float newY = y()/n; 
            float newZ = z()/n;
            return * new Vector3(newX,newY,newZ);
        }

        /**
         * Vector dot product
         * @param v1 The first vector
         * @param v2 The second vector
         * @return The dot product of v1 and v2 as a float
        */
        static float dot(Vector3 v1, Vector3 v2){
            return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
        }

        /**
         * Vector cross product
         * @param v1 The first vector
         * @param v2 The second vector
         * @return The cross product of v1 and v2 as a new vector
        */
        static Vector3 cross(Vector3 v1, Vector3 v2){
            return * new Vector3(
                                    v1.y()*v2.z() - v1.z()*v2.y(),
                                    v1.z()*v2.x() - v1.x()*v2.z(),
                                    v1.x()*v2.y() - v1.y()*v2.x()
                                );
        };

        /**
         * Get the angle between two vectors
         * @param v1 The first vector
         * @param v2 The second vector
         * @return The angle between v1 and v2
        */
        static float angle(Vector3 v1, Vector3 v2){
            float cosalpha = (v1 * v2) / (v1.norm() * v2.norm());
		    if (cosalpha < -1) cosalpha = -1; 
            if (cosalpha > 1) cosalpha = 1;
		    return std::acos(cosalpha);
        };
};

};