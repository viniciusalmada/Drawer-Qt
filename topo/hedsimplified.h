#ifndef DRAWER_HED_H
#define DRAWER_HED_H

#include <vector>
#include <QtCore/QPointF>
#include <functional>

namespace HEDSimpl {
	
	class HEDVertex {
	public:
		QPointF pt;
		int he = -1;
		
		explicit HEDVertex(QPointF pt);
		
		~HEDVertex() = default;
	};
	
	class HEDHalfedge {
	public:
		int vtx = -1;
		int edge = -1;
		int tri = -1;
		int next = -1;
		
		explicit HEDHalfedge(int vertexId = -1);
		
		~HEDHalfedge() = default;
	};
	
	class HEDTriangle {
	public:
		int he0 = -1;
		int he1 = -1;
		int he2 = -1;
		
		explicit HEDTriangle(int h0, int h1, int h2);
		
		~HEDTriangle() = default;
	};
	
	class HEDEdge {
	public:
		int he0 = -1;
		int he1 = -1;
		
		explicit HEDEdge(int h0, int h1);
		
		~HEDEdge() = default;
	};
	
	class Model {
	public:
		
		std::vector<HEDVertex> mVertices;
		std::vector<HEDHalfedge> mHalfedges;
		std::vector<HEDEdge> mEdges;
		std::vector<HEDTriangle> mTriangles;
		
		bool continueCollectEdges(std::vector<int>& edges, int& he, const int& firstEdge);
		
		void collectEdgesReverse(std::vector<int>& edges, int& he);
	
	public:
		
		explicit Model();
		
		int newVertex(const QPointF& pt);
		
		int newHalfedge(int vertexId);
		
		int newEdge(int he0, int he1);
		
		int newTriangle(int he0, int he1, int he2);
		
		void updateHalfedgeOfVertex(int vtxId, int heId);
		
		void updateHalfedgesOfTriangle(int triId, int he0, int he1, int he2);
		
		void updateVertexOfHalfedge(int he, int vtx);
		
		int halfedgeFromTriangle(int triId, int pos) const;
		
		int vertexOfTriangle(int triId, int pos) const;
		
		std::vector<int> verticesOfTriangles(int triId) const;
		
		int halfedgeFromVertex(int vtxId);
		
		int edgeFromHalfedge(int he);
		
		std::vector<int> nearEdges(const std::vector<int>& vertices);
		
		int mateOfHalfedge(int he);
		
		int triangleOfHalfedge(int he);
		
		int nextOfHalfedge(int he);
		
		QPointF pointOfHalfedge(int he);
		
		int vertexOfHalfedge(int he);
		
		QPointF pointOfVertex(int vtx);
		
		bool isEdgeLegal(int edge, const std::function<bool(Model&, int)>& predicate);
		
		std::vector<int> makeFlip(int edgeId);
		
		bool isEdgeNearTwoFaces(int edgeId);
		
		int findEdgeThatContainsPoint(const QPointF& pt);
		
		int findTriangleThatContainsPoint(const QPointF& pt);
		
		std::vector<int> splitEdge(int edgeId, const QPointF& pt);
		
		std::vector<int> splitTriangle(int tri, QPointF pt);
		
		void setHalfedgesOfEdge(int edgeId, int he0, int he1);
		
		HEDSimpl::HEDEdge getEdge(int i);
		
		std::vector<QPointF> getVerticesTriangle(int tri) const;
	};
}

#endif