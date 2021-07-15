#version 330 core

in vec4 vertexFS;
in vec3 normalFS;
in vec3 matambFS;
in vec3 matdiffFS;
in vec3 matspecFS;
in float matshinFS;

out vec4 FragColor;

uniform mat4 view;
uniform mat4 TGAvio;
uniform vec3 colFocus;
uniform vec3 posFocus[3];
uniform int numFocus;
uniform vec3 colReactor;
uniform vec3 posReactor;
vec3 llumAmbient = vec3(.1, .1, .1);


vec3 Lambert (vec3 NormSCO, vec3 L, bool reactor)
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matambFS;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0) {
        if (!reactor) colRes = colRes + colFocus * matdiffFS * dot (L, NormSCO);
        else if(reactor) colRes = colRes + colReactor * matdiffFS * dot (L, NormSCO);
    }

    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO, vec3 colRes)
{
    // Els vectors estan normalitzats
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

vec3 calculaReactor() {

    vec4 posReactorNou = view * TGAvio * vec4(posReactor, 1.0);
    vec3 LFSR = normalize((posReactorNou - vertexFS).xyz);
    vec3 novaNormalReactorFS = normalize(normalFS);
    vec3 colResReactor = Lambert(novaNormalReactorFS, LFSR, true);
    return colResReactor;
}

vec3 calculaFocus() {

    vec3 sumaPosFoc = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < numFocus; ++i) {
        sumaPosFoc += posFocus[i];
    }

    vec4 posFocusNou = view * vec4(sumaPosFoc, 1.0);
    vec3 LFS = normalize((posFocusNou - vertexFS).xyz);
    vec3 novaNormalFS = normalize(normalFS);

    vec3 colRes = Lambert(novaNormalFS, LFS, false);
    vec3 res = colRes;
    for(int i = 0; i < numFocus; ++i) {
        vec4 posFocusNou = view * vec4(posFocus[i], 1.0);
        vec3 LFS = normalize((posFocusNou - vertexFS).xyz);
        vec3 novaNormalFS = normalize(normalFS);
        if(i < 1) colRes = vec3(0.0, 0.0, 0.0);
        res += Phong(novaNormalFS, LFS, vertexFS, colRes);
    }

    return res;
}

void main()
{
    vec3 resFocus = calculaFocus();
    vec3 resReactor = calculaReactor();
    vec3 res = resFocus + resReactor;
    FragColor = vec4(res, 1.0);

}
