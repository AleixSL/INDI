#version 330 core

//in vec3 fcolor;

in vec4 vertexFS;
in vec3 normalFS;
in vec3 matambFS;
in vec3 matdiffFS;
in vec3 matspecFS;
in float matshinFS;

out vec4 FragColor;

uniform mat4 view;
uniform vec3 colFocus;
uniform vec3 posFocus;
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);

uniform bool focusEscena;

vec3 Lambert (vec3 NormSCO, vec3 L)
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matambFS;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * matdiffFS * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO)
{
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCO, L);

    // Calculem R i V
    if ((dot(NormSCO,L) < 0) || (matshinFS == 0))
      return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
    vec3 V = normalize(-vertSCO.xyz); // perquè la càmera està a (0,0,0) en SCO

    if (dot(R, V) < 0)
      return colRes;  // no hi ha component especular

    float shine = pow(max(0.0, dot(R, V)), matshinFS);
    return (colRes + matspecFS * colFocus * shine);
}

void main()
{	
    vec3 LFS;
    if(focusEscena) {
        vec4 posFocusNou = view * vec4(posFocus, 1.0);
        LFS = normalize((posFocusNou - vertexFS).xyz);
    }
    else {
        LFS = normalize(posFocus - vertexFS.xyz);
    }
    vec3 novaNormalFS = normalize(normalFS);
    FragColor = vec4(Phong(novaNormalFS, LFS, vertexFS), 1);
}
