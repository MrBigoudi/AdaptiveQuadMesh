#pragma once

#include <cmath>
#include <iostream>
#include <cassert>

namespace maths{

/**
 * An enumeration to represent the vector's type
*/
enum VecType {COLOR, POS};

/**
 * A class to represent 3d vectors
*/
class Vector3{

    private:
        /**
         * The vector values
        */
        float mVect[3];

        /**
         * The vector's type
        */
        VecType mType;

    private:
        /**
         * Test if r,g,b values are correct for a color vector
        */
        void typeAssert() const {
            if (mType == COLOR){
                assert(mVect[0]>=0.0f && mVect[0]<=1.0f);
                assert(mVect[1]>=0.0f && mVect[1]<=1.0f);
                assert(mVect[2]>=0.0f && mVect[2]<=1.0f);
            }
        }

    public: // CONSTRUCTORS

        /**
         * Basic constructor
         * @param x The x coordinate
         * @param y The y coordinate
         * @param z The z coordinate
         * @param type The type of the vector (only used for adding checks for possible values and for printting format)
        */
        Vector3(float x=0.0f, float y=0.0f, float z=0.0f, VecType type=POS){
            mVect[0] = x;
            mVect[1] = y;
            mVect[2] = z;
            mType = type;
            typeAssert();
        }

        /**
         * Constructor by copy
         * @param v The vector to copy
        */
        Vector3(Vector3* v){
            mVect[0] = v->mVect[0];
            mVect[1] = v->mVect[1];
            mVect[2] = v->mVect[2];
            mType = v->mType;
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

        /**
         * Getter for the red component
         * @return The vector's red component
        */
        float r() const {return mVect[0];};

        /**
         * Getter for the green component
         * @return The vector's green component
        */
        float g() const {return mVect[1];};

        /**
         * Getter for the blue component
         * @return The vector's blue component
        */
        float b() const {return mVect[2];};

    public: // SETTERS
        /**
         * Setter for the x coordinate
         * @param x The new x coordinate
        */
        void x(const float & x){mVect[0]=x; typeAssert();};

        /**
         * Setter for the y coordinate
         * @param y The new y coordinate
        */
        void y(const float & y){mVect[1]=y; typeAssert();};

        /**
         * Setter for the z coordinate
         * @param z The new z coordinate
        */
        void z(const float & z){mVect[2]=z; typeAssert();};

        /**
         * Setter for the red component
         * @param r The new red component
        */
        void r(const float & r){mVect[0]=r; typeAssert();};

        /**
         * Setter for the green component
         * @param g The new green component
        */
        void g(const float & g){mVect[0]=g; typeAssert();};

        /**
         * Setter for the blue component
         * @param b The new blue component
        */
        void b(const float & b){mVect[0]=b; typeAssert();};


    public: // VECTORS OPERATIONS
        /**
         * Vector addition
        */
        Vector3 operator + (const Vector3 & v) const{
            return * new Vector3(x() + v.x(), y() + v.y(), z() + v.z());
        };

        /**
         * Vector substraction
        */
        Vector3 operator - (const Vector3 & v) const{
            return * new Vector3(x() - v.x(), y() - v.y(), z() - v.z());
        };

        /**
         * Vector scalar multiplication
        */
        Vector3 operator * (const float & f) const{
            return * new Vector3(x()*f, y()*f, z()*f);
        };

        /**
         * Vector multiplication
        */
        Vector3 operator * (const Vector3 & v) const{
            return * new Vector3(x()*v.x(), y()*v.y(), z()*v.z());
        };

        /**
         * Vector scalar division
        */
        Vector3 operator / (const float & f) const{
            return * new Vector3(x()/f, y()/f, z()/f);
        };

        /**
         * Vector division
        */
        Vector3 operator / (const Vector3 & v) const{
            return * new Vector3(x()/v.x(), y()/v.y(), z()/v.z());
        };

        /**
         * Norm of the vector
         * @return The norm as a float
        */
        float norm() const { return sqrtf(x()*x() + y()*y() + z()*z()); }
	    
        /**
         * Normalized a vector
         * @return A new vector corresponding to the normalised vector
        */
        Vector3 normalize() const { 
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
        static float dot(const Vector3 & v1, const Vector3 & v2) {
            return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
        }

        /**
         * Vector cross product
         * @param v1 The first vector
         * @param v2 The second vector
         * @return The cross product of v1 and v2 as a new vector
        */
        static Vector3 cross(const Vector3 & v1, const Vector3 & v2){
            return * new Vector3(
                                    v1.y()*v2.z() - v1.z()*v2.y(),
                                    v1.z()*v2.x() - v1.x()*v2.z(),
                                    v1.x()*v2.y() - v1.y()*v2.x()
                                );
        };


        /**
         * Get the distance between 2 vectors
         * @param v1 The first vector
         * @param v2 The second vector
         * @return The distance between v1 and v2
        */
        static float distance(const Vector3 & v1, const Vector3 & v2){
            return sqrtf((v1.x()-v2.x())*(v1.x()-v2.x()) 
                        + (v1.y()-v2.y())*(v1.y()-v2.y()) 
                        + (v1.z()-v2.z())*(v1.z()-v2.z()));
        };


        /**
         * Printer for the vector
        */
        friend std::ostream& operator <<(std::ostream &out , const Vector3 & v){
            if(v.mType == COLOR)
                out << "r: " << v.mVect[0] << ", g: " << v.mVect[1] << ", b: " << v.mVect[2];
            else
                out << "x: " << v.mVect[0] << ", y: " << v.mVect[1] << ", z: " << v.mVect[2]; 
            return out; 
        };

        /**
         * Cast a vector into a printable string
         * @return The vector as a string
        */
        std::string toString() const {
            char buffer[50];
            if(mType == COLOR)
                sprintf(buffer, "r: %f, g: %f, b: %f", mVect[0], mVect[1], mVect[2]);
            else
                sprintf(buffer, "x: %f, y: %f, z: %f", mVect[0], mVect[1], mVect[2]);
            return buffer;
        }
};

};