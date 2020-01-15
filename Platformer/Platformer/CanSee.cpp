#include "CanSee.h"

bool CanSee(Vector2 originPos, Vector2 targetPos, float x_interval, float y_interval, Level* p_level)
{
	// Test line of sight
	bool reachedTarget = false;
	bool canSee = true;

	Vector2 direction = targetPos - originPos;
	direction /= static_cast<float>(sqrt(static_cast<double>(direction.x) * static_cast<double>(direction.x) + static_cast<double>(direction.y) * static_cast<double>(direction.y)));

	Vector2 testPos = originPos;

	// Can see middle?
	while (!reachedTarget)
	{
		testPos += {direction.x* x_interval, direction.y* y_interval};

		if (p_level->isWall(static_cast<int>(testPos.x), static_cast<int>(testPos.y)))
		{
			canSee = false;
			break;
		}

		Vector2 difference = (targetPos - testPos);
		if (difference.x < 0)
		{
			difference.x *= -1;
		}
		if (difference.y < 0)
		{
			difference.y *= -1;
		}
		if (difference.x < x_interval && difference.y < y_interval)
		{
			reachedTarget = true;
		}
	}

	return canSee;
}
