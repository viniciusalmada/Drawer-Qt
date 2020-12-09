#ifndef DRAWER_HED_H
#define DRAWER_HED_H

#include <vector>
#include <map>
#include <set>

namespace HED {
	
	class EulerOp;
	
	class HEDVertex {
	public:
		double x{};
		double y{};
		int he = -1;
		
		explicit HEDVertex();
		
		explicit HEDVertex(double x, double y);
	};
	
	class HEDHalfedge {
	public:
		int vtx = -1;
		int edge = -1;
		int loop = -1;
		int next = -1;
		
		explicit HEDHalfedge();
		
		explicit HEDHalfedge(int vertexId);
	};
	
	class HEDFace {
	public:
		int loopOut = -1;
		int loopIn = -1;
		
		explicit HEDFace();
	};
	
	class HEDLoop {
	public:
		int he = -1;
		int face = -1;
		int next = -1;
		
		explicit HEDLoop();
		
		explicit HEDLoop(int face);
	};
	
	class HEDEdge {
	public:
		int he0 = -1;
		int he1 = -1;
		
		explicit HEDEdge();
		
		explicit HEDEdge(int h0, int h1);
	};
	
	class Model {
	private:
		std::map<int, HEDVertex> mVertices;
		std::map<int, HEDHalfedge> mHalfedges;
		std::map<int, HEDEdge> mEdges;
		std::map<int, HEDFace> mFaces;
		std::map<int, HEDLoop> mLoops;
		
		int mVerticesCount = 0;
		int mHalfedgesCount = 0;
		int mEdgesCount = 0;
		int mFacesCount = 0;
		int mLoopCount = 0;
	
	public:
		
		friend class EulerOp;
		
		explicit Model();
		
		~Model();
		
		int newVertex(double x, double y);
		
		int newHalfedge(int vertexId);
		
		int newEdge(int he0, int he1);
		
		int newFace();
		
		int newLoop(int face);
		
		void updateHalfedgeOfVertex(int vtxId, int heId);
		
		void updateVertexOfHalfedge(int he, int vtx);
		
		int halfedgeFromVertex(int vtxId);
		
		int edgeFromHalfedge(int he);
		
		int mateOfHalfedge(int he);
		
		void setHalfedgesOfEdge(int edgeId, int he0, int he1);
		
		HED::HEDEdge getEdge(int i);
		
		int getEdgeFromHe(int he);
		
		void setNextOfHe(int he, int next);
		
		void setEdgeOfHe(int he, int edge);
		
		int getLoopFromHe(int he);
		
		void setLoopOfHe(int he, int loop);
		
		int getHe0FromEdge(int edge);
		
		int getHe1FromEdge(int edge);
		
		int getFaceFromLoop(int loop);
		
		void setVtxOfHe(int he, int vtx);
		
		int getNextFromHe(int he);
		
		int getVtxFromHe(int he);
		
		void setHeOfVtx(int vtx, int he);
		
		std::set<int> getNearEdgesFromVtx(int vtx);
		
		int getHeFromVtx(int vtx);
		
		std::pair<double, double> getPtFromVtx(int vtx);
		
		std::vector<std::pair<int, double>> getEdgesAngles(const std::set<int>& edges, int vtx);
		
		int getHeFromEdgeOppositeVtx(int edge, int vtx);
		
		int getHeFromEdgeFromVtx(int edge, int vtx);
	};
}

#endif