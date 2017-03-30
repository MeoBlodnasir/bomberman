#ifndef FT_GRAPHICS_VERTEX_LOCATION_H
#	define FT_GRAPHICS_VERTEX_LOCATION_H

/*
**	A garder impérativement sous la forme de header C,
**	afin de pouvoir l'inclure dans des shaders.
*/

#	define FT_VERTEX_LOCATION_POSITION		0
#	define FT_VERTEX_LOCATION_NORMAL		1
#	define FT_VERTEX_LOCATION_UV			2
#	define FT_VERTEX_LOCATION_COLOR			3
/* A mettre à jour à chaque ajout-suppression de Location */
#	define FT_VERTEX_LOCATION_COUNT			4

#endif // FT_GRAPHICS_VERTEX_LOCATION_H
