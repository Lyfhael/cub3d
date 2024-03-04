#include "parsing.h"
#include "raycasting.h"

int	main(int ac, char **ags)
{
	t_pm	pm;

	if (!parsing(&pm, ac, ags))
		return (FAILURE);
	ray_casting(&pm);
	_c3d_free_t_pm(&pm);
	return (SUCCESS);
}
