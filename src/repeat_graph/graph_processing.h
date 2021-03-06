//(c) 2016 by Authors
//This file is a part of ABruijn program.
//Released under the BSD license (see LICENSE file)

//This module provides a few repeat graph processing functions
//that do no require reads alignment (e.g. only graph sctructure is used)

#pragma once

#include "repeat_graph.h"
#include "repeat_resolver.h"

//Represents an unbranching path in the graph.
//Used to represent contigs and more
struct UnbranchingPath
{
	UnbranchingPath(const GraphPath& path, 
					FastaRecord::Id id = FastaRecord::ID_NONE,
		   			bool circular = false, int length = 0, int meanCoverage = 0):
		   	path(path), id(id), circular(circular), repetitive(false), 
			length(length), meanCoverage(meanCoverage) {}

	std::string name() const
	{
		return "contig_" + std::to_string(id.signedId());
	}

	std::string nameUnsigned() const
	{
		std::string idTag = id.strand() ? std::to_string(id.signedId()) : 
										  std::to_string(id.rc().signedId());
		return "contig_" + idTag;
	}

	std::string edgesStr() const
	{
		if (path.empty()) return "";

		std::string contentsStr;
		for (auto& edge : path)
		{
			contentsStr += std::to_string(edge->edgeId.signedId()) + " -> ";
		}

		contentsStr.erase(contentsStr.size() - 4);
		return contentsStr;
	}

	bool isLooped() const
	{
		return path.front()->nodeLeft == path.back()->nodeRight;
	}

	GraphNode*& nodeLeft()
	{
		return path.front()->nodeLeft;
	}

	GraphNode*& nodeRight()
	{
		return path.back()->nodeRight;
	}

	GraphPath path;
	FastaRecord::Id id;
	bool circular;
	bool repetitive;
	int32_t length;
	int32_t meanCoverage;
};

//A class for basic repeat graph processing
//Condencing edges, collapsing bulges, trimming tips etc.
class GraphProcessor
{
public:
	GraphProcessor(RepeatGraph& graph, const SequenceContainer& asmSeqs,
				   const SequenceContainer& readSeqs):
		_graph(graph), _asmSeqs(asmSeqs), _readSeqs(readSeqs) {}

	void simplify();
	void fixChimericJunctions();
	void trimTips();
	std::vector<UnbranchingPath> getUnbranchingPaths() const;
	std::vector<UnbranchingPath> getEdgesPaths() const;

private:
	void condenceEdges();
	void collapseBulges();

	RepeatGraph& _graph;
	const SequenceContainer& _asmSeqs;
	const SequenceContainer& _readSeqs;
};
