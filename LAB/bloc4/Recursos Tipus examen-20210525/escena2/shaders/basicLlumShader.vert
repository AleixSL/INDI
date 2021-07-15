#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;
uniform int tipusFocus;

// Material vaca
uniform int matvaca;
vec3 matamb1 = vec3(0.3,0.3,0.3);
vec3 matdiff1 = vec3(0.8,0.8,0.8);
vec3 matspec1 = vec3(0.8,0.8,0.8);
float matshin1 = 100;

// Valors per als components que necessitem dels focus de llum
vec3 colFocus = vec3(0.0, 1.0, 1.0);
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
vec3 posFocus = vec3(1, 1, 1); 
vec3 colFocus2 = vec3(1.0, 1.0, 1.0);

out vec3 fcolor;

vec3 matambtemp;
vec3 matdifftemp;
vec3 matspectemp;
float matshintemp;
vec3 colFocusTemp;



vec3 Lambert (vec3 NormSCO, vec3 L) 
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matambtemp;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * matdifftemp * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO) 
{
    // Els vectors estan normalitzats


    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCO, L);

    // Calculem R i V
    if (dot(NormSCO,L) < 0)
      return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: normalize (2.0*dot(NormSCO,L)*NormSCO - L);
    vec3 V = normalize(-vertSCO.xyz);

    if ((dot(R, V) < 0) || (matshintemp == 0))
      return colRes;  // no hi ha component especular
    
    // Afegim la component especular
    float shine = pow(max(0.0, dot(R, V)), matshintemp);
    return (colRes + matspectemp * colFocus * shine);
}

void main()
{	
    vec4 focusSCO;
    if(matvaca == 0) {
        matambtemp = matamb1;
        matdifftemp = matdiff1;
        matspectemp = matspec1;
        matshintemp = matshin1;
    }
    else if(matvaca == 1) {
        matambtemp = matamb;
        matdifftemp = matdiff;
        matspectemp = matspec;
        matshintemp = matshin;
    }

//    fcolor = matdiff;
    mat3 normalMatrix = inverse(transpose(mat3(view * TG)));
    vec3 NormSCO = normalize(normalMatrix * normal);
    vec4 vertSCO = view * TG * vec4(vertex, 1.0);
    if(tipusFocus == 1) {
        colFocusTemp = colFocus2;
        focusSCO = view * vec4(posFocus, 1.0);   // Focus d'escena
    }
    else {
        colFocusTemp = colFocus;
        focusSCO = vec4(posFocus, 1.0);            // Focus de càmera
    }
    vec4 L = focusSCO - vertSCO;
    vec3 Lxyz = normalize(L.xyz);

    fcolor = Lambert(NormSCO, Lxyz);
    fcolor = Phong(NormSCO, Lxyz, vertSCO);
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
