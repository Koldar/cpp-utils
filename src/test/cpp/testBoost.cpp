#include "catch.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/heap/d_ary_heap.hpp>

class ImportantInfo {
public:
    int g;
    int h;
    char state;
public:
    ImportantInfo(char s): g{0}, h{0}, state{s} {

    } 
public:
    friend bool operator >(const ImportantInfo& a, const ImportantInfo& b) {
        return a.state > b.state;
    }
};


class HeapNode;

typedef typename boost::heap::d_ary_heap<HeapNode, 
    boost::heap::arity<2>, 
    boost::heap::mutable_<true>
    //implicitly "<" of HeapNode
> BinaryHeap;
typedef typename BinaryHeap::handle_type BinaryHeapHandle;

// node stored in the heap
class HeapNode {
public:
    ImportantInfo* info;
    BinaryHeapHandle handle;
public:
    HeapNode(ImportantInfo* info): info{info}, handle{handle} {

    }
public:
    //use this to avoid having to write explicitly a Comparator function
    friend bool operator <(const HeapNode& a, const HeapNode& b) {
        //the queue in boost are by default maxqueue. So we need to reorder them!
        return *a.info > *b.info;
    }
};


SCENARIO("test boost") {

    GIVEN("boost split") {
        std::string line{"MemTotal:        8008672 kB"};
        std::vector<std::string> split;
        boost::split(split, line, boost::is_any_of(":"));
        REQUIRE(split[1] == std::string{"        8008672 kB"});
    }
    GIVEN("boost d_ary_tree") {
        BinaryHeap queue{};

        auto a = new ImportantInfo{'a'};
        auto b = new ImportantInfo{'b'};
        auto c = new ImportantInfo{'c'};

        HeapNode na{a};
        HeapNode nb{b};
        HeapNode nc{c};

        BinaryHeap::handle_type ha = queue.push(na);
        (*ha).handle = ha;
        //if you deference the handle_type ha you will get a value (or a reference)
        // of HeapNode. However, boost implemented only operator "*" , not "->"

        BinaryHeap::handle_type hb = queue.push(nb);
        (*hb).handle = hb;
        
        BinaryHeap::handle_type hc = queue.push(nc);
        (*hc).handle = hc;

        REQUIRE(queue.size() == 3);

        //to peek the top element, use top. It returns the node associated

        HeapNode x = queue.top();
        ImportantInfo* top = x.info;
        REQUIRE(top->state == 'a');

        //to remove the top ewlement without fetching it, use pop()
        queue.pop();

        x = queue.top();
        top = x.info;
        REQUIRE(top->state == 'b');
        queue.pop();

        x = queue.top();
        top = x.info;
        REQUIRE(top->state == 'c');
        queue.pop();

        REQUIRE(queue.size() == 0);

        delete a;
        delete b;
        delete c;
    }
}