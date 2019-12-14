read projectName
read outputName
g++ -o ../run/$outputName $projectName  glad.c -lglfw3 -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ldl -lXinerama -lXcursor
../run/$outputName