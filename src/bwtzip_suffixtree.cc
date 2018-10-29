// bwtzip_suffixtree.cc - bwtzip Suffix Tree functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "bwtzip_common.hh"
#include "bwtzip_suffixtree.hh"
#include "clock.hh"
#include <stack>
#include <iostream>
#include <cstdlib>
using namespace std;
using namespace bwtzip;
using namespace bwtzip::ukkonen;

size_t bwtzip::ukkonen::SuffixNode::count = 0;

// ********************* TRANSITIONLIST

bwtzip::ukkonen::TransitionList::TransitionList() {
    vec.reserve(2);
}

bool bwtzip::ukkonen::TransitionList::hasChildren() const {
    return !vec.empty();
}

Transition bwtzip::ukkonen::TransitionList::getFirstChild() const {
    return vec.back();
}

void bwtzip::ukkonen::TransitionList::removeFirstChild() {
    vec.pop_back();
}

bool bwtzip::ukkonen::TransitionList::doesTransitionExist(short a) const {
    for (vector<Transition>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
        if (*it == a) {
            return true;
        }
    }

    return false;
}

Transition bwtzip::ukkonen::TransitionList::findTransition(short a) const {
    for (vector<Transition>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
        if (*it == a) {
            return *it;
        }
    }

    cout << "Problem in findTransition()!" << endl;
    exit(EXIT_FAILURE);
}

void bwtzip::ukkonen::TransitionList::insertTransition(const Transition& t) {
    for (vector<Transition>::iterator it = vec.begin(); it != vec.end(); ++it) {
        if (t < *it) {          // Do nothing, go to next element.
        } else if (t > *it) {   // Case 1: Insert before some element of the list.
            vec.insert(it, t);  // Insert t before it
            return;
        } else {                // if (t == *it)
            *it = t;            // Case 2: Edit something in the list.
            return;
        }
    }

    vec.push_back(t); // Case 3: Insert at end of list.
}

// ********************* SUFFIXNODE

bwtzip::ukkonen::SuffixNode::SuffixNode()
    : link(NULL) {

    count++;
}

bwtzip::ukkonen::SuffixNode::~SuffixNode() {
    count--;
}

bool bwtzip::ukkonen::SuffixNode::haschildren() const {
    return children.hasChildren();
}

// The DFS destroys the tree as it walks through it.
// The stack contains internal nodes, never leaf nodes.
void bwtzip::ukkonen::SuffixNode::dfs(vector<size_t>& v, SuffixNode * root) {
    stack<pair<SuffixNode *, size_t> > stak;
    stak.push(make_pair(root, (size_t) 0));

    while (!stak.empty()) {
        // stak.top().first points to the node we're processing.
        if (stak.top().first->haschildren()) {
            Transition temp = stak.top().first->children.getFirstChild();
            stak.top().first->children.removeFirstChild();
            // temp.child points to the SuffixNode below us.
            // Is it a leaf?
            if (temp.child != NULL) {
                // There is an internal node below us. Let's process it.
                pair<SuffixNode *, size_t> mypair(
                    temp.child,
                    stak.top().second + temp.right - temp.left + 1
                );
                stak.push(mypair);
            } else { // There is a leaf below us.
                v.push_back(stak.top().second + temp.right - temp.left + 1); // Update v
            }
        } else {    // We're finished with this internal node.
            delete stak.top().first;    // Delete the internal node.
            stak.pop();
        }
    }
}

// void Bcell::SuffixNode::dfs(vector<size_t>& v, SuffixNode * root) {
// The DFS is given an empty vector whose capacity has been reserved
// such that it will never need to grow during the entire DFS - this
// is very efficient in time and space!
// The DFS is also given a pointer to the root.
// The DFS will traverse the entire SuffixTree (except for bottom,
// which is only used during the execution of the Ukkonen algorithm) in
// lexicographic order, visiting left nodes before right nodes. In
// this manner it will find all of the leaves of the tree, which
// correspond to suffixes of the string, in sorted order.
// The vector v contains the height of each leaf from left to right.
// Recall that Transitions are labeled by substrings of the original
// string. If we're at a leaf, we add up the lengths of the substrings
// over all the Transitions from the root to the leaf, and that is the
// leaf's height.
// Knowing the leaf's height allows us to figure out which suffix the
// leaf represents, since all the suffixes are of different lengths.
// The code which generates the BWT-transformed text once given the
// heights of the leaves is "trivial".
//
//     stack<pair<SuffixNode *, size_t> > stak;
// The stack contains internal nodes which have traversed and will need
// to backtrack over later. It also remembers the height of each
// internal node.
//
//     stak.push(make_pair(root, (size_t) 0));
// We start at the root, which is at height 0 (the empty suffix).
//
//     while (!stak.empty()) {
// We will continue doing the DFS for as long as there are internal
// nodes left to process. When the stack is empty, that means that
// we have freed the root and all its descendants, and can return - the
// vector v will have been filled with the information we want.
//
// stak.top().first points to the node we're processing.
// It is an internal node.
//         if (stak.top().first->haschildren()) {
// If the internal node we are processing has children, we must
// traverse those children.
//
//             Transition temp =
// stak.top().first->children.getFirstChild();
// This Transition points to the first child of the internal node we're
// processing.
//
//             stak.top().first->children.removeFirstChild();
// We remove that first child, since we will be processing it shortly.
//
// temp.child points to the SuffixNode below us.
// Is it a leaf?
//             if (temp.child != NULL) {
// There is an internal node below us. Let's process it.
//
//                 pair<SuffixNode *, size_t>
//                     mypair(temp.child,
//                     stak.top().second + temp.right - temp.left + 1);
// This says that we're currently processing the internal node below us.
// We also calculate its height using the height of our current
// internal node and the length of the substring which the Transition
// represents.
//
//                 stak.push(mypair);
// We put that on the stack, and we will process that internal node on
// the next iteration.
//
//             } else {
// There is a leaf below us - the Transition points to NULL.
// We have already removed the Transition from the parent's
// TransitionList. The leaf does not actually exist, so we need
// not delete it or clean up after it. All we need to do is
// update v, since we've found a leaf.
//                 v.push_back(stak.top().second + temp.right - temp.left + 1);
// This is the same equation that generates the height of a node as
// seen directly above.
//             }
//         } else {
// If the internal node we're processing has NO children, then we are
// finished with it. We need merely delete the internal node and
// backtrack up the tree, which we do by popping the stack.
//             delete stak.top().first;
//             stak.pop();
//         }
//     }
// }
//
// Note that thanks to the NULL trick, the DFS is more elegant: any
// SuffixNode with no children is an internal node that we have entirely
// processed. We don't need to be confused by leaf nodes having no
// children, since the leaf nodes don't exist.

void bwtzip::ukkonen::SuffixNode::displayInstances() {
    cout << "Number of SuffixNodes instantiated: " << count << endl;
}

// ********************* SUFFIXTREE

bwtzip::ukkonen::SuffixTree::SuffixTree(const vector<unsigned char>& v)
    : root(new SuffixNode),
      sanitizedtext(initText(v)),
      vsize(v.size()),
      sentineledlength(v.size() + 1),
      infinity(v.size()) {

    // There will be v.size() + 1 leaves (which are NULL)
    // There will be at most v.size() + 1 SuffixNodes including root and bottom.

    for (int j = 1; j <= 257; j++) {
        bottom.children.insertTransition(Transition(sanitizedtext[-j], -j, -j, root));
    }

    root->link = &bottom;

    // Algorithm 2, steps 4 - 8

  Clock ukkonentimer("Ukkonen Timer");
    pair<SuffixNode *, long> temp(root, 0);

    for (long i = 0; i < (long) v.size() + 1; i++) {
        temp = update(temp, i);
        temp = canonize(temp.first, temp.second, i);
    }
  ukkonentimer.print();
}

vector<size_t> bwtzip::ukkonen::SuffixTree::lengthsOfSortedSuffixes() {
    vector<size_t> ret;
    ret.reserve(sentineledlength);
  Clock dfstimer("DFS Timer");
    SuffixNode::dfs(ret, root);
  dfstimer.print();
    return ret;
}

vector<unsigned char> bwtzip::ukkonen::SuffixTree::getTransformed() {
    vector<unsigned char> transformed(sentineledlength);
    const vector<size_t> l = lengthsOfSortedSuffixes();

    vector<unsigned char> primaryindex;
    vector<unsigned char> sentinelindex;

    for (size_t i = 0; i < l.size(); i++) {
        if (l[i] < vsize) {
            transformed[i] = sanitizedtext[vsize - l[i]];
        } else if (l[i] == vsize) {
            primaryindex = encodeUL(i);
            transformed[i] = sanitizedtext[0];
        } else {                        // if (l[i] == vsize + 1)
            sentinelindex = encodeUL(i);
            transformed[i] = 0;         // Dummy character
        }
    }

    return primaryindex + sentinelindex + transformed;
}

bwtzip::ukkonen::SuffixTree::~SuffixTree() {
    sanitizedtext -= 257;
    delete[] sanitizedtext;
}

// Ukkonen: temp.left = k'  temp.right = p'  temp.child = s'
pair<SuffixNode *, long> bwtzip::ukkonen::SuffixTree::canonize(SuffixNode * s, long k, long p) {
    if (p >= k) {
        Transition temp = s->children.findTransition(sanitizedtext[k]);

        while (temp.right - temp.left <= p - k) {
            k = k + temp.right - temp.left + 1;
            s = temp.child;
            if (k <= p) {
                temp = s->children.findTransition(sanitizedtext[k]);
            }
        }
    }

    return make_pair(s, k);
}

pair<bool, SuffixNode *> bwtzip::ukkonen::SuffixTree::test_and_split(SuffixNode * s, long k, long p, short t) {
    if (k <= p) {
        Transition temp = s->children.findTransition(sanitizedtext[k]);
        long kprime = temp.left;
        long pprime = temp.right;
        SuffixNode * sprime = temp.child;

        if (t == sanitizedtext[kprime + p - k + 1]) {
            return make_pair(true, s);
        } else {
            SuffixNode * r = new SuffixNode;

            s->children.insertTransition(
                Transition(sanitizedtext[kprime], kprime, kprime + p - k, r));
            r->children.insertTransition(
                Transition(sanitizedtext[kprime + p - k + 1], kprime + p - k + 1, pprime, sprime));

            return make_pair(false, r);
        }
    }

    return make_pair(s->children.doesTransitionExist(t), s);
}

pair<SuffixNode *, long> bwtzip::ukkonen::SuffixTree::update(pair<SuffixNode *, long> mypair, long i) {
    SuffixNode * oldr = root;
    pair<bool, SuffixNode *> temp = test_and_split(mypair.first, mypair.second, i - 1, sanitizedtext[i]);
    bool endpoint = temp.first;
    SuffixNode * r = temp.second;
    while (!endpoint) {
        r->children.insertTransition(Transition(sanitizedtext[i], i, infinity, NULL));

        if (oldr != root) {
            oldr->link = r;
        }
        oldr = r;
        mypair = canonize(mypair.first->link, mypair.second, i - 1);
        temp = test_and_split(mypair.first, mypair.second, i - 1, sanitizedtext[i]);
        endpoint = temp.first;
        r = temp.second;
    }
    if (oldr != root) {
        oldr->link = mypair.first;
    }
    return mypair;
}

short * bwtzip::ukkonen::SuffixTree::initText(const vector<unsigned char>& v) {
    short * ret = new short[v.size() + 258];

    for (size_t i = 0; i < 257; i++) {
        ret[i] = 256 - i;
    }

    ret += 257;

    for (size_t i = 0; i < v.size(); i++) {
        ret[i] = v[i];
    }

    ret[v.size()] = 256;    // Sentinel character

    return ret;
}

// ********************* WRAPPER FUNCTIONS

vector<unsigned char>& bwtzip::bwtStree(vector<unsigned char>& v) {
    if (v.size() < 2) {
        cout << "ERROR: vector of length less than 2 passed to bwtzip::bwtStree()." << endl;
        exit(EXIT_FAILURE);
    }

    SuffixNode::displayInstances();
    {
        SuffixTree suffixtree(v);
        SuffixNode::displayInstances();
        v = suffixtree.getTransformed();
    }
    SuffixNode::displayInstances();

    return v;
}
