#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

out vec4 vertexFS;
out vec3 normalFS;
out vec3 matambFS;
out vec3 matdiffFS;
out vec3 matspecFS;
out float matshinFS;


uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

//uniform vec3 colFocus;
//uniform vec3 posFocus;

// Valors per als components que necessitem dels focus de llum
//vec3 colFocus = vec3(0.8, 0.8, 0.8);
//vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
//vec3 posFocus = vec3(1, 0, 1);  // en SCA

//out vec3 fcolor;



void main()
{	
//    fcolor = matdiff;
//    mat3 normalMatrix = inverse(transpose(mat3(view * TG)));
//    vec3 NormSCO = normalize(normalMatrix * normal);
//    vec4 vertSCO = view * TG * vec4(vertex, 1.0);
//    vec4 focusSCO = view * vec4(posFocus, 1.0);   // Focus d'escena
//    vec4 focusSCO = vec4(posFocus, 1.0);            // Focus de càmera
//    vec4 L = focusSCO - vertSCO;
//    vec3 Lxyz = normalize(L.xyz);

//    fcolor = Lambert(NormSCO, Lxyz);
//    fcolor = Phong(NormSCO, Lxyz, vertSCO);

    vertexFS = view * TG * vec4(vertex, 1.0);
    normalFS = inverse(transpose(mat3(view * TG))) * normal;

    matambFS = matamb;
    matdiffFS = matdiff;
    matspecFS = matspec;
    matshinFS = matshin;

    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
