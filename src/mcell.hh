// mcell.hh - Memory Cell Abstraction Interface
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

// The Mcell is similar to a STL vector. It can hold arbitrary data
// (which need only support a few operations). Mcells are fully
// abstracted - all memory access is bounds-checked, and improper
// attempts to manipulate memory are detected immediately. Upon
// detection, program execution is immediately halted, and an
// error message is printed. Mcells are given names, so that error
// messages can be traced back to a particular named cell.

#ifndef MCELL_HH
#define MCELL_HH

#include <iostream>
#include <string>
#include <list>
#include <utility>
using namespace std;

// NOTE: Functions which return an Mcell<DataType>& return our cell,
// unless otherwise specified

template <class DataType> class Mcell {
public:
    Mcell();                            // Default constructor, creates an empty cell
    Mcell(string s, size_t n);          // Construct an uninitialized memory cell
                                        // of a given name and size.
    Mcell(const Mcell<DataType>& c);    // Copy constructor

    size_t getSize() const;             // Return the size of our cell
    string getName() const;             // Return the name of our cell

    Mcell<DataType>& alloc              // Free any memory in our cell and safely
        (string s, size_t n);           // allocate new uninitialized memory with
                                        // the given name and size.

    Mcell<DataType>& mfree();           // Safely free the memory in our cell, null out
                                        // its pointer, and set the size to zero. Does
                                        // not modify the name.

    Mcell<DataType>& transcopy(         // Safely copy n values from location src in the
        size_t n, size_t src,           // given cell to location dest in our cell.
        const Mcell<DataType>& c,
        size_t dest);       
    
    Mcell<DataType>& move(size_t n,     // Safely move n values from location src of our
        size_t src, size_t dest);       // cell to location dest.
        
    Mcell<DataType>& shiftdown(         // Move n values from location src to the next
        size_t n, size_t src);          // higher location.

    Mcell<DataType>& truncate(          // Make our cell contain only values at locations n
        size_t begin, size_t end);      // begin <= n < end.

    Mcell<DataType>& fill               // Fill our cell with a given value
        (DataType val);

    size_t find(DataType val);          // Return the location of the first occurence of
                                        // a given value, scream and die if it does not exist.

    Mcell<DataType>& operator=          // Make our cell into a copy of the given cell.
        (const Mcell<DataType>& c);

    Mcell<DataType>& operator=          
        (const list<DataType>& l);

    DataType& operator[]                // Safely access the value at location n in our cell.
        (size_t n);                     

    const DataType& operator[]          // Safely access the value at location n in our cell.
        (size_t n) const;               

    bool operator==                     // Does our cell contains the same things as a given cell?
        (const Mcell<DataType>& c)      // Does not compare names.
        const;

    bool operator!=(
        const Mcell<DataType>& c)
        const;
   
    Mcell<DataType> operator+           // Return a newly allocated cell whose
        (const Mcell<DataType>& c)      // contents are that of two cells concatenated.
        const;   
   
    Mcell<DataType>& operator+=         // Append the contents of the given
        (const Mcell<DataType>& c);     // cell onto our cell.

    virtual ~Mcell();                   // Destructor

protected:
    DataType *    buf;                  // The memory buffer itself
    string        name;                 // The name of the memory cell
    size_t        size;                 // How many values are being stored

private:
    void bracketCheck(size_t n) const;  // Check access for operator[]
    void check() const;                 // Has the cell been freed?
    bool isOOB(size_t n) const;         // Is a location out of bounds?
    void printInfo() const;             // Print the cell's information to cout
};

// PURPOSE: Default constructor, creates an empty cell
template <class DataType> Mcell<DataType>::Mcell() {  
    name = "Empty Default Mcell";
    size = 0;
    buf = NULL;
}

// PURPOSE: Construct a memory cell of a given name and size.
// NOTE: Memory is not initialized.
template <class DataType> Mcell<DataType>::Mcell(string s, size_t n) {
    buf = NULL;
    alloc(s, n);
}

// PURPOSE: Copy constructor
template <class DataType> Mcell<DataType>::Mcell(const Mcell<DataType>& c) {
    buf = NULL;
    alloc(c.name, c.size);
    transcopy(size, 0, c, 0);
}

// RETURN: The size of our Mcell
template <class DataType> size_t Mcell<DataType>::getSize() const {
    return size;
}

// RETURN: The name of our Mcell
template <class DataType> string Mcell<DataType>::getName() const {
    return name;
}

// PURPOSE: Free any memory in our cell and allocate new memory with the given name and size.
// NOTE: Memory is not initialized.
// ERROR: Scream and die if impossible.
// RETURN: Our cell.
template <class DataType> Mcell<DataType>& Mcell<DataType>::alloc(string s, size_t n) {
    if (buf != NULL) {
        mfree();
    }

    name = s;
    size = n;

    if (size == 0) {
        buf = NULL;
    } else {
        buf = new DataType[size];
    }

    return *this;
}

// PURPOSE: Free the memory in our cell and null out its pointer. Zero the size!
// NOTE: Name is not modified.
// ERROR: Scream and die if our cell is already freed.
// RETURN: Our cell.
template <class DataType> Mcell<DataType>& Mcell<DataType>::mfree() {
    check();        // Cannot free a freed memory cell
    
    delete[] buf;
    buf = NULL;
    size = 0;

    return *this;
}

// PURPOSE: Copy n values from location src in the given cell to location dest in our cell.
// NOTE: Works correctly when both cells are the same.
// NOTE: Now works correctly when transcopying 0 values.
// ERROR: Scream and die if out-of-bound access is attempted.
// RETURN: Our cell.
template <class DataType> Mcell<DataType>&
    Mcell<DataType>::transcopy(size_t n, size_t src, const Mcell<DataType>& c, size_t dest) {

    if (n == 0) {
        return *this;  // GNDN if transcopying 0 values
    }

    if (c.isOOB(src) || c.isOOB(src + n - 1) || isOOB(dest) || isOOB(dest + n - 1)) {
        cout << "ERROR: transcopy(): Illegal attempt to copy data." << endl;
        printInfo();
        c.printInfo();
        cout << "       Attempted to copy " << n << " values." << endl;
        cout << "       From " << src << " in cell 2 to " << dest << " in cell 1." << endl;
        exit(EXIT_FAILURE);
    }

    Mcell<DataType> temp("transcopy() Temporary Cell", n);

    for (size_t st = 0; st < n; st++) {
        temp[st] = c[src + st];
    }

    for (size_t st = 0; st < n; st++) {
        (*this)[dest + st] = temp[st];
    }

    return *this;
}

// PURPOSE: Move n values from location src of our cell to location dest.
// NOTE: Source values are left unchanged, unless they are in destination.
// ERROR: Scream and die if out-of-bound access is attempted.
// RETURN: Our cell.
template <class DataType> Mcell<DataType>&
    Mcell<DataType>::move(size_t n, size_t src, size_t dest) {

    if (isOOB(src) || isOOB(src + n - 1) || isOOB(dest) || isOOB(dest + n - 1)) {
        cout << "ERROR: move(): Illegal attempt to move data." << endl;
        printInfo();
        cout << "       Attempted to move " << n << " values from ";
        cout << src << " to " << dest << "." << endl;
        exit(EXIT_FAILURE);
    }

    return transcopy(n, src, *this, dest);
    
}
 
// Move n values from location src to the next higher location. Return our cell.
template <class DataType> Mcell<DataType>& 
    Mcell<DataType>::shiftdown(size_t n, size_t src) {
    
    if (isOOB(src) || isOOB(n + src)) {
        cout << "ERROR: shiftdown(): Illegal attempt to move data." << endl;
        printInfo();
        cout << "       Attempted to move " << n << " values from " << src << endl;
        exit(EXIT_FAILURE);
    }
    
    for (size_t i = src + n; i > src; i--) {
        (*this)[i] = (*this)[i - 1];
    }
    
    return *this;    
}


// PURPOSE: Make our cell contain only values at locations between begin and end.
// NOTE: The range is inclusive at begin and exclusive at end.
// RETURN: Our cell.
template <class DataType> Mcell<DataType>& Mcell<DataType>::truncate(size_t begin, size_t end) {
    string originalname = name;

    Mcell<DataType> temp("truncate() Temporary Mcell", end - begin);

    *this = temp.transcopy(end - begin, begin, *this, 0);

    name = originalname;

    return *this;
}

// PURPOSE: Fill our cell with a given value
// RETURN: Our cell.
template <class DataType> Mcell<DataType>& Mcell<DataType>::fill(DataType val) {
    for (size_t st = 0; st < size; st++) {
        (*this)[st] = val;
    }
    return *this;
}

// RETURN: The location of the first occurence of a given value
// ERROR: Scream and die if that value does not exist in our cell.
template <class DataType> size_t Mcell<DataType>::find(DataType val) {

    for (size_t st = 0; st < size; st++) {
        if ((*this)[st] == val) {
            return st;
        }
    }

    cout << "ERROR: find() unable to find given value." << endl;
    exit(EXIT_FAILURE);
}

// PURPOSE: Make our cell into a copy of the given cell.
template <class DataType> Mcell<DataType>& Mcell<DataType>::operator= (const Mcell<DataType>& c) {
    if (&c != this) {
        alloc(c.name, c.size);
        transcopy(c.size, 0, c, 0);
    }

    return *this;
}

template <class DataType> Mcell<DataType>& Mcell<DataType>::operator= (const list<DataType>& l) {
    alloc("STL List", l.size());

    typename list<DataType>::const_iterator it = l.begin();
    for (size_t i = 0; i < size; i++) {
        (*this)[i] = *it++;
    }
    return *this;
}

// PURPOSE: Access the value at location n in our cell.
// ERROR: Scream and die if out-of-bound access is attempted or if the cell is already freed.
template <class DataType> DataType& Mcell<DataType>::operator[](size_t n) {
    bracketCheck(n);
    return buf[n];
}

template <typename DataType> const DataType& Mcell<DataType>::operator[](size_t n) const {
    bracketCheck(n);
    return buf[n];
}

// RETURN: Whether our cell contains the same things as a given cell.
// NOTE: The names of the cells are not compared.
template <class DataType> bool Mcell<DataType>::operator==(const Mcell<DataType>& c) const {
    if (size != c.size) {
        return false;
    }
    
    for (size_t st = 0; st < c.size; st++) {
        if ((*this)[st] != c[st]) {
            return false;
        }
    }
    
    return true;
}

// RETURN: Whether our cell does not contain the same things as a given cell.
// NOTE: The names of the cells are not compared.
template <class DataType> bool Mcell<DataType>::operator!=(const Mcell<DataType>& c) const {
    return !(*this == c);
}

// PURPOSE: Return a newly allocated cell whose contents are that of two cells concatenated.
// NOTE: The name of the new cell is set to the name of the left cell.
template <class DataType> Mcell<DataType> Mcell<DataType>::operator+(const Mcell<DataType>& c) const {
    Mcell<DataType> ret(name, size + c.size);

    if (size != 0) {
        ret.transcopy(size, 0, *this, 0);
    }

    if (c.size != 0) {
        ret.transcopy(c.size, 0, c, size);
    }
    
    return ret;
}

// PURPOSE: Append the contents of the given cell onto our cell.
template <class DataType> Mcell<DataType>& Mcell<DataType>::operator+=(const Mcell<DataType>& c) {
    *this = *this + c;
    return *this;
}

// Destructor
template <class DataType> Mcell<DataType>::~Mcell() {
    delete[] buf;
}

// PURPOSE: Check access for operator[]
template <class DataType> void Mcell<DataType>::bracketCheck(size_t n) const {
    check();    // Cannot access a freed memory cell
    
    if (isOOB(n)) {
        cout << "ERROR: []: Invalid access attempt." << endl;
        cout << "       Can not access location " << n << "." << endl;
        printInfo();
        exit(EXIT_FAILURE);
    }
}

// PURPOSE: Scream and die if our cell has been freed.
template <class DataType> void Mcell<DataType>::check() const {
    if (buf == NULL) {
        cout << "ERROR: Cell already freed." << endl;
        printInfo();
        exit(EXIT_FAILURE);
    }
}

// RETURN: Whether a given location is out of bounds in our cell.
template <class DataType> bool Mcell<DataType>::isOOB(size_t n) const {
    return n >= size;
}

// PURPOSE: Print our cell's information to cout.
template <class DataType> void Mcell<DataType>::printInfo() const {
    cout << "       Cell: " << name << "; Size: " << size << "." << endl;
}

#endif // End of: #ifndef MCELL_HH
