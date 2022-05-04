#include "global.hpp"
#include "gfx.hpp"

void gfx::draw_circle(SDL_Point center, int radius, SDL_Color col)
{
	int offsetx, offsety, d;
	int status;

	offsetx = 0;
	offsety = radius;
	d = radius - 1;
	status = 0;

	while (offsety >= offsetx)
	{
		SDL_SetRenderDrawColor(global::renderer, col.r, col.g, col.b, col.a);

		status += SDL_RenderDrawLine(global::renderer, center.x - offsety, center.y + offsetx, center.x + offsety, center.y + offsetx);
		status += SDL_RenderDrawLine(global::renderer, center.x - offsetx, center.y + offsety, center.x + offsetx, center.y + offsety);
		status += SDL_RenderDrawLine(global::renderer, center.x - offsetx, center.y - offsety, center.x + offsetx, center.y - offsety);
		status += SDL_RenderDrawLine(global::renderer, center.x - offsety, center.y - offsetx, center.x + offsety, center.y - offsetx);

		if (status < 0)
		{
			status = -1;
			break;
		}

		if (d >= 2 * offsetx)
		{
			d -= 2 * offsetx + 1;
			offsetx += 1;
		}
		else if (d < 2 * (radius - offsety))
		{
			d += 2 * offsety - 1;
			offsety -= 1;
		}
		else
		{
			d += 2 * (offsety - offsetx - 1);
			offsety -= 1;
			offsetx += 1;
		}
	}
}