#ifndef FT_GRAPHICS_VERTEX_LOCATION
#	define FT_GRAPHICS_VERTEX_LOCATION

/*
**	A garder impérativement sous la forme de header C,
**	afin de pouvoir l'inclure dans les shaders.
*/

#	define FT_VERTEX_LOCATION_POSITION		0
#	define FT_VERTEX_LOCATION_UV			1
#	define FT_VERTEX_LOCATION_COLOR			2
/* A mettre à jour à chaque ajout-suppression de Location */
#	define FT_VERTEX_LOCATION_COUNT			3

#endif // DJO_GRAPHICS_VERTEX_LOCATION
