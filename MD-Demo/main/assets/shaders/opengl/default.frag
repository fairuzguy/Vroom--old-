// Inputs
varying vec4 fragmentColor;
uniform mat4 u_ObjectInfo[100];
uniform float u_counter;
uniform vec4 u_iResolution; // xy = resolution, zw = mouse position
uniform mat4 u_camera;
uniform ivec4 u_state;

// Constants
const vec3 CLEAR_COLOR = vec3(0);
const vec3 RAY_ORIGIN = vec3(0, 1, 0);
const float MAX_DISTANCE = 100;
const int MAX_STEPS = 100;
const float TOLERANCE = 0.01;
const vec2 EPSILON = vec2(0.0001, 0);
const float SHADOW_SURF = 0.02;
const float SOFT_SHADOW_DIST = 0.2;
const float AO_MAX_STEPS = 5;
const int MAX_OBJECTS = 100;

// Derived values
vec2 uv = (gl_FragCoord.xy - u_iResolution.xy*0.5)/u_iResolution.y;
vec3 RayPosition = u_camera[0].xyz;
vec3 RightVector = u_camera[1].xyz;
vec3 UpVector = u_camera[2].xyz;
vec3 LookVector = u_camera[3].xyz;
float Zoom = u_camera[0].w;
vec3 RayDirection = normalize(Zoom*LookVector + uv.x*RightVector + uv.y*UpVector);
int NumObjects = u_state.x;

// Forward declarations
float GetDist(vec3 Position);
float RayMarch();
vec3 GetVector(vec3 Position, float Dist);
vec3 GetNormal(vec3 Position);
vec2 LightMarch(vec3 RayOrigin, vec3 LightDirection);
float AmbientOcclusion(vec3 RayOrigin, vec3 Normal);
float GetLight(vec3 Position);

// Putting it all together
void main() {
    vec3 col = CLEAR_COLOR;
    float d = RayMarch();
    vec3 p = RayPosition + RayDirection*d;
    float dif = GetLight(p);
    col = vec3(dif);
    //col = normalize(GetVector(p, GetDist(p)));
    //col = u_camera[0].wxz;
    //col = RayDirection;
    //col = uv.xyx;
    //col = u_ObjectInfo[0][0].xyz;
    gl_FragColor = vec4(col,1.0);
}

// Global distance function
float GetDist(vec3 Position) {
    //vec4 Sphere = vec4(0 + sin(u_counter), 1.5 + sin(u_counter + 3.14), 6 + cos(u_counter), 1);
    //float SphereDist = length(Position-Sphere.xyz) - Sphere.w;
    float PlaneDist = Position.y;
    float WallDist = 14 - Position.z;
    float SceneDist = min(WallDist, PlaneDist);

    // temporary sphere-only objects (simpler parameters)
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (i == NumObjects) {
            break;
        }
        int j = i;
        vec3 spherePosition = u_ObjectInfo[j][0].xyz;
        float sphereSize = u_ObjectInfo[j][0].w;
        float sphereDist = length(Position - spherePosition) - sphereSize;
        SceneDist = min(SceneDist, sphereDist);
    }
    //float Dist = min(WallDist, min(SphereDist, PlaneDist));
    return SceneDist;
}

// Raymarch
float RayMarch() {
    float DistTravelled = 0;
    for (int i = 0; i < MAX_STEPS; i++) {
        vec3 Position = RayPosition + RayDirection*DistTravelled;
        float Dist = GetDist(Position);
        DistTravelled += Dist;
        if (DistTravelled > MAX_DISTANCE || Dist < TOLERANCE) {
            break;
        }
    }
    return DistTravelled;
}

// Normal at a point (tetrahedron technique)
vec3 GetNormal(vec3 Position) { 
    const float h = 0.0001;
    const vec2 k = vec2(1, -1);
    return normalize(
        k.xyy*GetDist(Position + k.xyy*h) +
        k.yyx*GetDist(Position + k.yyx*h) +
        k.yxy*GetDist(Position + k.yxy*h) +
        k.xxx*GetDist(Position + k.xxx*h)
    );
}

// Approximation for vector to distance field (useless for now...)
vec3 GetVector(vec3 Position, float Dist) {
    const vec2 Unit = vec2(0.2, 0);
    vec3 Pos1 = (Position + Unit.xyy);
    vec3 Pos2 = (Position + Unit.yxy);
    vec3 Pos3 = (Position + Unit.yyx);
    float Dist1 = GetDist(Pos1);
    float Dist2 = GetDist(Pos2);
    float Dist3 = GetDist(Pos3);
    float DistSquare = Dist*Dist;
    vec3 Vector = 0.5*vec3(Dist1*Dist1 - DistSquare, Dist2*Dist2 - DistSquare, Dist3*Dist3 - DistSquare)/Unit.x;
    return Vector;
}

// Light backwards march
vec2 LightMarch(vec3 RayOrigin, vec3 LightDirection, float LightDist) {
    float DistTravelled = 0;
    float MinShadowDist = 1;
    float PreviousDist = 10000000;
    const float ShadowSharpness = 32;
    for (int i = 0; i < MAX_STEPS; i++) {
        // march
        vec3 Position = RayOrigin + LightDirection*DistTravelled;
        float Dist = GetDist(Position);
        DistTravelled += Dist;

        // check if we went too far
        if (DistTravelled > MAX_DISTANCE) {
            DistTravelled = 1000000000; // emphasizes we didn't reach the light
            break;
        }

        // check if we reached the light
        if (DistTravelled > LightDist - TOLERANCE) {
            DistTravelled += TOLERANCE; // adjust so we ignore the shadow case
            break;
        }

        // if we hit an object, this is a hard shadow
        vec3 BackNorm = GetNormal(Position);
        float Align = dot(BackNorm, LightDirection);
        if (Dist < TOLERANCE) {
           // if (Align < 0) { // make sure the object within tolerance is not behind us
                break;
           // } // This ended up being useless once I fixed soft shadows...
        }

        // Soft shadows
        float TriangulatedDist = Dist*Dist/(2.0*PreviousDist);
        float d = sqrt(Dist*Dist-TriangulatedDist*TriangulatedDist);
        MinShadowDist = min(MinShadowDist, ShadowSharpness*Dist/max(0, DistTravelled - TriangulatedDist));

        PreviousDist = Dist; // keeping track for soft shadows
    }
    return vec2(DistTravelled, MinShadowDist);
}

// Ambient Occlusion
float GetAmbientOcclusion(vec3 Position, vec3 Normal) {
    float DistTravelled = 0;
    float AO = 0;
    float AOStep = 0.5;
    for (int i = 0; i < AO_MAX_STEPS; i++) {
        float Dist = GetDist(Position + DistTravelled*Normal);
        DistTravelled += Dist;
        //if (Dist < 1) {
            AO += AOStep*Dist/clamp(DistTravelled, 1, 1000000);
        //}
        AOStep *= 0.5;
    }
    return AO;
}

// Basic light function
float GetLight(vec3 Position) {
    // Initial light setup
    vec3 LightPos = vec3(10, 5, 6);
    LightPos.xyz += vec3(sin(u_counter), 3*cos(u_counter), 2*cos(u_counter));
    vec3 LightDirection = normalize(LightPos - Position);
    vec3 Normal = GetNormal(Position);
    float Diffuse = clamp(dot(Normal, LightDirection), 0, 1);
    float LightDist = length(LightPos - Position);

    float GrayScale = Diffuse;

    // Applying shadows
    vec2 Shadow = LightMarch(Position + Normal*SHADOW_SURF*5, LightDirection, LightDist);
    if ((Shadow.x < LightDist)) {
        GrayScale = Shadow.x/LightDist*0.01;
    } else {
        GrayScale *= Shadow.y;
    }

    // Applying ambient occlusion
    float AO = GetAmbientOcclusion(Position + Normal*SHADOW_SURF*5, Normal);
    GrayScale += clamp(AO, 0, 1-Diffuse);

    return GrayScale;
}