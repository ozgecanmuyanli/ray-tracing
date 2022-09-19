# Ray Tracer

Implementation of Peter Shirley's Ray Tracing In One Weekend book to learn the concept.

Implemented features:
* adding spheres
* ray-sphere intersection
* calculating surface normals
* antialiasing
* diffuse material
* gamma-correction
* metal material
* dielectrics
* positioning the camera
* defocus blur (depth of field)

**A series of rendered images from my ray-tracer:**

![final_render4](https://user-images.githubusercontent.com/96859854/191037191-25eaf50e-698b-40c8-b92a-a00371e0af40.png)
![final_render3](https://user-images.githubusercontent.com/96859854/191037232-051f1801-8e02-4773-a4ad-9690475adc47.png)
![camera](https://user-images.githubusercontent.com/96859854/191037534-17c49c12-f332-4c14-b728-1637dbdaec68.png)
![glass_sphere](https://user-images.githubusercontent.com/96859854/191037361-5364f580-fefb-4216-84b8-b39b131317e3.png)
![fuzzy_metal](https://user-images.githubusercontent.com/96859854/191037557-5ccf81a0-2300-46aa-a7d7-d5000c74b4ff.png)
![sphere_normals](https://user-images.githubusercontent.com/96859854/191037602-b11c4282-b5b1-46e1-b5d9-9f2c49f361dc.png)


## Compilation: Build Project for VS 2022
    rm -rf <build_folder>
    mkdir <build_folder>
    cmake -G "Visual Studio 17" -S . -B <build_folder>
    cmake --build <build_folder>
    
