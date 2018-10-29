// bwtzip_ahuff.cc - bwtzip Adaptive Huffman Coding functions
// GPLed - Joergen Ibsen - http://www.ibsensoftware.com/

#include "bwtzip_common.hh"
#include "bwtzip_ahuff.hh"
using namespace std;
using namespace bwtzip;
using namespace bwtzip::adaptivehuffman;

bwtzip::adaptivehuffman::AHuffTree::AHuffTree()
    : nodes(2 * NUM_SYMBOLS + 1), leafs(NUM_SYMBOLS + 1) {

    nodes[0].child         = 1;
    nodes[0].child_is_leaf = false;
    nodes[0].weight        = 2;
    nodes[0].parent        = -1;

    nodes[1].child         = ESCAPE;
    nodes[1].child_is_leaf = true;
    nodes[1].weight        = 1;
    nodes[1].parent        = 0;
    leafs[ESCAPE]          = 1;

    nodes[2].child         = SENTINEL;
    nodes[2].child_is_leaf = true;
    nodes[2].weight        = 1;
    nodes[2].parent        = 0;
    leafs[SENTINEL]        = 2;

    next_free_node = 3;

    for (int i = 0; i < ESCAPE; ++i) {
        leafs[i] = -1;
    }
}

void bwtzip::adaptivehuffman::AHuffTree::swap_nodes(size_t n1, size_t n2) {
    if (nodes[n1].child_is_leaf) {
        leafs[nodes[n1].child] = n2;
    } else {
        nodes[nodes[n1].child].parent = n2;
        nodes[nodes[n1].child + 1].parent = n2;
    }

    if (nodes[n2].child_is_leaf) {
        leafs[nodes[n2].child] = n1;
    } else {
        nodes[nodes[n2].child].parent = n1;
        nodes[nodes[n2].child + 1].parent = n1;
    }

    Node tmp = nodes[n1];
    nodes[n1] = nodes[n2];
    nodes[n1].parent = tmp.parent;
    tmp.parent = nodes[n2].parent;
    nodes[n2] = tmp;
}

void bwtzip::adaptivehuffman::AHuffTree::rescale() {
    int i, j = 0;

    // scale and collect all leaf nodes at beginning of array
    // note: after scaling they are still sorted
    for (i = 1; i < next_free_node; i++) {
        if (nodes[i].child_is_leaf) {
            nodes[j] = nodes[i];
            nodes[j].weight = (nodes[j].weight + 1) / 2;
            j++;
        }
    }

    i--; // point i to where the last element in the tree should be
    j--; // point j to the last of the sorted leaf nodes

    // init implicit queue
    int queue_l = j, queue_r = j;

    // now rebuild the tree
    while (queue_l) {
        // take smallest node from queue or sorted leafs
        // and copy it to its final place
        // (the queue is favored both to make room for the tree, and also
        //  because it results in a more 'flat' tree)
        if ((queue_l < queue_r) && ((j < 0) || (nodes[queue_r - 1].weight <= nodes[j].weight))) {
            queue_r--;
            nodes[i] = nodes[queue_r];

            // all nodes in the queue are internal, so set parent in children
            nodes[nodes[i].child    ].parent = i;
            nodes[nodes[i].child + 1].parent = i;

            i--;
        } else {
            nodes[i] = nodes[j];

            // update leaf table
            leafs[nodes[i].child] = i;

            i--;
            j--;
        }

        // take smallest node from queue or sorted leafs
        // and copy it to its final place
        if ((queue_l < queue_r) && ((j < 0) || (nodes[queue_r - 1].weight <= nodes[j].weight))) {
            queue_r--;
            nodes[i] = nodes[queue_r];

            // all nodes in the queue are internal, so set parent in children
            nodes[nodes[i].child    ].parent = i;
            nodes[nodes[i].child + 1].parent = i;

            i--;
        } else {
            nodes[i] = nodes[j];

            // update leaf table
            leafs[nodes[i].child] = i;

            i--;
            j--;
        }

        // add new combined (internal) node to queue
        queue_l--;
        nodes[queue_l].weight = nodes[i + 1].weight + nodes[i + 2].weight;
        nodes[queue_l].child_is_leaf = false;
        nodes[queue_l].child = i + 1;
    }

    // setup root and children of root
    nodes[0].parent = -1;
    nodes[1].parent = 0;
    nodes[2].parent = 0;
}

void bwtzip::adaptivehuffman::AHuffTree::add(const int sym) {
    int lightest_node = next_free_node - 1;
    int new_node = next_free_node;
    int zero_weight_node = next_free_node + 1;
    next_free_node += 2;

    nodes[new_node] = nodes[lightest_node];
    nodes[new_node].parent = lightest_node;
    leafs[nodes[new_node].child] = new_node;

    nodes[lightest_node].child            = new_node;
    nodes[lightest_node].child_is_leaf    = false;

    nodes[zero_weight_node].child         = sym;
    nodes[zero_weight_node].child_is_leaf = true;
    nodes[zero_weight_node].weight        = 0;
    nodes[zero_weight_node].parent        = lightest_node;
    leafs[sym] = zero_weight_node;
}

void bwtzip::adaptivehuffman::AHuffTree::update(const int sym) {
    int current_node = leafs[sym];

    while (current_node != -1) {
        nodes[current_node].weight += INCREMENT_VALUE;

        int new_node;

        for (new_node = current_node; new_node > 0; new_node--) {
            if (nodes[new_node - 1].weight >= nodes[current_node].weight) {
                break;
            }
        }

        if (current_node != new_node) {
            swap_nodes(current_node, new_node);
            current_node = new_node;
        }

        current_node = nodes[current_node].parent;
    }

    if (nodes[0].weight == MAX_FREQUENCY) {
        rescale();
    }
}

void bwtzip::adaptivehuffman::AHuffEncoder::encodeSymbol(const int sym) {
    AHuffTree::EncodeIterator ei(&aht, sym);

    unsigned int code = 0;
    unsigned int current_bit = 1;

    while (!ei.isDone()) {
        if (ei.getBit()) code |= current_bit;
        current_bit <<= 1;
        ei.next();
    }

    for (current_bit >>= 1; current_bit; current_bit >>= 1) {
        output.push_back(code & current_bit);
    }

    if (ei.escaped()) {
        for (unsigned int mask = 0x80; mask; mask >>= 1) {
            output.push_back(sym & mask);
        }
        aht.add(sym);
    }

    aht.update(sym);
}

vector<unsigned char> bwtzip::adaptivehuffman::AHuffEncoder::finalize() {
   return output.get_vuc();
}

int bwtzip::adaptivehuffman::AHuffDecoder::decodeSymbol() {
    AHuffTree::DecodeIterator di(&aht);

    while (!di.isDone()) {
        di.next(inputBit());
    }

    int sym = di.getSymbol();

    if (sym == ESCAPE) {
        sym = 0;
        for (unsigned int mask = 0x80; mask; mask >>= 1) {
            if (inputBit()) {
                sym |= mask;
            }
        }
        aht.add(sym);
    }

    aht.update(sym);

    return sym;
}

bool bwtzip::adaptivehuffman::AHuffDecoder::inputBit() {
    if (loc < pvec->size() * 8) {
        bool ret = ((*pvec)[loc / 8] >> (7 - loc % 8)) & 1;
        ++loc;
        return ret;
    } else {
        return 0;
    }
}

vector<unsigned char>& bwtzip::ahuff(vector<unsigned char>& v) {
    AHuffEncoder ahe;

    for (size_t i = 0; i < v.size(); i++) {
        ahe.encodeSymbol(v[i]);
    }
    ahe.encodeSymbol(SENTINEL);

    v = ahe.finalize();
    return v;
}

vector<unsigned char>& bwtzip::unahuff(vector<unsigned char>& v) {
    vector<unsigned char> decompressed;
    int decoded;

    AHuffDecoder ahd(&v);

    while ((decoded = ahd.decodeSymbol()) != SENTINEL) {
        decompressed.push_back(decoded);
    }

    swap(v, decompressed);
    return v;
}
