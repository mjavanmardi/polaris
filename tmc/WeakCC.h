#include "tmc/DataStructures.h"

namespace polaris {
	/// This class is used to calculate a weak connected components of the Graph.
class WeakCC
{
private:
	std::unordered_map<NodeID, bool> marked;
	std::unordered_map<NodeID, int> component_id;
	std::unordered_map<int, std::vector<polaris::LinkID>> link_component;
	int count;
	Digraph* g_reverse;
	void dfs(const polaris::Digraph& g, NodeID v)
	{
		marked[v] = true;
		component_id[v] = count;
		Digraph::AdjListRow l = g.Adj(v);
		Digraph::AdjListRow l_reverse = g_reverse->Adj(v);
		for (Digraph::AdjListRowIt it = l.cbegin();  it!=l.cend(); ++it)
		{
			if ( !Marked(it->downstream_node) ) 
			{
				link_component[count].push_back(it->dir_link_uid);
				dfs(g, it->downstream_node);
			}
		}
		for (Digraph::AdjListRowIt it = l_reverse.cbegin();  it!=l_reverse.cend(); ++it)
		{
			if ( !Marked(it->downstream_node) ) 
			{	
				link_component[count].push_back(it->dir_link_uid);
				dfs(g, it->downstream_node);
			}
		}
	}
	bool const Marked(const NodeID& v)
	{
		std::unordered_map<NodeID, bool>::iterator res = marked.find(v);
		if (res == marked.end())
		{
			return false;
		}
		else
			return res->second;
	}
public:
	WeakCC()
	{

	}
	void Clear()
	{
		marked.clear();
		component_id.clear();
		link_component.clear();
		count = 0;
		g_reverse = NULL;
	}

	//Runs the algorithm on the graph g
	void Run(const Digraph& g)
	{
		count = 0;
		Digraph::NodeContainer nodes = g.AllNodes();
		g_reverse = g.reverse();
		for (Digraph::NodeContainer::iterator it = nodes.begin(); it!=nodes.end();++it)
		{
			if (!Marked(*it))
			{
				dfs(g, *it);
				count++;

			}
		}
	}
	/// Returns the total number of components
	int Count()
	{
		return count;
	}	
	/// Returns the id of the component to which node v belongs
	int Id(NodeID v) const
	{
		return component_id.at(v);
	}
	/// This is the result of the algorithm, a map: component_id -> component_links
	const std::unordered_map<int, std::vector<polaris::LinkID> >& LinkComponents() const
	{
		return link_component;
	}
	
};

}
