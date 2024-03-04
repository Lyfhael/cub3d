#include "parsing.h"
#include "raycasting.h"

int	main(int ac, char **ags)
{
	t_c3d	c3d;

	if (!parsing(&c3d, ac, ags))
		return (FAILURE);
	ray_casting(&c3d);
	_c3d_free_t_c3d(&c3d);
	return (SUCCESS);
}
