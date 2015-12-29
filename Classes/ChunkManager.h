#ifndef __CHUNK_MANAGER_H__
#define __CHUNK_MANAGER_H__

#include "CommonDefinition.h"
#include "cocos2d.h"
#include "CommonDefinition.h"
#include "Chunk.h"
#include <vector>


class ChunkManager
{
public:
	ChunkManager(Node* parent);

	Vec2 rotatedVector(float angle);

	//give manager hints to update chunks.
	void updateChunks(const Point& point, bool visible = true);

	virtual ~ChunkManager();
protected:
	cocos2d::Node* _parent;

	static GridCoordinate convertPointToGrid(const Point& point) ;
	static int convertCoorToIndex(const GridCoordinate& grid);

	//check whether current chunks contain it.
	bool contains(const Point& point) const;

	//initialize a new chunk this point fall onto.
	Chunk* newChunk(const Point& interior);

	void discardInvisibleChunk(const Point& pos);

	Vector<Chunk*> _chunks;
};

#endif