#version 330 core

out vec4 FragColor;

in vec3 FragPos;

void main()
{
	float height = FragPos.y;
    vec3 color;

    if (height < 1.0) {
        color = vec3(0.2, 0.5, 0.8); // Voda / Pesak (Plava/Žuta)
    } else if (height < 5.0) {
        color = vec3(0.2, 0.7, 0.2); // Trava (Zelena)
    } else if (height < 10.0) {
        color = vec3(0.5, 0.4, 0.3); // Stene (Braon)
    } else {
        color = vec3(0.9, 0.9, 0.9); // Sneg (Bela)
    }

    FragColor = vec4(color, 1.0);
}