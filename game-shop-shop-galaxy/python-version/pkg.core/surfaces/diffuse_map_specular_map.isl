in {
	tex2D diffuse_map;
	tex2D specular_map;
	float glossiness = 0.5;
}

variant {
	vertex {
		out {
			vec2 v_uv;
		}

		source %{
			v_uv = vUV0;
		%}
	}

	pixel {
		source %{
			vec4 diffuse_color = texture2D(diffuse_map, v_uv);
			vec4 specular_color = texture2D(specular_map, v_uv);

			%diffuse% = diffuse_color;
			%specular% = specular_color;
			%glossiness% = glossiness;
		%}
	}
}
