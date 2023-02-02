#ifndef INCLUDED_IntArray
#define INCLUDED_IntArray

#include <iostream>

using namespace std;

class IntArray {
    /*
        Class Invariant (CI): 

    */

    private: 
        int * theArray;
        int contentLength;
        int arraySize;

    public: 
        // ==============================
        // Constructors
        // ==============================

            // Default Constructor

                // PRE: 
                // POST: 
                IntArray();

            // Copy Constructor

                // PRE: 
                // POST: 
                IntArray(const IntArray &pIntArray);

        // ==============================
        // Destructor
        // ==============================

                // PRE: 
                // POST: 
                ~IntArray();

        // ==============================
        // Accessor Functions
        // ==============================

                // PRE: 
                // POST: 
                int getNthIntInArray(int n) const;

                // PRE: 
                // POST: 
                int getArraySize() const;

                // PRE: 
                // POST: 
                int getContentLength() const;

                // PRE: 
                // POST: 
                int * getTheArray() const;


        // ==============================
        // Modifier Functions
        // ==============================



        // ==============================
        // Predicate Functions
        // ==============================





        // ==============================
        // Operators
        // ==============================



        // ==============================
        // Input/Output Member Functions
        // ==============================




}



#endif
