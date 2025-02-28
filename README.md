# Attribute-Based Mapping

## Overview
This project implements **Attribute-Based Texture Mapping** using **Visual C++, OpenGL, and GLSL**. The goal is to apply texture mapping based on geometric and view-dependent attributes, inspired by **X-Toon: An Extended Toon Shader (Barla et al., NPAR ‘06)**.

## Features
- **OBJ model loading** (provided loader)
- **PPM texture loading** (provided loader)
- **Camera and light manipulation**
- **Two mapping techniques**:
  - **Orientation-based mapping**: Tone detail effects based on surface normal and view vector.
  - **Depth-based mapping**: Tone detail effects based on object depth.

## Texture Mapping Formulation

### 1. **Lambertian (Diffuse) Shading**
The texture coordinate **s-axis** is derived from Lambertian shading:

\[
s = \mathbf{N} \cdot \mathbf{L}
\]

where:
- \( \mathbf{N} \) = Surface normal (unit vector)
- \( \mathbf{L} \) = Light direction (unit vector)

### 2. **Tone Detail (View-Dependent) Mapping**
The **t-axis** of the texture space is determined by the attribute function \( D \):

\[
t = D(z, \mathbf{N}, \mathbf{V}, r)
\]

where:
- \( z \) = Depth of vertex
- \( \mathbf{V} \) = View vector
- \( r \) = User-controlled parameter

**Special Cases**:
- **Silhouette & Backlighting:** 
  \[
  t = D(\mathbf{N}, \mathbf{V}, r)
  \]
- **Aerial Perspective:** 
  \[
  t = D(z, z_{\min}, r)
  \]

## Implementation Details
- **Orientation-Based Mapping**
  - Highlights near silhouettes
  - Simulates plastic and metallic shading
  - Controlled via **r** parameter

- **Depth-Based Mapping**
  - Simulates aerial perspective
  - Controlled via **zmin** and **r** parameters

## Models & Textures
- **Test models:** Sphere, head, boss, bunny (for orientation-based)
- **Terrain models:** Terrain-1, Terrain-2, Terrain-3 (for depth-based)
- **Textures:** Predefined for specific tone mapping effects.

## Usage Instructions
1. **Compile & Run** using Visual Studio and OpenGL.
2. Load **OBJ models** and **PPM textures**.
3. Adjust camera and light positions.
4. Select mapping effect and tune parameters.

## References
- **Barla et al., "X-Toon: An Extended Toon Shader," NPAR ‘06**.
- University course materials on rendering and attribute-based mapping.

---

This README gives a **clear project summary, equations, features, and usage details**. Let me know if you want any modifications! 🚀
