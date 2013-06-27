#include "tmc/DataStructures.h"

namespace polaris{
class DFS
{
private:
	std::unordered_map<NodeID, bool> marked;
	std::vector<NodeID> reachable;
	void dfs(polaris::Digraph g, NodeID v)
	{
		marked[v] = true;
		reachable.push_back(v);
		Digraph::AdjListRow l = g.Adj(v);
		for (Digraph::AdjListRowIt it = l.cbegin();  it!=l.cend(); ++it)
		{
			if (!Marked(it->downstream_node)) dfs(g, it->downstream_node);
		}
	}
public:
	DFS (const Digraph& g,  NodeID source)
	{
		dfs(g, source);
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
	const std::vector<NodeID>& Reachable()
	{
		return reachable;
	}
//	const std::vector<LinkID>& ReachableLink()
//	{
//		std::vector<LinkID> res;
//
//		for (/		return reachable;
//	}

};
}