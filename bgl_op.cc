#include <boost/config.hpp>

#include <time.h>
#include <algorithm>
#include <iostream>
#include <iterator>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/utility.hpp>

// [undirectedS | bidirectionalS]
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS>
    BidirectedGraph;
typedef boost::graph_traits<BidirectedGraph>::vertex_descriptor Vertex;
class myvisitor : public boost::bfs_visitor<> {
 public:
  template <class Vertex, class BidirectedGraph>
  void discover_vertex(Vertex v, BidirectedGraph&) {
    std::cout << v << std::endl;
  }
};

int main(int argc, char* argv[]) {
  std::vector<std::pair<uint32_t, uint32_t> > edges;
  for (int i = 3; i < 10; ++i) {
    std::pair<uint32_t, uint32_t> edge(i - 1, i);
    edges.push_back(edge);
  }

  BidirectedGraph g(edges.begin(), edges.end(), 9);

  myvisitor vis;
  boost::breadth_first_search(g, boost::vertex(2, g), boost::visitor(vis));
  return 0;
}
