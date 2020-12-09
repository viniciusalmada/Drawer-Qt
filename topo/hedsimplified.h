#ifndef DRAWER_HED_H
#define DRAWER_HED_H

#include <vector>
#include <QtCore/QPointF>
#include <functional>

namespace HEDSimpl {
	class Model {
		
		class HEDVertex {
		public:
			QPointF pt;
			int he = -1;
			
			explicit HEDVertex(QPointF pt);
		};
		
		class HEDHalfedge {
		public:
			int vtx = -1;
			int edge = -1;
			int tri = -1;
			int next = -1;
			
			explicit HEDHalfedge(int vertexId);
		};
		
		class HEDTriangle {
		public:
			int he0 = -1;
			int he1 = -1;
			int he2 = -1;
			
			explicit HEDTriangle(int h0, int h1, int h2);
		};
		
		class HEDEdge {
		public:
			int he0 = -1;
			int he1 = -1;
			
			explicit HEDEdge(int h0, int h1);
		};
		
		std::vector<HEDVertex> mVertices;
		std::vector<HEDHalfedge> mHalfedges;
		std::vector<HEDEdge> mEdges;
		std::vector<HEDTriangle> mTriangles;
		
		bool continueCollectEdges(std::vector<int>& edges, int& he, const int& firstEdge);
		
		void collectEdgesReverse(std::vector<int>& edges, int& he);
	
	public:
		
		explicit Model();
		
		~Model();
		
		int newVertex(const QPointF& pt);
		
		int newHalfedge(int vertexId);
		
		int newEdge(int he0, int he1);
		
		int newTriangle(int he0, int he1, int he2);
		
		void updateHalfedgeOfVertex(int vtxId, int heId);
		
		void updateHalfedgesOfTriangle(int triId, int he0, int he1, int he2);
		
		void updateVertexOfHalfedge(int he, int vtx);
		
		int halfedgeFromTriangle(int triId, int pos);
		
		int vertexOfTriangle(int triId, int pos);
		
		std::vector<int> verticesOfTriangles(int triId);
		
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
		
		HEDSimpl::Model::HEDEdge getEdge(int i);
	};
}

#endif