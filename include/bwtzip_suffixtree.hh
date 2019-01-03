// bwtzip_suffixtree.hh - bwtzip Suffix Tree functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#ifndef BWTZIP_SUFFIXTREE_HH
#define BWTZIP_SUFFIXTREE_HH

#include <vector>
#include <utility>

namespace bwtzip {
    std::vector<unsigned char>& bwtStree(std::vector<unsigned char>& v);

    namespace ukkonen {
        class SuffixNode;

        struct Transition {
            short        letter;
            long         left;
            long         right;
            SuffixNode * child;

            Transition(short letter, long left, long right, SuffixNode *child)
                : letter(letter), left(left), right(right), child(child) { }
            bool operator<(const Transition& rhs) const { return letter < rhs.letter; }
            bool operator>(const Transition& rhs) const { return letter > rhs.letter; }
            bool operator==(short a)              const { return letter == a; }
        };

// The TransitionList is an abstract container class. STL containers are
// unsuitable since the TransitionList must obey very precise semantics.
// Despite the name, a TransitionList is implemented using std::vector
// and is thus maximally space efficient.
// The std::vector is kept sorted by >. Thus, the smallest Transition
// (i.e. the child corresponding to the lowest character) is at the end
// of the vector, so we can access it using vec.back() and we can remove
// it efficiently using vec.pop_back().

// insertTransition is the most complicated method. If a Transition
// is inserted that has the same character as something already in
// the TransitionList, the Transition must be updated in place - a second
// Transition with the same character cannot be added.

        class TransitionList {
        public:
            TransitionList();
            bool        hasChildren() const;
            Transition  getFirstChild() const;
            void        removeFirstChild();
            bool        doesTransitionExist(short a) const;
            Transition  findTransition(short a) const;
            void        insertTransition(const Transition& t);

        private:
            std::vector<Transition> vec;
        };

// A SuffixNode contains a suffix link (which is a SuffixNode *) and a
// TransitionList for its children. SuffixNodes don't store any other
// information. Yet, in the Ukkonen algorithm, suffix links of leaf nodes
// are never traveled, and leaf nodes are born with no children and are
// never given any children. This is very important - born a leaf node,
// always a leaf node. It is pointless to have a Transition pointing to a
// leaf SuffixNode, because we always know what a leaf SuffixNode will
// contain. They're all identical. So, we may as well have one leaf
// SuffixNode in our entire program... or none at all. Now, when the
// code wants to create a leaf SuffixNode, the Transition from the parent
// to the leaf merely points to NULL. Since Transitions at all other times
// point to internal nodes (i.e. non-NULL), testing if the Transition
// points to NULL will tell us if there is a leaf below us.

// The Ukkonen algorithm never attempts to move onto a leaf, so only the
// code that creates leaves need be modified. The DFS needs to know about
// this, though, because it DOES traverse leaves (in fact, the leaves are
// the whole point).

        class SuffixNode {
        public:
            TransitionList children;
            SuffixNode *   link;

            SuffixNode();
            ~SuffixNode();
            bool haschildren() const;

            // The DFS destroys the tree as it walks through it.
            // The stack contains internal nodes, never leaf nodes.
            static void dfs(std::vector<size_t>& v, SuffixNode * root);

            static void displayInstances();

        private:
            static size_t count;
        };

        class SuffixTree {
        public:
            SuffixTree(const std::vector<unsigned char>& v);
            std::vector<unsigned char> getTransformed();
            ~SuffixTree();

        private:
            SuffixNode *          root;
            SuffixNode            bottom;
            short *               sanitizedtext;
            const size_t          vsize;
            const size_t          sentineledlength;
            const long            infinity;    // The location of the sentinel

            std::pair<SuffixNode *, long> canonize(SuffixNode * s, long k, long p);
            std::pair<bool, SuffixNode *> test_and_split(SuffixNode * s, long k, long p, short t);

            // Ukkonen: mypair.first = s  mypair.second = k
            std::pair<SuffixNode *, long> update(std::pair<SuffixNode *, long> mypair, long i);

            static short * initText(const std::vector<unsigned char>& v);
            std::vector<size_t> lengthsOfSortedSuffixes();
        };
    }
}

#endif // End of: #ifndef BWTZIP_SUFFIXTREE_HH
