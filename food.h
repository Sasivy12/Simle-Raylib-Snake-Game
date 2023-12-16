
class Food
{
public:
	Vector2 position = { 5, 9 };

	Food()
	{
		position = GenerateRandPos();
	}

	void Draw();

	Vector2 GenerateRandPos();
};

