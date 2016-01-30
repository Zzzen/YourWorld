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
	static ChunkManager* getInstance();

	Vec2 rotatedVector(float angle);

	//give manager hints to update chunks.
	void updateChunks(const Point& point, bool visible = true);
	static GridCoordinate convertPointToGrid(const Point& point);
	static int convertCoorToIndex(const GridCoordinate& grid);

	void setLayer(Node* node) { _parent = node; }
	virtual ~ChunkManager();

	static const float CHUNK_WIDTH;
	static const float CHUNK_HEIGHT;
private:
	ChunkManager() {}

	Node* _parent;

	//check whether current chunks contain it.
	bool contains(const Point& point) const;

	//initialize a new chunk this point fall onto.
	Chunk* newChunk(const Point& interior);

	void discardInvisibleChunk(const Point& pos);

	Vector<Chunk*> _chunks;
};

#endif