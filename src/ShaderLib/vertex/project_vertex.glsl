#ifdef USE_SKINNING

	vec4 mvPosition = viewMatrix * modelMatrix * skinned;

#else

	vec4 mvPosition = viewMatrix * modelMatrix * vec4( transformed, 1.0 );

#endif

gl_Position = projectionMatrix * mvPosition;
