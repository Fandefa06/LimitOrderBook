# 1. Versión mínima de CMake (la que tiene Ubuntu 24.04 es superior a esta)
cmake_minimum_required(VERSION 3.20)

# 2. Nombre de tu proyecto y lenguaje (CXX es C++)
project(LimitOrderBook CXX)

# 3. Exigir C++20 para usar las bondades modernas de las que hablamos
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 4. EXPLICAR DÓNDE ESTÁN LAS CABECERAS (Esto quita el error del #include)
# Le decimos que busque en la carpeta 'include'
include_directories(include)

# 5. CREAR EL EJECUTABLE
# El primer nombre es como se llamará tu programa.
# Los siguientes son los archivos .cpp que debe compilar.
add_executable(MotorHFT 
    src/main.cpp
)