// gcell.hh - Memory Cell Abstraction Interface (Gcell: Growing Cell)
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

// The Gcell is a conceptually simple ordered container of arbitrary data.
// It is always instantiated with a default constructor:
//      Gcell<footype> gcell;
// When instantiated, the Gcell is "empty".
// The size of a Gcell can be determined at any time with getSize():
//      size_t mysize = gcell.getSize();
// Data can be tacked on to the front of the Gcell using prependData():
//      gcell.prependData(137);
// And it can be tacked on to the end using appendData():
//      gcell.appendData(5);
// The first data element can be read and removed from the Gcell:
//      footype foo = gcell.popFrontData();
// Gcells can be copy-constructed.
//      Gcell<footype> one = two;
// Gcells can be assigned.
//      one = two;
// The first n elements of the Gcell can be removed and split off into
// a new Gcell, which is returned.
//      Gcell newcell = gcell.popFront(16);
// At any time, the Gcell's contents can be returned in a newly allocated
// Mcell, using the getMcell() method. This does not modify the Gcell.
//      Mcell<footype> mcell = gcell.getMcell();
// The Gcell can be emptied with the clear() method:
//      gcell.clear();
// The destructor simply calls clear().
// Gcells can also be appended to each other.
//      Gcell<footype> one, two;
//      [...data is added to Gcells one and two...]
//      one += two;
// They are appended in order; one comes before two.
// Destructive appending is also possible.
//      one.destructiveAppend(two);
// NOTE: After doing a destructiveAppend(), the second Gcell is emptied.

#ifndef GCELL_HH
#define GCELL_HH

#include <iostream>
#include "mcell.hh"

template <class DataType> class Gcell {
public:

// Time Complexity:
// Gcells have an extremely attractive time complexity, by design.
// At any given time, a Gcell of N elements consumes O(N) space.
// Let the second cell being operated on have M elements.

    Gcell();                                                // O(1)
    Gcell(const Gcell<DataType>& g);                        // O(M)
    size_t           getSize() const;                       // O(1)
    Gcell<DataType>& prependData(DataType d);               // O(1)
    Gcell<DataType>& appendData(DataType d);                // O(1)
    DataType         popFrontData();                        // O(1)
    Gcell<DataType>  popFront(size_t n);                    // O(n)
    Mcell<DataType> getMcell() const;                       // O(N)
    Gcell<DataType>& clear();                               // O(N)
    Gcell<DataType>& destructiveAppend(Gcell<DataType>& g); // O(1)
    Gcell<DataType>& operator+=(const Gcell<DataType>& g);  // O(M)
    Gcell<DataType>& operator=(const Gcell<DataType>& g);   // O(N + M)
    bool operator==(const Gcell<DataType>& g) const;        // O(N)
    bool operator!=(const Gcell<DataType>& g) const;        // O(N)
    virtual ~Gcell();                                       // O(N)

protected:
    void null();

    struct Node {
        DataType      data;        
        Node *        next;

        Node(DataType data, Node *next) : data(data), next(next) {}
    };

    Node * head;
    Node * tail;
    size_t size;
};

template <class DataType> Gcell<DataType>::Gcell() {
    null();
}

template <class DataType> Gcell<DataType>::Gcell(const Gcell<DataType>& g) {
    null();
    (*this) += g;
}

template <class DataType> size_t Gcell<DataType>::getSize() const {
    return size;
}

template <class DataType> Gcell<DataType>& Gcell<DataType>::operator=(const Gcell<DataType>& g) {
    if (&g != this) {
        clear();
        (*this) += g;
    }
    return *this;
}

template <class DataType> Gcell<DataType>& Gcell<DataType>::prependData(DataType d) {
    head = new Node(d, head);
    size++;

    if (head->next == NULL) {
        tail = head;
    }
    
    return *this;
}

template <class DataType> Gcell<DataType>& Gcell<DataType>::appendData(DataType d) {
    size++;

    if (head == NULL) {
        head = tail = new Node(d, NULL);
    } else {
        tail = tail->next = new Node(d, NULL);
    }

    return *this;
}

template <class DataType> DataType Gcell<DataType>::popFrontData() {
    if (size == 0) {
        cout << "ERROR: popFrontData() called on an empty Gcell." << endl;
        exit(EXIT_FAILURE);
    }

    DataType ret = head->data;

    if (size == 1) {
        clear();
    } else {
        Node * temp = head;
        head = head->next;
        delete temp;
        size--;
    }

    return ret;
}


template <class DataType> Gcell<DataType> Gcell<DataType>::popFront(size_t n) {
    if (n > size) {
        cout << "Gcell<DataType>::popFront() asked to pop " << n << " elements." << endl;
        cout << "Gcell only has " << size << " elements." << endl;
        exit(EXIT_FAILURE);
    }

    Gcell<DataType> ret;

    if (n == 0) {
        // GNDN
    } else if (n == size) {
        ret.head = head;
        ret.tail = tail;
        ret.size = size;
        null();
    } else {
        Node * temp = head;
        for (size_t i = 0; i < n - 1; i++) {
            temp = temp->next;
        }

        // temp now points to the last element to be popped
        ret.head = head;
        ret.tail = temp;
        ret.size = n;
        head = temp->next;
        // tail unchanged
        size -= n;

        // Sever link between cells
        temp->next = NULL;
    }

    return ret;
}

template <class DataType> Mcell<DataType> Gcell<DataType>::getMcell() const {
    Node *temp = head;

    Mcell<DataType> ret("Converted Gcell", size);

    for (size_t i = 0; i < size; i++) {
        ret[i] = temp->data;
        temp = temp->next;
    }

    if (temp != NULL) {
        cout << "Problem in Gcell<DataType>::getMcell()" << endl;
        exit(EXIT_FAILURE);
    }

    return ret;
}

template <class DataType> Gcell<DataType>& Gcell<DataType>::destructiveAppend(Gcell<DataType>& g) {
    if (g.head != NULL) {
        if (head == NULL) {
            head = g.head;
        } else {                    // Head remains where it is
            tail->next = g.head;    // Link is forged between the lists
        }
            
        tail = g.tail;              // Tail is updated to the end of the new list
    }                               // Othw. nothing to do
    
    size += g.size;                 // Update size
    g.null();                       // g is always null()ed after leaving the function
    return *this;                   // NOTE: We do not use g.clear() for obvious reasons
}

template <class DataType> Gcell<DataType>& Gcell<DataType>::operator+=(const Gcell<DataType>& g) {
    Node *temp = g.head;
    while (temp != NULL) {
        appendData(temp->data);
        temp = temp->next;
    }
    return *this;
}

template <class DataType> Gcell<DataType>& Gcell<DataType>::clear() {
    Node * temp = head;

    while (temp != NULL) {
        Node * curr = temp;
        temp = temp->next;
        delete curr;
    }
    
    null();
    return *this;
}

template <class DataType> void Gcell<DataType>::null() {
    head = tail = NULL;
    size = 0;
}

template <class DataType> bool Gcell<DataType>::operator==(const Gcell<DataType>& g) const {
    if (size != g.size) {
        return false;
    }

    Node * us = head;
    Node * them = g.head;

    while (us != NULL && them != NULL) {
        if (us->data != them->data) {
            return false;
        }
        us = us->next;
        them = them->next;
    }

    return true;
}

// RETURN: Whether our cell does not contain the same things as a given cell.
// NOTE: The names of the cells are not compared.
template <class DataType> bool Gcell<DataType>::operator!=(const Gcell<DataType>& g) const {
    return !(*this == g);
}

template <class DataType> Gcell<DataType>::~Gcell() {
    clear();
}

#endif // End of: #ifndef GCELL_HH
