# Attribute-Based Mapping
## Overview
This project implements **Attribute-Based Texture Mapping** using **Visual C++, OpenGL, and GLSL**. The goal is to apply texture mapping based on geometric and view-dependent attributes, inspired by **X-Toon: An Extended Toon Shader (Barla et al., NPAR ‘06)**.

**half silhouette:**

<img src="https://github.com/user-attachments/assets/115a7d55-954b-4295-abd4-62b274417798" width="512">

**Gold:**

<img src="https://github.com/user-attachments/assets/d39c95b3-1288-4a80-a7c9-1157c10ce2a2" width="512">

**Depth-based mapping:**

![CPSC591_691A12025-02-2719-07-03online-video-cutter com-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/3f85e651-97e2-4265-8fb5-271699fa1404)

![CPSC591_691A12025-02-2719-07-38online-video-cutter com-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/855fc24d-e62a-438d-9f4e-ffb6c283b5f3)


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

$$
s = \mathbf{N} \cdot \mathbf{L}
$$

where:
- $\mathbf{N}$ = Surface normal (unit vector)
- $\mathbf{L}$ = Light direction (unit vector)

### 2. **Tone Detail (View-Dependent) Mapping**
The **t-axis** of the texture space is determined by the attribute function $D$:

$$
t = D(z, \mathbf{N}, \mathbf{V}, r)
$$

where:
- $z$ = Depth of vertex
- $\mathbf{N}$ = Surface normal
- $\mathbf{V}$ = View vector
- $r$ = User-controlled parameter

**Special Cases**:
- **Silhouette & Backlighting:**
  $t = D(\mathbf{N}, \mathbf{V}, r)$

- **Aerial Perspective:**
 $t = D(z, z_{\min}, r)$

## Implementation Details
- **Orientation-Based Mapping**
  - Highlights near silhouettes
  - Simulates plastic and metallic shading
  - Controlled via **r** parameter

- **Depth-Based Mapping**
  - Simulates aerial perspective
  - Controlled via $z_{\min}$ and $r$ parameters

## Models & Textures
- **Test models:** Sphere, head, boss, bunny (for orientation-based)
- **Terrain models:** Terrain-1, Terrain-2, Terrain-3 (for depth-based)
- **Textures:** Predefined for specific tone mapping effects.

