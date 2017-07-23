TARGET=MazeGame

$(TARGET): game.o GenericTimer.o ImageData.o ImageIO.o main.o
	cc -o $(TARGET) game.o GenericTimer.o ImageData.o ImageIO.o main.o -framework GLUT -framework OpenGL -lm
	#-L. -lglut -lGL -lGLU -lXi -lXrandr -lm
	#-lglut -lGLU -lGL -lm
	#-framework GLUT -framework OpenGL
.c.o:
	cc -c $< -I. -Wall
run: $(TARGET)
	./$(TARGET)
clean:
	rm -f *.o *.c~ *.h~ $(TARGET)