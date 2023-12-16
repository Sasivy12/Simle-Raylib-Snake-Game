#include <deque>
#include <raylib.h>

class Snake
{
public:
	bool getBigger = false;
	std::deque<Vector2> Body = { Vector2{6, 9}, Vector2 {5, 9}, Vector2 {4, 9} };
	Vector2 Direction = { 1, 0 };

	void Draw();
	void Move();
};