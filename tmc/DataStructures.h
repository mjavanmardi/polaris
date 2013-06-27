#pragma once
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <set>
#include "utils/types.h"
#include "Exceptions.h"

namespace polaris {
///  Information abut graph nodes. For now only id is stored.
struct TmcNode {
public:
	TmcNode(polaris::NodeID id_) : id(id_){}
	polaris::NodeID id;
};
/// Contains Link information
struct AdjNode
{
	NodeID downstream_node; ///< The end node of the link
	double cost; ///< Cost of traversing the link
	LinkID dir_link_uid; ///< id of the link. This attribute is not required by algorithms but needed to be integrated with other polaris components
};

/// This class defines a directed weighted graph. 
/// In TMC graph is used to fined directed components and weights are not required, however in case those are needed in the future, I included them
class Digraph {
public:
	typedef std::vector<AdjNode> AdjListRow;
	typedef std::unordered_map<polaris::NodeID, AdjListRow> AdjList;
	typedef AdjList::const_iterator AdjListIt;
	typedef AdjListRow::const_iterator AdjListRowIt;
	typedef std::set<NodeID> NodeContainer;
	
	Digraph() {this->v = 0; this->e = 0;}
	/// Returns number of vertices in the graph. The vertices counting is not cleanly implemented, so the result is not to be trusted
	const int V() {return (int)this->all_nodes.size();}
	/// Returns number of edges 
	const int E() {return this->e;}
	/// Use this function to populate the graph
	void addEdge(polaris::NodeID v, polaris::NodeID w, float cost, LinkID dir_link_uid = -1)
	{
		adj_node.cost = (double) cost; 
		adj_node.dir_link_uid = dir_link_uid;
		adj_node.downstream_node = w;
		adj[v].push_back(adj_node);
		adj[w]; // create empty adjacency list to keep the Adj consistent
		e++;
		all_nodes.insert(v);
		all_nodes.insert(w);
	}
	/// Returns a set of all of the nodes defined in the graph
	const NodeContainer AllNodes() const
	{
		return all_nodes;
	}
	/// Returns the outgoing edges that are adjacent to the node v
	const AdjListRow& Adj(polaris::NodeID v) const
	{
		const AdjListIt it = adj.find(v);
		if (it == adj.end())
			throw no_link_ex;
		else
			return adj.at(v);
	}
	/// Builds a new graph with directions of all of the edges reversed and returns a pointer to it
	Digraph* reverse() const
	{
		Digraph* result = new Digraph();
		for (auto adj_list_it = adj.cbegin(); adj_list_it != adj.cend(); ++adj_list_it)
		{
			polaris::NodeID dest = adj_list_it->first;
			for (auto adj_row_it = adj_list_it->second.cbegin(); adj_row_it != adj_list_it->second.cend(); ++adj_row_it)
			{
				result->addEdge(adj_row_it->downstream_node, dest, (float)adj_row_it->cost, adj_row_it->dir_link_uid);
			}
		}
		return result;
	}
	void print() const
	{
		for (auto adj_list_it = adj.cbegin(); adj_list_it != adj.cend(); ++adj_list_it)
		{
			polaris::NodeID dest = adj_list_it->first;
			for (auto adj_row_it = adj_list_it->second.cbegin(); adj_row_it != adj_list_it->second.cend(); ++adj_row_it)
			{
				cout << dest << ',' << adj_row_it->downstream_node << '\n'; 
			}
		}
	}
	void Clear()
	{
		v = 0;
		e = 0;
		// maps a node to a vector of 2-tuples. Each tuple corresponds to an adjacent node
		// first element of the tuple contains the node id and the second constrains link cost
		adj.clear();
		all_nodes.clear();
	}

private:
	int v;
	int e;
	// maps a node to a vector of 2-tuples. Each tuple corresponds to an adjacent node
	// first element of the tuple contains the node id and the second constrains link cost
	AdjList adj;
	NodeContainer all_nodes;
	AdjNode adj_node;

};
}