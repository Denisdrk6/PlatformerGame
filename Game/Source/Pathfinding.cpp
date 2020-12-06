#include "App.h"
#include "PathFinding.h"

#include "Defs.h"
#include "Log.h"

PathFinding::PathFinding() : Module(), map(NULL), lastPath(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.Create("pathfinding");
}

// Destructor
PathFinding::~PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	lastPath.Clear();
	RELEASE_ARRAY(map);

	return true;
}

// Sets up the walkability map
void PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width * height];
	memcpy(map, data, width * height);
}

// Utility: return true if pos is inside the map boundaries
bool PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x/32 >= 0 && pos.x/32 <= (int)width &&
		pos.y/32 >= 0 && pos.y/32 <= (int)height);
}

// Utility: returns true is the tile is walkable
bool PathFinding::IsWalkable(const iPoint& pos, bool Fly) const
{
	uchar t = GetTileAt(pos);


	if (Fly == true) {
		return t != INVALID_WALK_CODE && t > 0;
	}
	else
	{
		return t != INVALID_WALK_CODE && t == 2;
	}

}

// Utility: return the walkability value of a tile
uchar PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y/32 * width) + pos.x/32];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const DynArray<Path>* PathFinding::GetLastPath() const
{
	return &lastPath;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
ListItem<PathNode>* PathList::Find(const iPoint& point) const
{
	ListItem<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
ListItem<PathNode>* PathList::GetNodeLowestScore() const
{
	ListItem<PathNode>* ret = NULL;
	int min = 65535;

	ListItem<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent, direction nextNodeDir) : g(g), h(h), pos(pos), parent(parent), nextNodeDir(nextNodeDir)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& listToFill) const
{
	iPoint cell;
	uint before = listToFill.list.Count();

	// north
	cell.create(pos.x, pos.y + 1);
	if (app->pathfinding->IsWalkable(cell, app->pathfinding->fly_path))
		listToFill.list.Add(PathNode(-1, -1, cell, this, DIR_UP));

	// south
	cell.create(pos.x, pos.y - 1);
	if (app->pathfinding->IsWalkable(cell, app->pathfinding->fly_path))
		listToFill.list.Add(PathNode(-1, -1, cell, this, DIR_DOWN));

	// east
	cell.create(pos.x + 1, pos.y);
	if (app->pathfinding->IsWalkable(cell, app->pathfinding->fly_path))
		listToFill.list.Add(PathNode(-1, -1, cell, this, DIR_RIGHT));

	// west
	cell.create(pos.x - 1, pos.y);
	if (app->pathfinding->IsWalkable(cell, app->pathfinding->fly_path))
		listToFill.list.Add(PathNode(-1, -1, cell, this, DIR_LEFT));

	return listToFill.list.Count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int PathFinding::CreatePath(const iPoint& origin, const iPoint& destination, bool Fly)
{
	//Clear the last path
 	lastPath.Clear();
	// L12b: TODO 1: if origin or destination are not walkable, return -1
	if (IsWalkable(origin, Fly) == false || IsWalkable(destination, Fly) == false) {
		return -1;
	}

	fly_path = Fly;
	// L12b: TODO 2: Create two lists: open, close
	// Add the origin tile to open
	// Iterate while we have tile in the open list

	PathList open;
	PathList close;

	PathNode originNode(0, origin.DistanceManhattan(destination), origin, NULL, DIR_NONE);
	open.list.Add(originNode);

	while (open.list.Count() > 0) {
		// L12b: TODO 3: Move the lowest score cell from open list to the closed list
		ListItem<PathNode>* NodeVisited = open.GetNodeLowestScore();
		close.list.Add(NodeVisited->data);
		open.list.Del(NodeVisited);
		// L12b: TODO 4: If we just added the destination, we are done!
		// Backtrack to create the final path
		// Use the Pathnode::parent and Flip() the path when you are finish
		ListItem<PathNode>* Node = close.list.end;
		if (Node->data.pos == destination && Fly) {
			Path FinalPath(Node->data.pos.x, Node->data.pos.y, Node->data.nextNodeDir);
			lastPath.PushBack(FinalPath);

			const PathNode* PrevNode;
			const PathNode* AuxNode;
			PrevNode = &Node->data;

			for (AuxNode = PrevNode->parent; AuxNode; AuxNode = AuxNode->parent) {
				Path AuxPath(AuxNode->pos.x, AuxNode->pos.y, AuxNode->nextNodeDir);
				if (AuxNode->nextNodeDir != PrevNode->nextNodeDir) {
					lastPath.PushBack(AuxPath);
				}


				PrevNode = AuxNode;
			}
			lastPath.Flip();
			return 1;

		}
		else if (Node->data.pos.x == destination.x && !Fly) {
			Path FinalPath(Node->data.pos.x, Node->data.pos.y, Node->data.nextNodeDir);
			lastPath.PushBack(FinalPath);

			const PathNode* PrevNode;
			const PathNode* AuxNode;
			PrevNode = &Node->data;

			for (AuxNode = PrevNode->parent; AuxNode; AuxNode = AuxNode->parent) {
				if (AuxNode->nextNodeDir != PrevNode->nextNodeDir) {
					Path AuxPath(AuxNode->pos.x, AuxNode->pos.y, AuxNode->nextNodeDir);
					lastPath.PushBack(AuxPath);
				}

				PrevNode = AuxNode;
			}
			lastPath.Flip();
			return 1;
		}
		// L12b: TODO 5: Fill a list of all adjancent nodes
		PathList adjacent;
		Node->data.FindWalkableAdjacents(adjacent);
		ListItem<PathNode>* AdjacentNode = adjacent.list.start;
		// L12b: TODO 6: Iterate adjancent nodes:
		// ignore nodes in the closed list
		// If it is NOT found, calculate its F and add it to the open list
		// If it is already in the open list, check if it is a better path (compare G)
		// If it is a better path, Update the parent
		while (AdjacentNode != NULL) {
			AdjacentNode->data.CalculateF(destination);
			if (close.Find(AdjacentNode->data.pos) != NULL) {
				AdjacentNode = AdjacentNode->next;
				continue;
			}

			if (open.Find(AdjacentNode->data.pos) != NULL) {
				if (AdjacentNode->data.g < open.Find(AdjacentNode->data.pos)->data.g) {

					open.Find(AdjacentNode->data.pos)->data.g = AdjacentNode->data.pos.DistanceTo(origin);

					AdjacentNode = AdjacentNode->next;
					continue;
				}
			}
			else {
				PathNode NextNode(AdjacentNode->data.g, AdjacentNode->data.h, AdjacentNode->data.pos, AdjacentNode->data.parent, AdjacentNode->data.nextNodeDir);
				open.list.Add(NextNode);

				AdjacentNode = AdjacentNode->next;
				continue;
			}

			AdjacentNode = AdjacentNode->next;
		}
	}
	return -1;
}